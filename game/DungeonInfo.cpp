#include "stdafx.h"

#ifdef ENABLE_DUNGEON_INFO_SYSTEM
#include "DungeonInfo.h"

static std::vector<SDungeonData*> s_vecDungeonProto;

SDungeonData::SDungeonData() {}

CDungeonInfoManager::CDungeonInfoManager() = default;
CDungeonInfoManager::~CDungeonInfoManager() = default;

void CDungeonInfoManager::Destroy() {}

void CDungeonInfoManager::Reload()
{
	Initialize();

	const DESC_MANAGER::DESC_SET& c_RefSet = DESC_MANAGER::Instance().GetClientSet();

	for (itertype(c_RefSet) iter = c_RefSet.begin(); iter != c_RefSet.end(); ++iter)
	{
		LPCHARACTER pkCh = (*iter)->GetCharacter();
		if (!pkCh)
			continue;

		if (pkCh->IsDungeonInfoOpen())
			SendInfo(pkCh, TRUE);
	}
}

void CDungeonInfoManager::Initialize()
{
	SDungeonData* pSDungeonData = nullptr;
	std::vector<SDungeonData*>::iterator iter;

	char szFileName[255 + 1];
	snprintf(szFileName, sizeof(szFileName), "%s/dungeon_info.txt", LocaleService_GetBasePath().c_str());

	sys_log(0, "DungeonInfoInit %s", szFileName);

	for (iter = s_vecDungeonProto.begin(); iter != s_vecDungeonProto.end(); iter++)
	{
		pSDungeonData = *iter;
		M2_DELETE(pSDungeonData);
	}

	s_vecDungeonProto.clear();

	if (!LoadFile(szFileName))
		sys_err("DungeonInfoLoad failed");
}

