#include "stdafx.h"

#ifdef ENABLE_DUNGEON_INFO_SYSTEM

#if !defined(_DUNGEON_INFO_MANAGER_H_)
#define _DUNGEON_INFO_MANAGER_H_

#define DUNGEON_TOKEN(string) if (!str_cmp(szTokenString, string))

#include "locale_service.h"
#include "char.h"
#include "desc.h"
#include "packet.h"
#include "quest.h"
#include "questmanager.h"
#include "utils.h"
#include "config.h"
#include "desc_manager.h"
#include "db.h"
#include "buffer_manager.h"

enum EQuestType
{
	QUEST_FLAG_PC,
	QUEST_FLAG_GLOBAL
};

struct SDungeonLimits
{
	int32_t iMin, iMax;
	bool operator == (const SDungeonLimits& c_sRef)
	{
		return (this->iMin == c_sRef.iMin)
			&& (this->iMax == c_sRef.iMax);
	}
};

struct SDungeonEntryPosition
{
	long lBaseX, lBaseY;
	bool operator == (const SDungeonEntryPosition& c_sRef)
	{
		return (this->lBaseX == c_sRef.lBaseX)
			&& (this->lBaseY == c_sRef.lBaseY);
	}
};

struct SDungeonBonus
{
	uint16_t byAttBonus, byDefBonus;
	bool operator == (const SDungeonBonus& c_sRef)
	{
		return (this->byAttBonus == c_sRef.byAttBonus)
			&& (this->byDefBonus == c_sRef.byDefBonus);
	}
};

struct SDungeonItem
{
	uint32_t dwVnum;
	uint16_t wCount;

	bool operator == (const SDungeonItem& c_sRef)
	{
		return (this->wCount == c_sRef.wCount)
			&& (this->dwVnum == c_sRef.dwVnum);
	}
};

struct SDungeonQuest
{
	std::string strQuest;
	std::string strQuestFlag;
	uint8_t byType;

	bool operator == (const SDungeonQuest& c_sRef)
	{
		return (this->byType == c_sRef.byType) &&
			(this->strQuestFlag == c_sRef.strQuestFlag)
			&& (this->strQuest == c_sRef.strQuest);
	}
};

struct SDungeonData
{
	uint16_t byType = 0;

	long lMapIndex = 0;
	long lEntryMapIndex = 0;

	std::vector<SDungeonEntryPosition> vecEntryPosition;

	uint32_t dwBossVnum = 0;

	std::vector<SDungeonLimits> vecLevelLimit;
	std::vector<SDungeonLimits> vecMemberLimit;
	std::vector<SDungeonItem> vecRequiredItem;

	uint32_t dwDuration = 0;
	uint32_t dwCooldown = 0;
	uint8_t byElement = 0;

	std::vector<SDungeonBonus> vecBonus;
	std::vector<SDungeonItem> vecBossDropItem;

	std::vector<SDungeonQuest> vecQuest;

	SDungeonData();
};

class CDungeonInfoManager : public singleton<CDungeonInfoManager>
{
public:
	CDungeonInfoManager();
	virtual ~CDungeonInfoManager();

	void Destroy();

	void Initialize();
	bool LoadFile(const char* c_szFileName);

	void Reload();

	void SendInfo(LPCHARACTER pkCh, BOOL bReset = FALSE);
	void Warp(LPCHARACTER pkCh, uint16_t byIndex);
	uint32_t GetResult(LPCHARACTER pkCh, std::string strQuest, std::string strFlag);
	void Ranking(LPCHARACTER pkCh, uint16_t byIndex, uint8_t byRankType = 0);
	void UpdateRanking(LPCHARACTER pkCh, const std::string c_strQuestName);


};

#endif /* _DUNGEON_INFO_MANAGER_H_ */

#endif // __DUNGEON_INFO_SYSTEM__
