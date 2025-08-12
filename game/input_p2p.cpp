#include "stdafx.h"
#include "../../common/billing.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "party.h"
#include "messenger_manager.h"
#include "empire_text_convert.h"
#include "unique_item.h"
#include "xmas_event.h"
#include "affect.h"
#include "castle.h"
#include "locale_service.h"
#include "questmanager.h"
#include "skill.h"
#include "threeway_war.h"
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
#include "buffer_manager.h"
#endif

#ifdef ENABLE_INGAME_ITEMSHOP
#include "ItemShopNewManager.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Input Processor
CInputP2P::CInputP2P()
{
	BindPacketInfo(&m_packetInfoGG);
}

void CInputP2P::Login(LPDESC d, const char * c_pData)
{
	P2P_MANAGER::instance().Login(d, (TPacketGGLogin *) c_pData);
}

void CInputP2P::Logout(LPDESC d, const char * c_pData)
{
	TPacketGGLogout * p = (TPacketGGLogout *) c_pData;
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	P2P_MANAGER::instance().Logout(p->szName, p->bAFisWarping);
#else
	P2P_MANAGER::instance().Logout(p->szName);
#endif
}

int CInputP2P::Relay(LPDESC d, const char * c_pData, size_t uiBytes)
{
	TPacketGGRelay * p = (TPacketGGRelay *) c_pData;

	if (uiBytes < sizeof(TPacketGGRelay) + p->lSize)
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	sys_log(0, "InputP2P::Relay : %s size %d", p->szName, p->lSize);

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(p->szName);

	const BYTE* c_pbData = (const BYTE *) (c_pData + sizeof(TPacketGGRelay));

	if (!pkChr)
		return p->lSize;

	if (*c_pbData == HEADER_GC_WHISPER)
	{
		if (pkChr->IsBlockMode(BLOCK_WHISPER))
		{
			return p->lSize;
		}

		char buf[1024];
		memcpy(buf, c_pbData, MIN(p->lSize, sizeof(buf)));

		TPacketGCWhisper* p2 = (TPacketGCWhisper*) buf;

		BYTE bToEmpire = (p2->bType >> 4);
		p2->bType = p2->bType & 0x0F;
		if(p2->bType == 0x0F) {
			p2->bType = WHISPER_TYPE_SYSTEM;
		} else {
			if (!pkChr->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
				if (bToEmpire >= 1 && bToEmpire <= 3 && pkChr->GetEmpire() != bToEmpire)
				{
					ConvertEmpireText(bToEmpire,
							buf + sizeof(TPacketGCWhisper),
							p2->wSize - sizeof(TPacketGCWhisper),
							10+2*pkChr->GetSkillPower(SKILL_LANGUAGE1 + bToEmpire - 1));
				}
		}

		pkChr->GetDesc()->Packet(buf, p->lSize);
	}
	else
		pkChr->GetDesc()->Packet(c_pbData, p->lSize);

	return (p->lSize);
}

#ifdef ENABLE_FULL_NOTICE
int CInputP2P::Notice(LPDESC d, const char * c_pData, size_t uiBytes, bool bBigFont)
#else
int CInputP2P::Notice(LPDESC d, const char * c_pData, size_t uiBytes)
#endif
{
	TPacketGGNotice * p = (TPacketGGNotice *) c_pData;

	if (uiBytes < sizeof(TPacketGGNotice) + p->lSize)
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char szBuf[256+1];
	strlcpy(szBuf, c_pData + sizeof(TPacketGGNotice), MIN(p->lSize + 1, sizeof(szBuf)));
#ifdef ENABLE_FULL_NOTICE
	SendNotice(szBuf, bBigFont);
#else
	SendNotice(szBuf);
#endif
	return (p->lSize);
}

int CInputP2P::MonarchNotice(LPDESC d, const char * c_pData, size_t uiBytes)
{
	TPacketGGMonarchNotice * p = (TPacketGGMonarchNotice *) c_pData;

	if (uiBytes < p->lSize + sizeof(TPacketGGMonarchNotice))
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char szBuf[256+1];
	strlcpy(szBuf, c_pData + sizeof(TPacketGGMonarchNotice), MIN(p->lSize + 1, sizeof(szBuf)));
	SendMonarchNotice(p->bEmpire, szBuf);
	return (p->lSize);
}

int CInputP2P::MonarchTransfer(LPDESC d, const char* c_pData)
{
	TPacketMonarchGGTransfer* p = (TPacketMonarchGGTransfer*) c_pData;
	LPCHARACTER pTargetChar = CHARACTER_MANAGER::instance().FindByPID(p->dwTargetPID);

	if (pTargetChar != NULL)
	{
		unsigned int qIndex = quest::CQuestManager::instance().GetQuestIndexByName("monarch_transfer");

		if (qIndex != 0)
		{
			pTargetChar->SetQuestFlag("monarch_transfer.x", p->x);
			pTargetChar->SetQuestFlag("monarch_transfer.y", p->y);
			quest::CQuestManager::instance().Letter(pTargetChar->GetPlayerID(), qIndex, 0);
		}
	}

	return 0;
}

int CInputP2P::Guild(LPDESC d, const char* c_pData, size_t uiBytes)
{
	TPacketGGGuild * p = (TPacketGGGuild *) c_pData;
	uiBytes -= sizeof(TPacketGGGuild);
	c_pData += sizeof(TPacketGGGuild);

	CGuild * g = CGuildManager::instance().FindGuild(p->dwGuild);

	switch (p->bSubHeader)
	{
		case GUILD_SUBHEADER_GG_CHAT:
			{
				if (uiBytes < sizeof(TPacketGGGuildChat))
					return -1;

				TPacketGGGuildChat * p = (TPacketGGGuildChat *) c_pData;

				if (g)
					g->P2PChat(p->szText);

				return sizeof(TPacketGGGuildChat);
			}

		case GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS:
			{
				if (uiBytes < sizeof(int))
					return -1;

				int iBonus = *((int *) c_pData);
				CGuild* pGuild = CGuildManager::instance().FindGuild(p->dwGuild);
				if (pGuild)
				{
					pGuild->SetMemberCountBonus(iBonus);
				}
				return sizeof(int);
			}
		default:
			sys_err ("UNKNOWN GUILD SUB PACKET");
			break;
	}
	return 0;
}

struct FuncShout
{
	const char * m_str;
	BYTE m_bEmpire;
	const char * m_szName;

#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	const char* m_szCountry{};
	int m_hyperlinks{};
	FuncShout(const char * str, BYTE bEmpire, const char* szCountry, int hyperlinks) : m_str(str), m_bEmpire(bEmpire), m_szCountry(szCountry), m_hyperlinks(hyperlinks)
#else
	FuncShout(const char* str, BYTE bEmpire, DWORD dwPID) : m_str(str), m_bEmpire(bEmpire)
#endif
	{
	}

	void operator () (LPDESC d)
	{
#ifdef ENABLE_NEWSTUFF
		if (!d->GetCharacter() || (!g_bGlobalShoutEnable && d->GetCharacter()->GetGMLevel() == GM_PLAYER && d->GetEmpire() != m_bEmpire))
			return;
#else
		if (!d->GetCharacter() || (d->GetCharacter()->GetGMLevel() == GM_PLAYER && d->GetEmpire() != m_bEmpire))
			return;
#endif

#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
		struct packet_chat pack_chat;
		pack_chat.header = HEADER_GC_CHAT;
		pack_chat.size = sizeof(struct packet_chat) + strlen(m_str);
		pack_chat.type = CHAT_TYPE_SHOUT;
		pack_chat.id = 0;
		pack_chat.bEmpire = m_bEmpire;
		pack_chat.bCanFormat = m_hyperlinks > 0;
		strlcpy(pack_chat.szCountry, m_szCountry, sizeof(pack_chat.szCountry));

		TEMP_BUFFER buf;
		buf.write(&pack_chat, sizeof(struct packet_chat));
		buf.write(m_str, strlen(m_str));

		d->Packet(buf.read_peek(), buf.size());

		if (test_server)
			sys_log(0, "SEND_COMMAND %s %s", d->GetCharacter()->GetName(), m_str);
#else
		d->GetCharacter()->ChatPacket(CHAT_TYPE_SHOUT, "%s", m_str);
#endif
	}
};

#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
void SendShout(const char* szText, BYTE bEmpire, const char* szCountry, int hyperlinks)
#else
void SendShout(const char* szText, BYTE bEmpire)
#endif
{
	const DESC_MANAGER::DESC_SET & c_ref_set = DESC_MANAGER::instance().GetClientSet();
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	std::for_each(c_ref_set.begin(), c_ref_set.end(), FuncShout(szText, bEmpire, szCountry, hyperlinks));
#else
	std::for_each(c_ref_set.begin(), c_ref_set.end(), FuncShout(szText, bEmpire));
#endif
}

void CInputP2P::Shout(const char * c_pData)
{
	TPacketGGShout * p = (TPacketGGShout *) c_pData;
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	SendShout(p->szText, p->bEmpire, p->szCountry, p->hyperlinks);
#else
	SendShout(p->szText, p->bEmpire);
#endif
}

void CInputP2P::Disconnect(const char * c_pData)
{
	TPacketGGDisconnect * p = (TPacketGGDisconnect *) c_pData;

	LPDESC d = DESC_MANAGER::instance().FindByLoginName(p->szLogin);

	if (!d)
		return;

	if (!d->GetCharacter())
	{
		d->SetPhase(PHASE_CLOSE);
	}
	else
		d->DisconnectOfSameLogin();
}

void CInputP2P::Setup(LPDESC d, const char * c_pData)
{
	TPacketGGSetup * p = (TPacketGGSetup *) c_pData;
	sys_log(0, "P2P: Setup %s:%d", d->GetHostName(), p->wPort);
	d->SetP2P(d->GetHostName(), p->wPort, p->bChannel);
}

void CInputP2P::MessengerAdd(const char * c_pData)
{
	TPacketGGMessenger * p = (TPacketGGMessenger *) c_pData;
	sys_log(0, "P2P: Messenger Add %s %s", p->szAccount, p->szCompanion);
	MessengerManager::instance().__AddToList(p->szAccount, p->szCompanion);
}

void CInputP2P::MessengerRemove(const char * c_pData)
{
	TPacketGGMessenger * p = (TPacketGGMessenger *) c_pData;
	sys_log(0, "P2P: Messenger Remove %s %s", p->szAccount, p->szCompanion);
	MessengerManager::instance().__RemoveFromList(p->szAccount, p->szCompanion);
}

void CInputP2P::FindPosition(LPDESC d, const char* c_pData)
{
	TPacketGGFindPosition* p = (TPacketGGFindPosition*) c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->dwTargetPID);
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	if (ch)
#else
	if (ch && ch->GetMapIndex() < 10000)
#endif
	{
		TPacketGGWarpCharacter pw;
		pw.header = HEADER_GG_WARP_CHARACTER;
		pw.pid = p->dwFromPID;
		pw.x = ch->GetX();
		pw.y = ch->GetY();
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
		pw.mapIndex = (ch->GetMapIndex() < 10000) ? 0 : ch->GetMapIndex();
#endif
		d->Packet(&pw, sizeof(pw));
	}
}