bool CDungeonInfoManager::LoadFile(const char* c_szFileName)
{
	FILE* pFile;

	char szOneLine[512 + 1];
	int iValue1, iValue2, iValue3;
	char szValue1[QUEST_NAME_MAX_LEN], szValue2[QUEST_NAME_MAX_LEN], szValue3[QUEST_NAME_MAX_LEN];

	const char* c_szDelim = " \t\r\n";
	char* szValue, * szTokenString;

	SDungeonData* pSDungeonData = nullptr;

	SDungeonEntryPosition sDungeonEntryPosition = { 0, 0 };
	SDungeonLimits sDungeonLimit = { 0, 0 };
	SDungeonItem sDungeonItem = { 0, 0 };
	SDungeonBonus sDungeonBonus = { 0, 0 };

	SDungeonQuest sDungeonQuest = { "", "", 0 };

	if (0 == c_szFileName || 0 == c_szFileName[0])
		return false;

	if ((pFile = fopen(c_szFileName, "r")) == 0)
		return false;

	while (fgets(szOneLine, 512 + 1, pFile))
	{
		iValue1 = iValue2 = iValue3 = 0;

		if (szOneLine[0] == '#')
			continue;

		szTokenString = strtok(szOneLine, c_szDelim);

		if (nullptr == szTokenString)
			continue;

		if ((szValue = strtok(nullptr, c_szDelim)))
		{
			str_to_number(iValue1, szValue);
			strcpy(szValue1, szValue);
		}

		if ((szValue = strtok(nullptr, c_szDelim)))
		{
			str_to_number(iValue2, szValue);
			strcpy(szValue2, szValue);
		}

		if ((szValue = strtok(nullptr, c_szDelim)))
		{
			str_to_number(iValue3, szValue);
			strcpy(szValue3, szValue);
		}

#define OR else

		DUNGEON_TOKEN("dungeon")
		{
			pSDungeonData = M2_NEW SDungeonData;
		}
		OR DUNGEON_TOKEN("TYPE")
		{
			pSDungeonData->byType = iValue1;
		}
		OR DUNGEON_TOKEN("MAP_INDEX")
		{
			pSDungeonData->lMapIndex = iValue1;
		}
		OR DUNGEON_TOKEN("ENTRY_MAP_INDEX")
		{
			pSDungeonData->lEntryMapIndex = iValue1;
		}
		OR DUNGEON_TOKEN("ENTRY_BASE_POSITION")
		{
			sDungeonEntryPosition.lBaseX = iValue1;
			sDungeonEntryPosition.lBaseY = iValue2;

			pSDungeonData->vecEntryPosition.emplace_back(sDungeonEntryPosition);
		}
		OR DUNGEON_TOKEN("BOSS_VNUM")
		{
			pSDungeonData->dwBossVnum = iValue1;
		}
		OR DUNGEON_TOKEN("LEVEL_LIMIT")
		{
			sDungeonLimit.iMin = iValue1;
			sDungeonLimit.iMax = iValue2;

			pSDungeonData->vecLevelLimit.emplace_back(sDungeonLimit);
		}
		OR DUNGEON_TOKEN("MEMBER_LIMIT")
		{
			sDungeonLimit.iMin = iValue1;
			sDungeonLimit.iMax = iValue2;

			pSDungeonData->vecMemberLimit.emplace_back(sDungeonLimit);
		}
		OR DUNGEON_TOKEN("REQUIRED_ITEM")
		{
			sDungeonItem.dwVnum = iValue1;
			sDungeonItem.wCount = iValue2;

			pSDungeonData->vecRequiredItem.emplace_back(sDungeonItem);
		}
		OR DUNGEON_TOKEN("DURATION")
		{
			pSDungeonData->dwDuration = iValue1;
		}
		OR DUNGEON_TOKEN("COOLDOWN")
		{
			pSDungeonData->dwCooldown = iValue1;
		}
		OR DUNGEON_TOKEN("ELEMENT")
		{
			pSDungeonData->byElement = iValue1;
		}
		OR DUNGEON_TOKEN("BONUS_ATT")
		{
			sDungeonBonus.byAttBonus = iValue1;
			sDungeonBonus.byDefBonus = 0;

			pSDungeonData->vecBonus.emplace_back(sDungeonBonus);
		}
		OR DUNGEON_TOKEN("BONUS_DEF")
		{
			sDungeonBonus.byAttBonus = 0;
			sDungeonBonus.byDefBonus = iValue1;

			pSDungeonData->vecBonus.emplace_back(sDungeonBonus);
		}
		OR DUNGEON_TOKEN("BOSS_DROP_ITEM")
		{
			sDungeonItem.dwVnum = iValue1;
			sDungeonItem.wCount = iValue2;

			pSDungeonData->vecBossDropItem.emplace_back(sDungeonItem);
		}
		OR DUNGEON_TOKEN("QUEST")
		{
			sDungeonQuest.strQuest = szValue1;
			sDungeonQuest.strQuestFlag = szValue2;
			if (strcmp(szValue3, "GLOBAL") == 0)
				sDungeonQuest.byType = QUEST_FLAG_GLOBAL;
			else
				sDungeonQuest.byType = QUEST_FLAG_PC;

			pSDungeonData->vecQuest.emplace_back(sDungeonQuest);
		}
		OR DUNGEON_TOKEN("end")
		{
			s_vecDungeonProto.emplace_back(pSDungeonData);
		}
	}

	fclose(pFile);
	return true;
}

void CDungeonInfoManager::Warp(LPCHARACTER pkCh, uint16_t byIndex)
{
	if (!pkCh)
		return;

	LPDESC pkDesc = pkCh->GetDesc();
	if (!pkDesc)
		return;

	{
		SDungeonData* pSDungeonData = s_vecDungeonProto[byIndex];
		if (pSDungeonData)
		{
			for (size_t iLimit = 0; iLimit < pSDungeonData->vecLevelLimit.size(); ++iLimit)
			{
				if (pkCh->GetLevel() < pSDungeonData->vecLevelLimit[iLimit].iMin)
					return;

				if (pkCh->GetLevel() > pSDungeonData->vecLevelLimit[iLimit].iMax)
					return;
			}

			for (size_t iPos = 0; iPos < pSDungeonData->vecLevelLimit.size(); ++iPos)
			{
				pkCh->WarpSet(
					pSDungeonData->vecEntryPosition[iPos].lBaseX * 100,
					pSDungeonData->vecEntryPosition[iPos].lBaseY * 100
				);
				return;
			}
		}
	}
}

