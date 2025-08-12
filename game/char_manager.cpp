#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "desc.h"
#include "char.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "party.h"
#include "regen.h"
#include "p2p.h"
#include "dungeon.h"
#include "db.h"
#include "config.h"
#include "xmas_event.h"
#include "questmanager.h"
#include "questlua.h"
#include "locale_service.h"
#include "shutdown_manager.h"
#include "../../common/CommonDefines.h"

#include "desc_client.h"
#include "desc_manager.h"

#if (!defined(__GNUC__) || defined(__clang__)) && !defined(CXX11_ENABLED)
#	include <boost/bind.hpp>

#elif defined(CXX11_ENABLED)
#	include <functional>
	template <typename T>
	decltype(std::bind(&T::second, std::placeholders::_1)) select2nd()
	{
		return std::bind(&T::second, std::placeholders::_1);
	}
#endif

CHARACTER_MANAGER::CHARACTER_MANAGER() :
	m_iVIDCount(0),
	m_pkChrSelectedStone(NULL),
	m_bUsePendingDestroy(false)
{
	RegisterRaceNum(xmas::MOB_XMAS_FIRWORK_SELLER_VNUM);
	RegisterRaceNum(xmas::MOB_SANTA_VNUM);
	RegisterRaceNum(xmas::MOB_XMAS_TREE_VNUM);

	m_iMobItemRate = 100;
	m_iMobDamageRate = 100;
	m_iMobGoldAmountRate = 100;
	m_iMobGoldDropRate = 100;
	m_iMobExpRate = 100;

	m_iMobItemRatePremium = 100;
	m_iMobGoldAmountRatePremium = 100;
	m_iMobGoldDropRatePremium = 100;
	m_iMobExpRatePremium = 100;

	m_iUserDamageRate = 100;
	m_iUserDamageRatePremium = 100;
	
#ifdef ENABLE_EVENT_MANAGER
	m_eventData.clear();
#endif
}

CHARACTER_MANAGER::~CHARACTER_MANAGER()
{
	Destroy();
}

void CHARACTER_MANAGER::Destroy()
{
	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.begin();
	while (it != m_map_pkChrByVID.end()) {
		LPCHARACTER ch = it->second;
		M2_DESTROY_CHARACTER(ch); // m_map_pkChrByVID is changed here
		it = m_map_pkChrByVID.begin();
#ifdef ENABLE_EVENT_MANAGER
		m_eventData.clear();
#endif
	}
}

void CHARACTER_MANAGER::GracefulShutdown()
{
	NAME_MAP::iterator it = m_map_pkPCChr.begin();

	while (it != m_map_pkPCChr.end())
		(it++)->second->Disconnect("GracefulShutdown");
}

DWORD CHARACTER_MANAGER::AllocVID()
{
	++m_iVIDCount;
	return m_iVIDCount;
}

LPCHARACTER CHARACTER_MANAGER::CreateCharacter(const char * name, DWORD dwPID)
{
	DWORD dwVID = AllocVID();

#ifdef M2_USE_POOL
	LPCHARACTER ch = pool_.Construct();
#else
	LPCHARACTER ch = M2_NEW CHARACTER;
#endif
	ch->Create(name, dwVID, dwPID ? true : false);

	m_map_pkChrByVID.emplace(dwVID, ch);

	if (dwPID)
	{
		char szName[CHARACTER_NAME_MAX_LEN + 1];
		str_lower(name, szName, sizeof(szName));

		m_map_pkPCChr.emplace(szName, ch);
		m_map_pkChrByPID.emplace(dwPID, ch);
	}

	return (ch);
}

#ifndef DEBUG_ALLOC
void CHARACTER_MANAGER::DestroyCharacter(LPCHARACTER ch)
#else
void CHARACTER_MANAGER::DestroyCharacter(LPCHARACTER ch, const char* file, size_t line)
#endif
{
	if (!ch)
		return;

	// <Factor> Check whether it has been already deleted or not.
	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.find(ch->GetVID());
	if (it == m_map_pkChrByVID.end()) {
		sys_err("[CHARACTER_MANAGER::DestroyCharacter] <Factor> %d not found", (long)(ch->GetVID()));
		return; // prevent duplicated destrunction
	}

	if (ch->IsNPC() && !ch->IsPet() && ch->GetRider() == NULL)
	{
		if (ch->GetDungeon())
		{
			ch->GetDungeon()->DeadCharacter(ch);
		}
	}

	if (m_bUsePendingDestroy)
	{
		m_set_pkChrPendingDestroy.emplace(ch);
		return;
	}

	m_map_pkChrByVID.erase(it);

	if (true == ch->IsPC())
	{
		char szName[CHARACTER_NAME_MAX_LEN + 1];

		str_lower(ch->GetName(), szName, sizeof(szName));

		NAME_MAP::iterator it = m_map_pkPCChr.find(szName);

		if (m_map_pkPCChr.end() != it)
			m_map_pkPCChr.erase(it);
	}

	if (0 != ch->GetPlayerID())
	{
		itertype(m_map_pkChrByPID) it = m_map_pkChrByPID.find(ch->GetPlayerID());

		if (m_map_pkChrByPID.end() != it)
		{
			m_map_pkChrByPID.erase(it);
		}
	}

	UnregisterRaceNumMap(ch);

	RemoveFromStateList(ch);

#ifdef M2_USE_POOL
	pool_.Destroy(ch);
#else
#ifndef DEBUG_ALLOC
	M2_DELETE(ch);
#else
	M2_DELETE_EX(ch, file, line);
#endif
#endif
}