void CInputP2P::WarpCharacter(const char* c_pData)
{
	TPacketGGWarpCharacter* p = (TPacketGGWarpCharacter*) c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->pid);
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	if (ch)
		ch->WarpSet(p->x, p->y, p->mapIndex);
#else
	if (ch)
		ch->WarpSet(p->x, p->y);
#endif
}

void CInputP2P::GuildWarZoneMapIndex(const char* c_pData)
{
	TPacketGGGuildWarMapIndex * p = (TPacketGGGuildWarMapIndex*) c_pData;
	CGuildManager & gm = CGuildManager::instance();

	sys_log(0, "P2P: GuildWarZoneMapIndex g1(%u) vs g2(%u), mapIndex(%d)", p->dwGuildID1, p->dwGuildID2, p->lMapIndex);

	CGuild * g1 = gm.FindGuild(p->dwGuildID1);
	CGuild * g2 = gm.FindGuild(p->dwGuildID2);

	if (g1 && g2)
	{
		g1->SetGuildWarMapIndex(p->dwGuildID2, p->lMapIndex);
		g2->SetGuildWarMapIndex(p->dwGuildID1, p->lMapIndex);
	}
}

void CInputP2P::Transfer(const char * c_pData)
{
	TPacketGGTransfer * p = (TPacketGGTransfer *) c_pData;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(p->szName);

	if (ch)
		ch->WarpSet(p->lX, p->lY);
}