void CDungeonInfoManager::Ranking(LPCHARACTER pkCh, uint16_t byIndex, uint8_t byRankType)
{
	if (!pkCh || !pkCh->GetDesc())
		return;

	SDungeonData* pSDungeonData = s_vecDungeonProto[byIndex];
	if (pSDungeonData)
	{
		const char* c_szFilter = "completed";
		const char* c_szSort = "ASC";
		switch (byRankType)
		{
			case ERankTypes::RANKING_TYPE_COMPLETED:
				c_szFilter = "finish";
				c_szSort = "DESC"; 
				break;

			case ERankTypes::RANKING_TYPE_TIME:
				c_szFilter = "finish_time";
				c_szSort = "ASC";
				break;

			case ERankTypes::RANKING_TYPE_DAMAGE:
				c_szFilter = "finish_damage";
				c_szSort = "DESC";
				break;

			default:
				c_szFilter = "completed";
				break;
		}

		auto pMsg(DBManager::Instance().DirectQuery("SELECT `player`.`name`, `player`.`level`, `%s` FROM `player`.`dungeon_ranking`"
			"LEFT JOIN `player`.`player` ON `pid` = `player`.`id` "
			"INNER JOIN `account`.`account` ON `account`.`id` = `player`.`account_id` "
			"WHERE `account`.`status` != 'BLOCK' AND `dungeon_index` = %d ORDER BY `%s` %s LIMIT %d "
			, c_szFilter, pSDungeonData->lMapIndex, c_szFilter, c_szSort, ERank::MAX_RANKING_COUNT)
		);

		if (pMsg->uiSQLErrno)
			return;

		TEMP_BUFFER buf;
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			if (!strncmp(row[0], "[", 1)) // GM
				continue;

			//const TPacketGCDungeonInfoRank packet = TPacketGCDungeonInfoRank(row[0], std::stoi(row[1]), std::stoi(row[2]));
			TPacketGCDungeonInfoRank packet;
			strlcpy(packet.szName, row[0], sizeof(packet.szName));
			str_to_number(packet.iLevel, row[1]);
			str_to_number(packet.dwPoints, row[2]);
			pkCh->GetDesc()->Packet(&packet, sizeof(packet));
		}

		//const TPacketGCDungeonInfoRank packet = TPacketGCDungeonInfoRank();
		TPacketGCDungeonInfoRank packet;
		strlcpy(packet.szName, "", sizeof(packet.szName));
		str_to_number(packet.iLevel, 0);
		str_to_number(packet.dwPoints, 0);
		pkCh->GetDesc()->Packet(&packet, sizeof(packet));
	}

	return;
}