LPCHARACTER CHARACTER_MANAGER::Find(DWORD dwVID)
{
	itertype(m_map_pkChrByVID) it = m_map_pkChrByVID.find(dwVID);

	if (m_map_pkChrByVID.end() == it)
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && dwVID != (DWORD)found->GetVID()) {
		sys_err("[CHARACTER_MANAGER::Find] <Factor> %u != %u", dwVID, (DWORD)found->GetVID());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::Find(const VID & vid)
{
	LPCHARACTER tch = Find((DWORD) vid);

	if (!tch || tch->GetVID() != vid)
		return NULL;

	return tch;
}

LPCHARACTER CHARACTER_MANAGER::FindByPID(DWORD dwPID)
{
	itertype(m_map_pkChrByPID) it = m_map_pkChrByPID.find(dwPID);

	if (m_map_pkChrByPID.end() == it)
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && dwPID != found->GetPlayerID()) {
		sys_err("[CHARACTER_MANAGER::FindByPID] <Factor> %u != %u", dwPID, found->GetPlayerID());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::FindPC(const char * name)
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	str_lower(name, szName, sizeof(szName));
	NAME_MAP::iterator it = m_map_pkPCChr.find(szName);

	if (it == m_map_pkPCChr.end())
		return NULL;

	// <Factor> Added sanity check
	LPCHARACTER found = it->second;
	if (found != NULL && strncasecmp(szName, found->GetName(), CHARACTER_NAME_MAX_LEN) != 0) {
		sys_err("[CHARACTER_MANAGER::FindPC] <Factor> %s != %s", name, found->GetName());
		return NULL;
	}
	return found;
}

LPCHARACTER CHARACTER_MANAGER::SpawnMobRandomPosition(DWORD dwVnum, long lMapIndex, bool is_aggressive)
{
	{
		if (dwVnum == 5001 && !quest::CQuestManager::instance().GetEventFlag("japan_regen"))
		{
			sys_log(1, "WAEGU[5001] regen disabled.");
			return NULL;
		}
	}

	{
		if (dwVnum == 5002 && !quest::CQuestManager::instance().GetEventFlag("newyear_mob"))
		{
			sys_log(1, "HAETAE (new-year-mob) [5002] regen disabled.");
			return NULL;
		}
	}

	{
		if (dwVnum == 5004 && !quest::CQuestManager::instance().GetEventFlag("independence_day"))
		{
			sys_log(1, "INDEPENDECE DAY [5004] regen disabled.");
			return NULL;
		}
	}

	const CMob * pkMob = CMobManager::instance().Get(dwVnum);

	if (!pkMob)
	{
		sys_err("no mob data for vnum %u", dwVnum);
		return NULL;
	}

	if (!map_allow_find(lMapIndex))
	{
		sys_err("not allowed map %u", lMapIndex);
		return NULL;
	}

	LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
	if (pkSectreeMap == NULL) {
		return NULL;
	}

	int i;
	long x, y;
	for (i=0; i<2000; i++)
	{
		x = number(1, (pkSectreeMap->m_setting.iWidth / 100)  - 1) * 100 + pkSectreeMap->m_setting.iBaseX;
		y = number(1, (pkSectreeMap->m_setting.iHeight / 100) - 1) * 100 + pkSectreeMap->m_setting.iBaseY;
		//LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);
		LPSECTREE tree = pkSectreeMap->Find(x, y);

		if (!tree)
			continue;

		DWORD dwAttr = tree->GetAttribute(x, y);

		if (IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT))
			continue;

		if (IS_SET(dwAttr, ATTR_BANPK))
			continue;

		break;
	}

	if (i == 2000)
	{
		sys_err("cannot find valid location");
		return NULL;
	}

	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "SpawnMobRandomPosition: cannot create monster at non-exist sectree %d x %d (map %d)", x, y, lMapIndex);
		return NULL;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().CreateCharacter(pkMob->m_table.szLocaleName);

	if (!ch)
	{
		sys_log(0, "SpawnMobRandomPosition: cannot create new character");
		return NULL;
	}

	ch->SetProto(pkMob);

	// if mob is npc with no empire assigned, assign to empire of map
	if (pkMob->m_table.bType == CHAR_TYPE_NPC)
		if (ch->GetEmpire() == 0)
			ch->SetEmpire(SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex));

	ch->SetRotation(number(0, 360));
	if (is_aggressive) //@fixme195
		ch->SetAggressive();

	if (!ch->Show(lMapIndex, x, y, 0, false))
	{
		M2_DESTROY_CHARACTER(ch);
		sys_err(0, "SpawnMobRandomPosition: cannot show monster");
		return NULL;
	}

	char buf[512+1];
	long local_x = x - pkSectreeMap->m_setting.iBaseX;
	long local_y = y - pkSectreeMap->m_setting.iBaseY;
	snprintf(buf, sizeof(buf), "spawn %s[%d] random position at %ld %ld %ld %ld (time: %d)", ch->GetName(), dwVnum, x, y, local_x, local_y, get_global_time());

	if (test_server)
		SendNotice(buf);

	sys_log(0, buf);
	return (ch);
}

LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow)
{
	const CMob * pkMob = CMobManager::instance().Get(dwVnum);
	if (!pkMob)
	{
		sys_err("SpawnMob: no mob data for vnum %u", dwVnum);
		return NULL;
	}

	if (!(pkMob->m_table.bType == CHAR_TYPE_NPC || pkMob->m_table.bType == CHAR_TYPE_WARP || pkMob->m_table.bType == CHAR_TYPE_GOTO) || mining::IsVeinOfOre (dwVnum))
	{
		LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

		if (!tree)
		{
			sys_log(0, "no sectree for spawn at %d %d mobvnum %d mapindex %d", x, y, dwVnum, lMapIndex);
			return NULL;
		}

		DWORD dwAttr = tree->GetAttribute(x, y);

		bool is_set = false;

		if ( mining::IsVeinOfOre (dwVnum) ) is_set = IS_SET(dwAttr, ATTR_BLOCK);
		else is_set = IS_SET(dwAttr, ATTR_BLOCK | ATTR_OBJECT);

		if ( is_set )
		{
			// SPAWN_BLOCK_LOG
			static bool s_isLog=quest::CQuestManager::instance().GetEventFlag("spawn_block_log");
			static DWORD s_nextTime=get_global_time()+10000;

			DWORD curTime=get_global_time();

			if (curTime>s_nextTime)
			{
				s_nextTime=curTime;
				s_isLog=quest::CQuestManager::instance().GetEventFlag("spawn_block_log");

			}

			if (s_isLog)
				sys_log(0, "SpawnMob: BLOCKED position for spawn %s %u at %d %d (attr %u)", pkMob->m_table.szName, dwVnum, x, y, dwAttr);
			// END_OF_SPAWN_BLOCK_LOG
			return NULL;
		}

		if (IS_SET(dwAttr, ATTR_BANPK))
		{
			sys_log(0, "SpawnMob: BAN_PK position for mob spawn %s %u at %d %d", pkMob->m_table.szName, dwVnum, x, y);
			return NULL;
		}
	}

	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "SpawnMob: cannot create monster at non-exist sectree %d x %d (map %d)", x, y, lMapIndex);
		return NULL;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().CreateCharacter(pkMob->m_table.szLocaleName);

	if (!ch)
	{
		sys_log(0, "SpawnMob: cannot create new character");
		return NULL;
	}

	if (iRot == -1)
		iRot = number(0, 360);

	ch->SetProto(pkMob);

	// if mob is npc with no empire assigned, assign to empire of map
	if (pkMob->m_table.bType == CHAR_TYPE_NPC)
		if (ch->GetEmpire() == 0)
			ch->SetEmpire(SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex));

	ch->SetRotation(iRot);

	if (bShow && !ch->Show(lMapIndex, x, y, z, bSpawnMotion))
	{
		M2_DESTROY_CHARACTER(ch);
		sys_log(0, "SpawnMob: cannot show monster");
		return NULL;
	}

	return (ch);
}