void CInputP2P::XmasWarpSanta(const char * c_pData)
{
	TPacketGGXmasWarpSanta * p =(TPacketGGXmasWarpSanta *) c_pData;

	if (p->bChannel == g_bChannel && map_allow_find(p->lMapIndex))
	{
		int	iNextSpawnDelay = 50 * 60;

		xmas::SpawnSanta(p->lMapIndex, iNextSpawnDelay);

		TPacketGGXmasWarpSantaReply pack_reply;
		pack_reply.bHeader = HEADER_GG_XMAS_WARP_SANTA_REPLY;
		pack_reply.bChannel = g_bChannel;
		P2P_MANAGER::instance().Send(&pack_reply, sizeof(pack_reply));
	}
}

void CInputP2P::XmasWarpSantaReply(const char* c_pData)
{
	TPacketGGXmasWarpSantaReply* p = (TPacketGGXmasWarpSantaReply*) c_pData;

	if (p->bChannel == g_bChannel)
	{
		CharacterVectorInteractor i;

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(xmas::MOB_SANTA_VNUM, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();

			while (it != i.end()) {
				M2_DESTROY_CHARACTER(*it++);
			}
		}
	}
}

void CInputP2P::LoginPing(LPDESC d, const char * c_pData)
{
	TPacketGGLoginPing * p = (TPacketGGLoginPing *) c_pData;

	SendBillingExpire(p->szLogin, BILLING_DAY, 0, NULL);

	if (!g_pkAuthMasterDesc) // If I am master, I have to broadcast
		P2P_MANAGER::instance().Send(p, sizeof(TPacketGGLoginPing), d);
}