void CDungeonInfoManager::SendInfo(LPCHARACTER pkCh, BOOL bReload)
{
	if (!pkCh)
		return;

	LPDESC pkDesc = pkCh->GetDesc();
	if (!pkDesc)
		return;

	for (size_t iIndex = 0; iIndex < s_vecDungeonProto.size(); ++iIndex)
	{
		SDungeonData* pSDungeonData = s_vecDungeonProto[iIndex];

		TPacketGCDungeonInfo packet;
		packet.byHeader = HEADER_GC_DUNGEON_INFO;
		packet.bySubHeader = SUBHEADER_DUNGEON_INFO_SEND;
		packet.byIndex = iIndex;
		packet.bReset = bReload;
		packet.byType = pSDungeonData->byType;
		packet.lMapIndex = pSDungeonData->lMapIndex;
		packet.lEntryMapIndex = pSDungeonData->lEntryMapIndex;
		packet.dwBossVnum = pSDungeonData->dwBossVnum;

		// Level limits
		for (size_t iLimit = 0; iLimit < pSDungeonData->vecLevelLimit.size(); ++iLimit)
		{
			packet.sLevelLimit.iLevelMin = pSDungeonData->vecLevelLimit[iLimit].iMin;
			packet.sLevelLimit.iLevelMax = pSDungeonData->vecLevelLimit[iLimit].iMax;
		}

		// Member level limits
		for (size_t iLimit = 0; iLimit < pSDungeonData->vecMemberLimit.size(); ++iLimit)
		{
			packet.sLevelLimit.iMemberMin = pSDungeonData->vecMemberLimit[iLimit].iMin;
			packet.sLevelLimit.iMemberMax = pSDungeonData->vecMemberLimit[iLimit].iMax;
		}

		// Required items
		for (size_t iSlot = 0; iSlot < pSDungeonData->vecRequiredItem.size(); ++iSlot)
		{
			packet.sRequiredItem[iSlot].dwVnum = pSDungeonData->vecRequiredItem[iSlot].dwVnum;
			packet.sRequiredItem[iSlot].wCount = pSDungeonData->vecRequiredItem[iSlot].wCount;
		}

		// Duration
		packet.dwDuration = pSDungeonData->dwDuration;

		// Cooldown / Wait time
		uint32_t dwCooldown = 0;
		for (size_t iQFlag = 0; iQFlag < pSDungeonData->vecQuest.size(); ++iQFlag)
		{
			time_t dwFlagValue = 0;
			switch (pSDungeonData->vecQuest[iQFlag].byType)
			{
				case QUEST_FLAG_PC:
				{
					char szQuestFlag[QUEST_NAME_MAX_LEN * 2 + 1];
					sprintf(szQuestFlag, "%s.%s",
						pSDungeonData->vecQuest[iQFlag].strQuest.c_str(),
						pSDungeonData->vecQuest[iQFlag].strQuestFlag.c_str());
					dwFlagValue = pkCh->GetQuestFlag(szQuestFlag);
				}
				break;

				case QUEST_FLAG_GLOBAL:
					dwFlagValue = quest::CQuestManager::Instance().GetEventFlag(pSDungeonData->vecQuest[iQFlag].strQuestFlag);
					break;
			}

			if (dwFlagValue > get_global_time())
			{
				dwCooldown = dwFlagValue - get_global_time();
			}
			else
			{
				uint32_t dwRemainSec = (dwFlagValue + pSDungeonData->dwCooldown) - get_global_time();
				if (dwRemainSec > 0)
				{
					dwCooldown = dwRemainSec;
				}
				else
					dwCooldown = 0;
			}

			packet.dwCooldown = dwCooldown > 0 ? dwCooldown : 0;
		}

		// Element
		packet.byElement = pSDungeonData->byElement;

		// Bonus
		for (size_t iAffect = 0; iAffect < pSDungeonData->vecBonus.size(); ++iAffect)
		{
			if (iAffect > POINT_MAX_NUM)
				break;

			packet.sBonus.byAttBonus[iAffect] = pSDungeonData->vecBonus[iAffect].byAttBonus;
			packet.sBonus.byDefBonus[iAffect] = pSDungeonData->vecBonus[iAffect].byDefBonus;
		}

		// Boss drops
		for (size_t iSlot = 0; iSlot < pSDungeonData->vecBossDropItem.size(); ++iSlot)
		{
			packet.sBossDropItem[iSlot].dwVnum = pSDungeonData->vecBossDropItem[iSlot].dwVnum;
			packet.sBossDropItem[iSlot].wCount = pSDungeonData->vecBossDropItem[iSlot].wCount;
		}

		// Results
		for (size_t iQFlag = 0; iQFlag < pSDungeonData->vecQuest.size(); ++iQFlag)
		{
			std::string strQuest = pSDungeonData->vecQuest[iQFlag].strQuest;
			packet.sResults.dwFinish = GetResult(pkCh, strQuest, "finish");
			packet.sResults.dwFinishTime = GetResult(pkCh, strQuest, "finish_time");
			packet.sResults.dwFinishDamage = GetResult(pkCh, strQuest, "finish_damage");
		}

		pkDesc->Packet(&packet, sizeof(packet));
	}

	if (!pkCh->IsDungeonInfoOpen())
	{
		pkCh->SetDungeonInfoOpen(true);
		pkCh->StartDungeonInfoReloadEvent();

		TPacketGCDungeonInfo packet;
		packet.bySubHeader = SUBHEADER_DUNGEON_INFO_OPEN;
		pkDesc->Packet(&packet, sizeof(packet));
	}
}

uint32_t CDungeonInfoManager::GetResult(LPCHARACTER pkCh, std::string strQuest, std::string strFlag)
{
	if (!pkCh)
		return 0;

	char szQuestFlag[QUEST_NAME_MAX_LEN * 2 + 1];
	sprintf(szQuestFlag, "%s.%s", strQuest.c_str(), strFlag.c_str());

	return pkCh->GetQuestFlag(szQuestFlag);
}
#endif