LPCHARACTER CHARACTER_MANAGER::SpawnMobRange(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, bool bIsException, bool bSpawnMotion, bool bAggressive )
{
	const CMob * pkMob = CMobManager::instance().Get(dwVnum);

	if (!pkMob)
		return NULL;

	if (pkMob->m_table.bType == CHAR_TYPE_STONE)
		bSpawnMotion = true;

	int i = 16;

	while (i--)
	{
		int x = number(sx, ex);
		int y = number(sy, ey);
		/*
		   if (bIsException)
		   if (is_regen_exception(x, y))
		   continue;
		 */
		LPCHARACTER ch = SpawnMob(dwVnum, lMapIndex, x, y, 0, bSpawnMotion);

		if (ch)
		{
			sys_log(1, "MOB_SPAWN: %s(%d) %dx%d", ch->GetName(), (DWORD) ch->GetVID(), ch->GetX(), ch->GetY());
			if (bAggressive)
				ch->SetAggressive();
			return (ch);
		}
	}

	return NULL;
}

void CHARACTER_MANAGER::SelectStone(LPCHARACTER pkChr)
{
	m_pkChrSelectedStone = pkChr;
}

bool CHARACTER_MANAGER::SpawnMoveGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, int tx, int ty, LPREGEN pkRegen, bool bAggressive_)
{
	CMobGroup * pkGroup = CMobManager::Instance().GetGroup(dwVnum);

	if (!pkGroup)
	{
		sys_err("NOT_EXIST_GROUP_VNUM(%u) Map(%u) ", dwVnum, lMapIndex);
		return false;
	}

	LPCHARACTER pkChrMaster = NULL;
	LPPARTY pkParty = NULL;

	const std::vector<DWORD> & c_rdwMembers = pkGroup->GetMemberVector();

	bool bSpawnedByStone = false;
	bool bAggressive = bAggressive_;

	if (m_pkChrSelectedStone)
	{
		bSpawnedByStone = true;
		if (m_pkChrSelectedStone->GetDungeon())
			bAggressive = true;
	}

	for (DWORD i = 0; i < c_rdwMembers.size(); ++i)
	{
		LPCHARACTER tch = SpawnMobRange(c_rdwMembers[i], lMapIndex, sx, sy, ex, ey, true, bSpawnedByStone);

		if (!tch)
		{
			if (i == 0)
				return false;

			continue;
		}

		sx = tch->GetX() - number(300, 500);
		sy = tch->GetY() - number(300, 500);
		ex = tch->GetX() + number(300, 500);
		ey = tch->GetY() + number(300, 500);

		if (m_pkChrSelectedStone)
			tch->SetStone(m_pkChrSelectedStone);
		else if (pkParty)
		{
			pkParty->Join(tch->GetVID());
			pkParty->Link(tch);
		}
		else if (!pkChrMaster)
		{
			pkChrMaster = tch;
			pkChrMaster->SetRegen(pkRegen);

			pkParty = CPartyManager::instance().CreateParty(pkChrMaster);
		}
		if (bAggressive)
			tch->SetAggressive();

		if (tch->Goto(tx, ty))
			tch->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	}

	return true;
}

bool CHARACTER_MANAGER::SpawnGroupGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, LPREGEN pkRegen, bool bAggressive_, LPDUNGEON pDungeon)
{
	const DWORD dwGroupID = CMobManager::Instance().GetGroupFromGroupGroup(dwVnum);

	if( dwGroupID != 0 )
	{
		return SpawnGroup(dwGroupID, lMapIndex, sx, sy, ex, ey, pkRegen, bAggressive_, pDungeon);
	}
	else
	{
		sys_err( "NOT_EXIST_GROUP_GROUP_VNUM(%u) MAP(%ld)", dwVnum, lMapIndex );
		return false;
	}
}

LPCHARACTER CHARACTER_MANAGER::SpawnGroup(DWORD dwVnum, long lMapIndex, int sx, int sy, int ex, int ey, LPREGEN pkRegen, bool bAggressive_, LPDUNGEON pDungeon)
{
	CMobGroup * pkGroup = CMobManager::Instance().GetGroup(dwVnum);

	if (!pkGroup)
	{
		sys_err("NOT_EXIST_GROUP_VNUM(%u) Map(%u) ", dwVnum, lMapIndex);
		return NULL;
	}

	LPCHARACTER pkChrMaster = NULL;
	LPPARTY pkParty = NULL;

	const std::vector<DWORD> & c_rdwMembers = pkGroup->GetMemberVector();

	bool bSpawnedByStone = false;
	bool bAggressive = bAggressive_;

	if (m_pkChrSelectedStone)
	{
		bSpawnedByStone = true;

		if (m_pkChrSelectedStone->GetDungeon())
			bAggressive = true;
	}

	LPCHARACTER chLeader = NULL;

	for (DWORD i = 0; i < c_rdwMembers.size(); ++i)
	{
		LPCHARACTER tch = SpawnMobRange(c_rdwMembers[i], lMapIndex, sx, sy, ex, ey, true, bSpawnedByStone);

		if (!tch)
		{
			if (i == 0)
				return NULL;

			continue;
		}

		if (i == 0)
			chLeader = tch;

		tch->SetDungeon(pDungeon);

		sx = tch->GetX() - number(300, 500);
		sy = tch->GetY() - number(300, 500);
		ex = tch->GetX() + number(300, 500);
		ey = tch->GetY() + number(300, 500);

		if (m_pkChrSelectedStone)
			tch->SetStone(m_pkChrSelectedStone);
		else if (pkParty)
		{
			pkParty->Join(tch->GetVID());
			pkParty->Link(tch);
		}
		else if (!pkChrMaster)
		{
			pkChrMaster = tch;
			pkChrMaster->SetRegen(pkRegen);

			pkParty = CPartyManager::instance().CreateParty(pkChrMaster);
		}

		if (bAggressive)
			tch->SetAggressive();
	}

	return chLeader;
}

struct FuncUpdateAndResetChatCounter
{
	void operator () (LPCHARACTER ch)
	{
		ch->ResetChatCounter();
		ch->CFSM::Update();
	}
};