// BLOCK_CHAT
void CInputP2P::BlockChat(const char * c_pData)
{
	TPacketGGBlockChat * p = (TPacketGGBlockChat *) c_pData;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(p->szName);

	if (ch)
	{
		sys_log(0, "BLOCK CHAT apply name %s dur %d", p->szName, p->lBlockDuration);
		ch->AddAffect(AFFECT_BLOCK_CHAT, POINT_NONE, 0, AFF_NONE, p->lBlockDuration, 0, true);
	}
	else
	{
		sys_log(0, "BLOCK CHAT fail name %s dur %d", p->szName, p->lBlockDuration);
	}
}
// END_OF_BLOCK_CHAT

void CInputP2P::IamAwake(LPDESC d, const char * c_pData)
{
	std::string hostNames;
	P2P_MANAGER::instance().GetP2PHostNames(hostNames);
	sys_log(0, "P2P Awakeness check from %s. My P2P connection number is %d. and details...\n%s", d->GetHostName(), P2P_MANAGER::instance().GetDescCount(), hostNames.c_str());
}

#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
#include "new_offlineshop.h"
#include "new_offlineshop_manager.h"

void CInputP2P::OfflineShopNotification (LPDESC d, const char* c_pData)
{
	TPacketGGOfflineShopNotification* p = (TPacketGGOfflineShopNotification*) c_pData;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID (p->ownerId);
	if (!ch)
	{
		return;
	}

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	offlineshop::CShop* pkShop = rManager.GetShopByOwnerID (p->ownerId);

	if (pkShop)
	{
		pkShop->SendNotificationClientPacket (p->vnum, p->price, p->count);
	}
}
#endif

#ifdef ENABLE_LOG_NOTIFICATIONS
void SendCShieldLogNotification(const char* szText)
{
	const auto& descSet = DESC_MANAGER::instance().GetClientSet();

	for (const auto& desc : descSet)
	{
		LPCHARACTER ch = desc->GetCharacter();

		if (ch && ch->GetGMLevel() >= ENABLE_LOG_NOTIFICATIONS)
		{
			char message[CHAT_MAX_LEN];
			snprintf(message, sizeof(message), "%s", szText);

			TPacketGCWhisper whisperPacket;
			memset(&whisperPacket, 0, sizeof(TPacketGCWhisper));
			whisperPacket.bHeader = HEADER_GC_WHISPER;
			whisperPacket.bType = WHISPER_TYPE_GM;
			whisperPacket.wSize = sizeof(TPacketGCWhisper) + strlen(message) + 1;
			strncpy(whisperPacket.szNameFrom, "CShield", sizeof(whisperPacket.szNameFrom));

			desc->BufferedPacket(&whisperPacket, sizeof(whisperPacket));
			desc->Packet(message, strlen(message) + 1);
		}
	}
}

void CInputP2P::CShieldLogNotification(const char* c_pData)
{
	TPacketGGCShieldLogNotification* p = (TPacketGGCShieldLogNotification*)c_pData;
	SendCShieldLogNotification(p->szText);
}
#endif