void CHARACTER_MANAGER::Update(int iPulse)
{
	using namespace std;
#if defined(__GNUC__) && !defined(__clang__) && !defined(CXX11_ENABLED)
	using namespace __gnu_cxx;
#endif

	BeginPendingDestroy();

	{
		if (!m_map_pkPCChr.empty())
		{
			CHARACTER_VECTOR v;
			v.reserve(m_map_pkPCChr.size());
#if (defined(__GNUC__) && !defined(__clang__)) || defined(CXX11_ENABLED)
			transform(m_map_pkPCChr.begin(), m_map_pkPCChr.end(), back_inserter(v), select2nd<NAME_MAP::value_type>());
#else
			transform(m_map_pkPCChr.begin(), m_map_pkPCChr.end(), back_inserter(v), boost::bind(&NAME_MAP::value_type::second, _1));
#endif

			if (0 == (iPulse % PASSES_PER_SEC(5)))
			{
				FuncUpdateAndResetChatCounter f;
				for_each(v.begin(), v.end(), f);
			}
			else
			{
				for_each(v.begin(), v.end(), msl::bind2nd(std::mem_fn(&CHARACTER::UpdateCharacter), iPulse));
			}
		}

	}

	{
		if (!m_set_pkChrState.empty())
		{
			CHARACTER_VECTOR v;
			v.reserve(m_set_pkChrState.size());
#if defined(__GNUC__) && !defined(__clang__) && !defined(CXX11_ENABLED)
			transform(m_set_pkChrState.begin(), m_set_pkChrState.end(), back_inserter(v), identity<CHARACTER_SET::value_type>());
#else
			v.insert(v.end(), m_set_pkChrState.begin(), m_set_pkChrState.end());
#endif
			for_each(v.begin(), v.end(), msl::bind2nd(std::mem_fn(&CHARACTER::UpdateStateMachine), iPulse));
		}
	}

	{
		CharacterVectorInteractor i;

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(xmas::MOB_SANTA_VNUM, i))
		{
			for_each(i.begin(), i.end(),
					msl::bind2nd(std::mem_fn(&CHARACTER::UpdateStateMachine), iPulse));
		}
	}

	if (0 == (iPulse % PASSES_PER_SEC(3600)))
	{
		for (itertype(m_map_dwMobKillCount) it = m_map_dwMobKillCount.begin(); it != m_map_dwMobKillCount.end(); ++it)
			DBManager::instance().SendMoneyLog(MONEY_LOG_MONSTER_KILL, it->first, it->second);

		m_map_dwMobKillCount.clear();
	}

	if (test_server && 0 == (iPulse % PASSES_PER_SEC(60)))
		sys_log(0, "CHARACTER COUNT vid %zu pid %zu", m_map_pkChrByVID.size(), m_map_pkChrByPID.size());

	FlushPendingDestroy();

	// ShutdownManager Update
	CShutdownManager::Instance().Update();
}

void CHARACTER_MANAGER::ProcessDelayedSave()
{
	CHARACTER_SET::iterator it = m_set_pkChrForDelayedSave.begin();

	while (it != m_set_pkChrForDelayedSave.end())
	{
		LPCHARACTER pkChr = *it++;
		pkChr->SaveReal();
	}

	m_set_pkChrForDelayedSave.clear();
}

bool CHARACTER_MANAGER::AddToStateList(LPCHARACTER ch)
{
	assert(ch != NULL);

	CHARACTER_SET::iterator it = m_set_pkChrState.find(ch);

	if (it == m_set_pkChrState.end())
	{
		m_set_pkChrState.emplace(ch);
		return true;
	}

	return false;
}

void CHARACTER_MANAGER::RemoveFromStateList(LPCHARACTER ch)
{
	CHARACTER_SET::iterator it = m_set_pkChrState.find(ch);

	if (it != m_set_pkChrState.end())
	{
		//sys_log(0, "RemoveFromStateList %p", ch);
		m_set_pkChrState.erase(it);
	}
}

void CHARACTER_MANAGER::DelayedSave(LPCHARACTER ch)
{
	m_set_pkChrForDelayedSave.emplace(ch);
}

bool CHARACTER_MANAGER::FlushDelayedSave(LPCHARACTER ch)
{
	CHARACTER_SET::iterator it = m_set_pkChrForDelayedSave.find(ch);

	if (it == m_set_pkChrForDelayedSave.end())
		return false;

	m_set_pkChrForDelayedSave.erase(it);
	ch->SaveReal();
	return true;
}

void CHARACTER_MANAGER::RegisterForMonsterLog(LPCHARACTER ch)
{
	m_set_pkChrMonsterLog.emplace(ch);
}

void CHARACTER_MANAGER::UnregisterForMonsterLog(LPCHARACTER ch)
{
	m_set_pkChrMonsterLog.erase(ch);
}

void CHARACTER_MANAGER::PacketMonsterLog(LPCHARACTER ch, const void* buf, int size)
{
	itertype(m_set_pkChrMonsterLog) it;

	for (it = m_set_pkChrMonsterLog.begin(); it!=m_set_pkChrMonsterLog.end();++it)
	{
		LPCHARACTER c = *it;

		if (ch && DISTANCE_APPROX(c->GetX()-ch->GetX(), c->GetY()-ch->GetY())>6000)
			continue;

		LPDESC d = c->GetDesc();

		if (d)
			d->Packet(buf, size);
	}
}

void CHARACTER_MANAGER::KillLog(DWORD dwVnum)
{
	const DWORD SEND_LIMIT = 10000;

	itertype(m_map_dwMobKillCount) it = m_map_dwMobKillCount.find(dwVnum);

	if (it == m_map_dwMobKillCount.end())
		m_map_dwMobKillCount.emplace(dwVnum, 1);
	else
	{
		++it->second;

		if (it->second > SEND_LIMIT)
		{
			DBManager::instance().SendMoneyLog(MONEY_LOG_MONSTER_KILL, it->first, it->second);
			m_map_dwMobKillCount.erase(it);
		}
	}
}

void CHARACTER_MANAGER::RegisterRaceNum(DWORD dwVnum)
{
	m_set_dwRegisteredRaceNum.emplace(dwVnum);
}

void CHARACTER_MANAGER::RegisterRaceNumMap(LPCHARACTER ch)
{
	DWORD dwVnum = ch->GetRaceNum();

	if (m_set_dwRegisteredRaceNum.find(dwVnum) != m_set_dwRegisteredRaceNum.end())
	{
		sys_log(0, "RegisterRaceNumMap %s %u", ch->GetName(), dwVnum);
		m_map_pkChrByRaceNum[dwVnum].emplace(ch);
	}
}

void CHARACTER_MANAGER::UnregisterRaceNumMap(LPCHARACTER ch)
{
	DWORD dwVnum = ch->GetRaceNum();

	itertype(m_map_pkChrByRaceNum) it = m_map_pkChrByRaceNum.find(dwVnum);

	if (it != m_map_pkChrByRaceNum.end())
		it->second.erase(ch);
}

bool CHARACTER_MANAGER::GetCharactersByRaceNum(DWORD dwRaceNum, CharacterVectorInteractor & i)
{
	std::map<DWORD, CHARACTER_SET>::iterator it = m_map_pkChrByRaceNum.find(dwRaceNum);

	if (it == m_map_pkChrByRaceNum.end())
		return false;

	i = it->second;
	return true;
}

#define FIND_JOB_WARRIOR_0	(1 << 3)
#define FIND_JOB_WARRIOR_1	(1 << 4)
#define FIND_JOB_WARRIOR_2	(1 << 5)
#define FIND_JOB_WARRIOR	(FIND_JOB_WARRIOR_0 | FIND_JOB_WARRIOR_1 | FIND_JOB_WARRIOR_2)
#define FIND_JOB_ASSASSIN_0	(1 << 6)
#define FIND_JOB_ASSASSIN_1	(1 << 7)
#define FIND_JOB_ASSASSIN_2	(1 << 8)
#define FIND_JOB_ASSASSIN	(FIND_JOB_ASSASSIN_0 | FIND_JOB_ASSASSIN_1 | FIND_JOB_ASSASSIN_2)
#define FIND_JOB_SURA_0		(1 << 9)
#define FIND_JOB_SURA_1		(1 << 10)
#define FIND_JOB_SURA_2		(1 << 11)
#define FIND_JOB_SURA		(FIND_JOB_SURA_0 | FIND_JOB_SURA_1 | FIND_JOB_SURA_2)
#define FIND_JOB_SHAMAN_0	(1 << 12)
#define FIND_JOB_SHAMAN_1	(1 << 13)
#define FIND_JOB_SHAMAN_2	(1 << 14)
#define FIND_JOB_SHAMAN		(FIND_JOB_SHAMAN_0 | FIND_JOB_SHAMAN_1 | FIND_JOB_SHAMAN_2)

//
// (job+1)*3+(skill_group)
//
LPCHARACTER CHARACTER_MANAGER::FindSpecifyPC(unsigned int uiJobFlag, long lMapIndex, LPCHARACTER except, int iMinLevel, int iMaxLevel)
{
	LPCHARACTER chFind = NULL;
	itertype(m_map_pkChrByPID) it;
	int n = 0;

	for (it = m_map_pkChrByPID.begin(); it != m_map_pkChrByPID.end(); ++it)
	{
		LPCHARACTER ch = it->second;

		if (ch == except)
			continue;

		if (ch->GetLevel() < iMinLevel)
			continue;

		if (ch->GetLevel() > iMaxLevel)
			continue;

		if (ch->GetMapIndex() != lMapIndex)
			continue;

		if (uiJobFlag)
		{
			unsigned int uiChrJob = (1 << ((ch->GetJob() + 1) * 3 + ch->GetSkillGroup()));

			if (!IS_SET(uiJobFlag, uiChrJob))
				continue;
		}

		if (!chFind || number(1, ++n) == 1)
			chFind = ch;
	}

	return chFind;
}

int CHARACTER_MANAGER::GetMobItemRate(LPCHARACTER ch)
{
	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0)
				return m_iMobItemRatePremium/2;
			return m_iMobItemRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_ITEM) > 0)
		return m_iMobItemRatePremium;
	return m_iMobItemRate;
}

int CHARACTER_MANAGER::GetMobDamageRate(LPCHARACTER ch)
{
	return m_iMobDamageRate;
}

int CHARACTER_MANAGER::GetMobGoldAmountRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobGoldAmountRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
				return m_iMobGoldAmountRatePremium/2;
			return m_iMobGoldAmountRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
		return m_iMobGoldAmountRatePremium;
	return m_iMobGoldAmountRate;
}

int CHARACTER_MANAGER::GetMobGoldDropRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobGoldDropRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
				return m_iMobGoldDropRatePremium/2;
			return m_iMobGoldDropRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_GOLD) > 0)
		return m_iMobGoldDropRatePremium;
	return m_iMobGoldDropRate;
}

int CHARACTER_MANAGER::GetMobExpRate(LPCHARACTER ch)
{
	if ( !ch )
		return m_iMobExpRate;

	//PREVENT_TOXICATION_FOR_CHINA
	if (g_bChinaIntoxicationCheck)
	{
		if ( ch->IsOverTime( OT_3HOUR ) )
		{
			if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
				return m_iMobExpRatePremium/2;
			return m_iMobExpRate/2;
		}
		else if ( ch->IsOverTime( OT_5HOUR ) )
		{
			return 0;
		}
	}
	//END_PREVENT_TOXICATION_FOR_CHINA
	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
		return m_iMobExpRatePremium;
	return m_iMobExpRate;
}

int	CHARACTER_MANAGER::GetUserDamageRate(LPCHARACTER ch)
{
	if (!ch)
		return m_iUserDamageRate;

	if (ch && ch->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
		return m_iUserDamageRatePremium;

	return m_iUserDamageRate;
}

void CHARACTER_MANAGER::SendScriptToMap(long lMapIndex, const std::string & s)
{
	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);

	if (NULL == pSecMap)
		return;

	struct packet_script p;

	p.header = HEADER_GC_SCRIPT;
	p.skin = 1;
	p.src_size = s.size();

	quest::FSendPacket f;
	p.size = p.src_size + sizeof(struct packet_script);
	f.buf.write(&p, sizeof(struct packet_script));
	f.buf.write(&s[0], s.size());

	pSecMap->for_each(f);
}

bool CHARACTER_MANAGER::BeginPendingDestroy()
{
	if (m_bUsePendingDestroy)
		return false;

	m_bUsePendingDestroy = true;
	return true;
}

void CHARACTER_MANAGER::FlushPendingDestroy()
{
	using namespace std;

	m_bUsePendingDestroy = false;

	if (!m_set_pkChrPendingDestroy.empty())
	{
		sys_log(0, "FlushPendingDestroy size %d", m_set_pkChrPendingDestroy.size());

		CHARACTER_SET::iterator it = m_set_pkChrPendingDestroy.begin(),
			end = m_set_pkChrPendingDestroy.end();
		for ( ; it != end; ++it) {
			M2_DESTROY_CHARACTER(*it);
		}

		m_set_pkChrPendingDestroy.clear();
	}
}