int CInputP2P::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	if (test_server)
		sys_log(0, "CInputP2P::Analyze[Header %d]", bHeader);

	int iExtraLen = 0;

	switch (bHeader)
	{
		case HEADER_GG_SETUP:
			Setup(d, c_pData);
			break;

		case HEADER_GG_LOGIN:
			Login(d, c_pData);
			break;

		case HEADER_GG_LOGOUT:
			Logout(d, c_pData);
			break;

		case HEADER_GG_RELAY:
			if ((iExtraLen = Relay(d, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#ifdef ENABLE_FULL_NOTICE
		case HEADER_GG_BIG_NOTICE:
			if ((iExtraLen = Notice(d, c_pData, m_iBufferLeft, true)) < 0)
				return -1;
			break;
#endif
		case HEADER_GG_NOTICE:
			if ((iExtraLen = Notice(d, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_GG_SHUTDOWN:
			sys_err("Accept shutdown p2p command from %s.", d->GetHostName());
			Shutdown(10);
			break;

		case HEADER_GG_GUILD:
			if ((iExtraLen = Guild(d, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_GG_SHOUT:
			Shout(c_pData);
			break;

		case HEADER_GG_DISCONNECT:
			Disconnect(c_pData);
			break;

		case HEADER_GG_MESSENGER_ADD:
			MessengerAdd(c_pData);
			break;

		case HEADER_GG_MESSENGER_REMOVE:
			MessengerRemove(c_pData);
			break;

		case HEADER_GG_FIND_POSITION:
			FindPosition(d, c_pData);
			break;

		case HEADER_GG_WARP_CHARACTER:
			WarpCharacter(c_pData);
			break;

		case HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX:
			GuildWarZoneMapIndex(c_pData);
			break;

		case HEADER_GG_TRANSFER:
			Transfer(c_pData);
			break;

		case HEADER_GG_XMAS_WARP_SANTA:
			XmasWarpSanta(c_pData);
			break;

		case HEADER_GG_XMAS_WARP_SANTA_REPLY:
			XmasWarpSantaReply(c_pData);
			break;

		case HEADER_GG_RELOAD_CRC_LIST:
			LoadValidCRCList();
			break;

		case HEADER_GG_CHECK_CLIENT_VERSION:
			CheckClientVersion();
			break;

		case HEADER_GG_LOGIN_PING:
			LoginPing(d, c_pData);
			break;

		case HEADER_GG_BLOCK_CHAT:
			BlockChat(c_pData);
			break;

		case HEADER_GG_SIEGE:
			{
				TPacketGGSiege* pSiege = (TPacketGGSiege*)c_pData;
				castle_siege(pSiege->bEmpire, pSiege->bTowerCount);
			}
			break;

		case HEADER_GG_MONARCH_NOTICE:
			if ((iExtraLen = MonarchNotice(d, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_GG_MONARCH_TRANSFER :
			MonarchTransfer(d, c_pData);
			break;

		case HEADER_GG_CHECK_AWAKENESS:
			IamAwake(d, c_pData);
			break;
#ifdef ENABLE_LOG_NOTIFICATIONS
		case HEADER_GG_CSHIELD_LOG_NOTIFICATION:
			CShieldLogNotification(c_pData);
			break;
#endif

#ifdef ENABLE_ANTI_MULTIPLE_FARM
		case HEADER_GG_ANTI_FARM:
			RecvAntiFarmUpdateStatus(d, c_pData);
			break;
#endif

#ifdef ENABLE_INGAME_ITEMSHOP
		case HEADER_GG_ITEMSHOP:
			if ((iExtraLen = ItemShopNewManager::Instance().ProcessP2P(c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif
}

	return (iExtraLen);
}

#ifdef ENABLE_ANTI_MULTIPLE_FARM
#include "HAntiMultipleFarm.h"

auto CInputP2P::RecvAntiFarmUpdateStatus(LPDESC d, const char* c_pData) -> void // checking the drop status if FALSE then don't drop items for the player unless he closes one client or logout from the other account 
{
	if (!d) return;
	
	CAntiMultipleFarm::TP2PChangeDropStatus* p = (CAntiMultipleFarm::TP2PChangeDropStatus*)c_pData;// here 
	
	std::vector <DWORD> dwPIDs;
	for (uint8_t i = 0; i < MULTIPLE_FARM_MAX_ACCOUNT; ++i)
		dwPIDs.emplace_back(p->dwPIDs[i]);
	
	CAntiMultipleFarm::instance().P2PSendBlockDropStatusChange(p->cMAIf, dwPIDs);
}
#endif