CharacterVectorInteractor::CharacterVectorInteractor(const CHARACTER_SET & r)
{
	using namespace std;
#if defined(__GNUC__) && !defined(__clang__) && !defined(CXX11_ENABLED)
	using namespace __gnu_cxx;
#endif

	reserve(r.size());
#if defined(__GNUC__) && !defined(__clang__) && !defined(CXX11_ENABLED)
	transform(r.begin(), r.end(), back_inserter(*this), identity<CHARACTER_SET::value_type>());
#else
	insert(end(), r.begin(), r.end());
#endif

	if (CHARACTER_MANAGER::instance().BeginPendingDestroy())
		m_bMyBegin = true;
}

#ifdef ENABLE_EVENT_MANAGER
#include "item_manager.h"
#include "item.h"
#include "desc_client.h"
#include "desc_manager.h"
void CHARACTER_MANAGER::ClearEventData()
{
	m_eventData.clear();
}
void CHARACTER_MANAGER::CheckBonusEvent(LPCHARACTER ch)
{
	const TEventManagerData* eventPtr = CheckEventIsActive(BONUS_EVENT, ch->GetEmpire());
	if (eventPtr)
		ch->ApplyPoint(eventPtr->value[0], eventPtr->value[1]);
}
const TEventManagerData* CHARACTER_MANAGER::CheckEventIsActive(BYTE eventIndex, BYTE empireIndex)
{
	const time_t cur_Time = time(NULL);
	[[maybe_unused]] const struct tm vKey = *localtime(&cur_Time);

	for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
	{
		[[maybe_unused]] const auto& dayIndex = it->first;
		const auto& dayVector = it->second;
		for (DWORD j = 0; j < dayVector.size(); ++j)
		{
			const auto& eventData = dayVector[j];
			if (eventData.eventIndex == eventIndex)
			{
				if (eventData.channelFlag != 0)
					if (eventData.channelFlag != g_bChannel)
						continue;
				if (eventData.empireFlag != 0 && empireIndex != 0)
					if (eventData.empireFlag != empireIndex)
						continue;

				if (eventData.eventStatus == true)
					return &eventData;
			}
		}
	}
	return NULL;
}
void CHARACTER_MANAGER::CheckEventForDrop(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM>& vec_item)
{
	const BYTE killerEmpire = pkKiller->GetEmpire();
	const TEventManagerData* eventPtr = NULL;
	LPITEM rewardItem = NULL;

	if (pkChr->IsStone())
	{
		eventPtr = CheckEventIsActive(DOUBLE_STONE_EVENT, killerEmpire);
		if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
		{
			const int prob = number(1, 250);
			const int success_prob = eventPtr->value[3];
			if (success_prob >= prob)
			{
				std::vector<LPITEM> m_cache;
				for (DWORD j = 0; j < vec_item.size(); ++j)
				{
					const auto vItem = vec_item[j];
					rewardItem = ITEM_MANAGER::Instance().CreateItem(vItem->GetVnum(), vItem->GetCount(), 0, true);
					if (rewardItem) m_cache.emplace_back(rewardItem);
				}
				for (DWORD j = 0; j < m_cache.size(); ++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}
	else if (pkChr->GetMobRank() >= MOB_RANK_BOSS)
	{
		eventPtr = CheckEventIsActive(DOUBLE_BOSS_EVENT, killerEmpire);
		if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
		{
			const int prob = number(1, 250);
			const int success_prob = eventPtr->value[3];
			if (success_prob >= prob)
			{
				std::vector<LPITEM> m_cache;
				for (DWORD j = 0; j < vec_item.size(); ++j)
				{
					const auto vItem = vec_item[j];
					rewardItem = ITEM_MANAGER::Instance().CreateItem(vItem->GetVnum(), vItem->GetCount(), 0, true);
					if (rewardItem) m_cache.emplace_back(rewardItem);
				}
				for (DWORD j = 0; j < m_cache.size(); ++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}

	if (!pkChr->IsPC())
	{
		eventPtr = CheckEventIsActive(DOUBLE_ITEM_EVENT, killerEmpire);
		if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
		{
			const int prob = number(1, 250);
			const int success_prob = eventPtr->value[3];
			if (success_prob >= prob)
			{
				std::vector<LPITEM> m_cache;
				for (DWORD j = 0; j < vec_item.size(); ++j)
				{
					const auto vItem = vec_item[j];
					rewardItem = ITEM_MANAGER::Instance().CreateItem(vItem->GetVnum(), vItem->GetCount(), 0, true);
					if (rewardItem) m_cache.emplace_back(rewardItem);
				}
				for (DWORD j = 0; j < m_cache.size(); ++j)
					vec_item.emplace_back(m_cache[j]);
			}
		}
	}

	eventPtr = CheckEventIsActive(AY_ISIGI_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50011, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(CORAP_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50010, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(FUTBOL_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50096, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(CIKOLATA_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50025, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}


	eventPtr = CheckEventIsActive(ALTIGEN_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50037, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(ABANOZ_EVENT, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(50109, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(PATRON_CARK, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(61407, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	eventPtr = CheckEventIsActive(BILET_CARK, killerEmpire);
	if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	{
		const int prob = number(1, 250);
		const int success_prob = eventPtr->value[3];
		if (success_prob >= prob)
		{
			// If your moonlight item vnum is different change 50011!
			LPITEM item = ITEM_MANAGER::Instance().CreateItem(61406, 1, 0, true);
			if (item) vec_item.emplace_back(item);
		}
	}

	// eventPtr = CheckEventIsActive(DUNGEON_TICKET_LOOT_EVENT, killerEmpire);
	// if (eventPtr && LEVEL_DELTA(pkChr->GetLevel(), pkKiller->GetLevel(), 200))
	// {
		// const int prob = number(1, 250);
		// const int success_prob = eventPtr->value[3];
		// if (success_prob >= prob)
		// {
			// If you have different book index put here!
			// constexpr DWORD m_lticketItems[] = {990054, 990055, 990056, 990057, 990058, 990059, 990060, 990071, 990072, 990073, 990074, 990075};
			// std::vector<LPITEM> m_cache;
			// for (DWORD j = 0; j < vec_item.size(); ++j)
			// {
				// const auto vItem = vec_item[j];
				// const DWORD itemVnum = vItem->GetVnum();
				// for (DWORD x = 0; x < _countof(m_lticketItems); ++x)
				// {
					// if (m_lticketItems[x] == itemVnum)
					// {
						// rewardItem = ITEM_MANAGER::Instance().CreateItem(itemVnum, vItem->GetCount(), 0, true);
						// if (rewardItem) m_cache.emplace_back(rewardItem);
						// break;
					// }
				// }
				// for (DWORD j = 0; j < m_cache.size(); ++j)
					// vec_item.emplace_back(m_cache[j]);
			// }
		// }
	// }
}
void CHARACTER_MANAGER::CompareEventSendData(TEMP_BUFFER* buf)
{
	const BYTE dayCount = m_eventData.size();
	const BYTE subIndex = EVENT_MANAGER_LOAD;
	const int cur_Time = time(NULL);
	TPacketGCEventManager p;
	p.header = HEADER_GC_EVENT_MANAGER;
	p.size = sizeof(TPacketGCEventManager) + sizeof(BYTE) + sizeof(BYTE) + sizeof(int);
	for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
	{
		[[maybe_unused]] const auto& dayIndex = it->first;
		const auto& dayData = it->second;
		const BYTE dayEventCount = dayData.size();
		p.size += sizeof(BYTE) + sizeof(BYTE) + (dayEventCount * sizeof(TEventManagerData));
	}
	buf->write(&p, sizeof(TPacketGCEventManager));
	buf->write(&subIndex, sizeof(BYTE));
	buf->write(&dayCount, sizeof(BYTE));
	buf->write(&cur_Time, sizeof(int));
	for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
	{
		const auto& dayIndex = it->first;
		const auto& dayData = it->second;
		const BYTE dayEventCount = dayData.size();
		buf->write(&dayIndex, sizeof(BYTE));
		buf->write(&dayEventCount, sizeof(BYTE));
		if (dayEventCount > 0)
			buf->write(dayData.data(), dayEventCount * sizeof(TEventManagerData));
	}
}
void CHARACTER_MANAGER::UpdateAllPlayerEventData()
{
	TEMP_BUFFER buf;
	CompareEventSendData(&buf);
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
	{
		LPCHARACTER ch = (*it)->GetCharacter();
		if (!ch)
			continue;
		(*it)->Packet(buf.read_peek(), buf.size());
	}
}
void CHARACTER_MANAGER::SendDataPlayer(LPCHARACTER ch)
{
	auto desc = ch->GetDesc();
	if (!desc)
		return;
	TEMP_BUFFER buf;
	CompareEventSendData(&buf);
	desc->Packet(buf.read_peek(), buf.size());

/* 	const auto event = CheckEventIsActive(BEGGINER_EVENT, ch->GetEmpire());
	if (event != NULL && ch->GetQuestFlag("event_manager.is_new_player") == 0 && ch->GetLevel() == 0)
	{
		//value0 - itemvnum / value1 - itemcount
		ch->AutoGiveItem(event->value[0], event->value[1]);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("250_LEVEL_OLMALISIN."));
		ch->SetQuestFlag("event_manager.is_new_player", 1);
	}
	if (ch->GetLevel() == 0)
		ch->PointChange(POINT_LEVEL, 1); */
}
bool CHARACTER_MANAGER::CloseEventManuel(BYTE eventIndex)
{
	auto eventPtr = CheckEventIsActive(eventIndex);
	if (eventPtr != NULL)
	{
		const BYTE subHeader = EVENT_MANAGER_REMOVE_EVENT;
		db_clientdesc->DBPacketHeader(HEADER_GD_EVENT_MANAGER, 0, sizeof(BYTE) + sizeof(WORD));
		db_clientdesc->Packet(&subHeader, sizeof(BYTE));
		db_clientdesc->Packet(&eventPtr->eventID, sizeof(WORD));
		return true;
	}
	return false;
}
void CHARACTER_MANAGER::SetEventStatus(const WORD eventID, const bool eventStatus, const int endTime, const char* endTimeText)
{
	//eventStatus - 0-deactive  // 1-active

	TEventManagerData* eventData = NULL;
	for (auto it = m_eventData.begin(); it != m_eventData.end(); ++it)
	{
		if (it->second.size())
		{
			for (DWORD j = 0; j < it->second.size(); ++j)
			{
				TEventManagerData& pData = it->second[j];
				if (pData.eventID == eventID)
				{
					eventData = &pData;
					break;
				}
			}
		}
	}
	if (eventData == NULL)
		return;
	eventData->eventStatus = eventStatus;
	eventData->endTime = endTime;
	strlcpy(eventData->endTimeText, endTimeText, sizeof(eventData->endTimeText));

	// Auto open&close notice
	const std::map<BYTE, std::pair<std::string, std::string>> m_eventText = {
		{BONUS_EVENT,{"Bonus Etkinliği Başladı !","Bonus Etkinliği Sona Erdi !"}},
		{DOUBLE_BOSS_EVENT,{"2x Patron Etkinliği Başladı !","2x Patron Etkinliği Sona Erdi !"}},
		{DOUBLE_STONE_EVENT,{"2x Metin Taşı Etkinliği Başladı !","2x Metin Taşı Etkinliği Sona Erdi !"}},
		{DOUBLE_ITEM_EVENT,{"2x Item Etkinliği Başladı !","2x Item Etkinliği Sona Erdi !"}},
		{EXP_EVENT,{"2x Exp Etkinliği Başladı !","2x Exp Etkinliği Sona Erdi !"}},
		{YANG_EVENT,{"2x Yang Etkinliği Başladı !","2x Yang Etkinliği Sona Erdi !"}},
		{YUTNORI_EVENT,{"Yutnori Etkinliği Başladı !","Yutnori Etkinliği Sona Erdi !"}},
		{AY_ISIGI_EVENT,{"Ay Işığı Etkinliği Başladı !","Ay Işığı Etkinliği Sona Erdi !"}},
		{CORAP_EVENT,{"Çorap Etkinliği Başladı !","Çorap Etkinliği Sona Erdi !"}},
		{CIKOLATA_EVENT,{"Cikolata Etkinliği Başladı !","Cikolata Etkinliği Sona Erdi !"}},
		{ALTIGEN_EVENT,{"Altigen Hediye Paketi Etkinliği Başladı !","Altigen Hediye Paketi Etkinliği Sona Erdi !"}},
		{ABANOZ_EVENT,{"Abanoz Kutu Etkinliği Başladı !","Abanoz Kutu Etkinliği Sona Erdi !"}},
		{PATRON_CARK,{"Patron Cark Etkinliği Başladı !","Patron Cark Etkinliği Sona Erdi !"}},
		{BILET_CARK,{"Bilet Cark Etkinliği Başladı !","Bilet Cark Etkinliği Sona Erdi !"}},
		{FUTBOL_EVENT,{"Futbol Etkinligi Basladi !","Futbol Etkinligi Sona Erdi !"}},
#ifdef ENABLE_EMEK_POINT
		{DOUBLE_EMEK_EVENT,{"2x Emek Puan Etkinliği Başladı !","2x Emek Puan Etkinliği Sona Erdi !"}},
#endif
	};

	const auto it = m_eventText.find(eventData->eventIndex);
	if (it != m_eventText.end())
	{
		if (eventStatus)
			SendNotice(it->second.first.c_str());
		else
			SendNotice(it->second.second.c_str());
	}

	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();

	// Bonus event update status
	if (eventData->eventIndex == BONUS_EVENT)
	{
		for (auto itDesc = c_ref_set.begin(); itDesc != c_ref_set.end(); ++itDesc)
		{
			LPCHARACTER ch = (*itDesc)->GetCharacter();
			if (!ch)
				continue;
			if (eventData->empireFlag != 0)
				if (eventData->empireFlag != ch->GetEmpire())
					continue;
			if (eventData->channelFlag != 0)
				if (eventData->channelFlag != g_bChannel)
					return;
			if (!eventStatus)
			{
				const long value = eventData->value[1];
				ch->ApplyPoint(eventData->value[0], -value);
			}
			ch->ComputePoints();
		}
	}

	// const int now = time(0);
	// const BYTE subIndex = EVENT_MANAGER_EVENT_STATUS;

	// TPacketGCEventManager p;
	// p.header = HEADER_GC_EVENT_MANAGER;
	// p.size = sizeof(TPacketGCEventManager) + sizeof(BYTE) + sizeof(WORD) + sizeof(bool) + sizeof(int) + sizeof(int) + sizeof(eventData->endTimeText);

	// TEMP_BUFFER buf;
	// buf.write(&p, sizeof(TPacketGCEventManager));
	// buf.write(&subIndex, sizeof(BYTE));
	// buf.write(&eventData->eventID, sizeof(WORD));
	// buf.write(&eventData->eventStatus, sizeof(bool));
	// buf.write(&eventData->endTime, sizeof(int));
	// buf.write(&eventData->endTimeText, sizeof(eventData->endTimeText));
	// buf.write(&now, sizeof(int));

	// for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
	// {
		// LPCHARACTER ch = (*it)->GetCharacter();
		// if (!ch)
			// continue;
		// (*it)->Packet(buf.read_peek(), buf.size());
	// }
	
	for (auto it = c_ref_set.begin(); it != c_ref_set.end(); ++it)
	{
		LPCHARACTER ch = (*it)->GetCharacter();
		if (!ch)
			continue;

		SendDataPlayer(ch);
	}
}
void CHARACTER_MANAGER::SetEventData(BYTE dayIndex, const std::vector<TEventManagerData>& m_data)
{
	const auto it = m_eventData.find(dayIndex);
	if (it == m_eventData.end())
		m_eventData.emplace(dayIndex, m_data);
	else
	{
		it->second.clear();
		for (DWORD j = 0; j < m_data.size(); ++j)
			it->second.emplace_back(m_data[j]);
	}
}
#endif

CharacterVectorInteractor::~CharacterVectorInteractor()
{
	if (m_bMyBegin)
		CHARACTER_MANAGER::instance().FlushPendingDestroy();
}

#ifdef __BACK_DUNGEON__
bool CHARACTER_MANAGER::CheckBackDungeon(LPCHARACTER ch, BYTE checkType, DWORD mapIdx)
{
	if (!ch && checkType != BACK_DUNGEON_REMOVE)
		return false;

	// std::map<DWORD, std::map<std::pair<DWORD, WORD>, std::vector<DWORD>>> m_mapbackDungeon;

	if (checkType == BACK_DUNGEON_SAVE)
	{
		const DWORD dungeonIdx = ch->GetMapIndex();
		if (dungeonIdx < 10000)
			return false;
		LPDUNGEON dungeon = CDungeonManager::Instance().FindByMapIndex(dungeonIdx);
		if (!dungeon)
			return false;
		if (dungeon->GetFlag("can_back_dungeon") != 1)
			return false;
		const DWORD mapIdx = dungeonIdx / 10000;
		auto it = m_mapbackDungeon.find(mapIdx);
		if (it == m_mapbackDungeon.end())
		{
			std::map<std::pair<DWORD, WORD>, std::vector<DWORD>> m_mapList;
			m_mapbackDungeon.emplace(mapIdx, m_mapList);
			it = m_mapbackDungeon.find(mapIdx);
		}
		auto itDungeon = it->second.find(std::make_pair(dungeonIdx, mother_port));
		if (itDungeon == it->second.end())
		{
			std::vector<DWORD> m_pidList;
			it->second.emplace(std::make_pair(dungeonIdx, mother_port), m_pidList);
			itDungeon = it->second.find(std::make_pair(dungeonIdx, mother_port));
		}
		if(std::find(itDungeon->second.begin(), itDungeon->second.end(),ch->GetPlayerID()) == itDungeon->second.end())
			itDungeon->second.emplace_back(ch->GetPlayerID());
	}
	else if (checkType == BACK_DUNGEON_REMOVE)
	{
		if (mapIdx < 10000)
			return false;

		auto it = m_mapbackDungeon.find(mapIdx / 10000);
		if (it != m_mapbackDungeon.end())
		{
			for (auto& [dungeonData, playerList] : it->second)
			{
				if (mapIdx == dungeonData.first)
				{
					it->second.erase(dungeonData);
					return true;
				}
			}
		}
	}
	else if (checkType == BACK_DUNGEON_CHECK || checkType == BACK_DUNGEON_WARP)
	{
		const DWORD curretMapIdx = ch->GetMapIndex();
		if (curretMapIdx > 10000)
			return false;

		const std::map<DWORD, std::vector<DWORD>> m_baseToDungeonIdx = {
			{
				65, // baseMapidx
				{
					66,//devil-tower
				},
			}
		};

		const auto itBase = m_baseToDungeonIdx.find(curretMapIdx);
		if (itBase != m_baseToDungeonIdx.end())
		{
			for (const auto& dungeonIdx : itBase->second)
			{
				auto it = m_mapbackDungeon.find(dungeonIdx);
				if (it != m_mapbackDungeon.end())
				{
					for (auto& [dungeonData, playerList] : it->second)
					{
						LPDUNGEON dungeon = CDungeonManager::Instance().FindByMapIndex(dungeonData.first);
						if (std::find(playerList.begin(), playerList.end(), ch->GetPlayerID()) == playerList.end())
							continue;
						else if (!dungeon)
							continue;
						else if (dungeon->GetFlag("can_back_dungeon") != 1)
							continue;
						if (checkType == BACK_DUNGEON_WARP)
						{
							if (mapIdx == dungeonData.first)
							{
								ch->WarpSet(dungeon->GetFlag("LAST_X"), dungeon->GetFlag("LAST_Y"), dungeonData.first);
								return true;
							}
						}
						else
							ch->ChatPacket(CHAT_TYPE_COMMAND, "ReJoinDungeonDialog %d", dungeonData.first);
					}
				}
			}
		}
	}
	return false;
}
#endif
