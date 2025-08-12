#include "stdafx.h"
#include "Cache.h"

#include "QID.h"
#include "ClientManager.h"
#include "Main.h"
#include <fmt/fmt.h>

extern CPacketInfo g_item_info;
extern int g_iPlayerCacheFlushSeconds;
extern int g_iItemCacheFlushSeconds;
extern int g_test_server;
// MYSHOP_PRICE_LIST
extern int g_iItemPriceListTableCacheFlushSeconds;
// END_OF_MYSHOP_PRICE_LIST
//
extern int g_item_count;

CItemCache::CItemCache()
{
	m_expireTime = MIN(1800, g_iItemCacheFlushSeconds);
}

CItemCache::~CItemCache()
{
}

void CItemCache::Delete()
{
	if (m_data.vnum == 0)
		return;

	if (g_test_server)
		sys_log(0, "ItemCache::Delete : DELETE %u", m_data.id);

	m_data.vnum = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();
}

void CItemCache::OnFlush()
{
	if (m_data.vnum == 0)
	{
		char szQuery[QUERY_MAX_LEN];
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM item%s WHERE id=%u", GetTablePostfix(), m_data.id);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_DESTROY, 0, NULL);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush : DELETE %u %s", m_data.id, szQuery);
	}
	else
	{
		TPlayerItem *p = &m_data;
		const auto setQuery = fmt::format(FMT_COMPILE("id={}, owner_id={}, `window`={}, pos={}, count={}, vnum={}, socket0={}, socket1={}, socket2={}, "
														"attrtype0={}, attrvalue0={}, "
														"attrtype1={}, attrvalue1={}, "
														"attrtype2={}, attrvalue2={}, "
														"attrtype3={}, attrvalue3={}, "
														"attrtype4={}, attrvalue4={}, "
														"attrtype5={}, attrvalue5={}, "
														"attrtype6={}, attrvalue6={}, "
														"sealbind={} ")
														, p->id,
														p->owner,
														p->window,
														p->pos,
														p->count,
														p->vnum,
														p->alSockets[0],
														p->alSockets[1],
														p->alSockets[2],
														p->aAttr[0].bType, p->aAttr[0].sValue,
														p->aAttr[1].bType, p->aAttr[1].sValue,
														p->aAttr[2].bType, p->aAttr[2].sValue,
														p->aAttr[3].bType, p->aAttr[3].sValue,
														p->aAttr[4].bType, p->aAttr[4].sValue,
														p->aAttr[5].bType, p->aAttr[5].sValue,
														p->aAttr[6].bType, p->aAttr[6].sValue,
														p->sealbind
		); // @fixme205

		const auto itemQuery = fmt::format(FMT_COMPILE("INSERT INTO item{} SET {} ON DUPLICATE KEY UPDATE {}"),
														GetTablePostfix(), setQuery, setQuery);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush :REPLACE  (%s)", itemQuery.c_str());

		CDBManager::instance().ReturnQuery(itemQuery.c_str(), QID_ITEM_SAVE, 0, NULL);

		++g_item_count;
	}

	m_bNeedQuery = false;
}

//
// CPlayerTableCache
//
CPlayerTableCache::CPlayerTableCache()
{
	m_expireTime = MIN(1800, g_iPlayerCacheFlushSeconds);
}

CPlayerTableCache::~CPlayerTableCache()
{
}

void CPlayerTableCache::OnFlush()
{
	if (g_test_server)
		sys_log(0, "PlayerTableCache::Flush : %s", m_data.name);

	char szQuery[QUERY_MAX_LEN];
	CreatePlayerSaveQuery(szQuery, sizeof(szQuery), &m_data);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_SAVE, 0, NULL);
}

// MYSHOP_PRICE_LIST
//
// CItemPriceListTableCache class implementation
//

const int CItemPriceListTableCache::s_nMinFlushSec = 1800;

CItemPriceListTableCache::CItemPriceListTableCache()
{
	m_expireTime = MIN(s_nMinFlushSec, g_iItemPriceListTableCacheFlushSeconds);
}

void CItemPriceListTableCache::UpdateList(const TItemPriceListTable* pUpdateList)
{
	std::vector<TItemPriceInfo> tmpvec;

	for (uint idx = 0; idx < m_data.byCount; ++idx)
	{
		const TItemPriceInfo* pos = pUpdateList->aPriceInfo;
		for (; pos != pUpdateList->aPriceInfo + pUpdateList->byCount && m_data.aPriceInfo[idx].dwVnum != pos->dwVnum; ++pos)
			;

		if (pos == pUpdateList->aPriceInfo + pUpdateList->byCount)
			tmpvec.emplace_back(m_data.aPriceInfo[idx]);
	}

	if (pUpdateList->byCount > SHOP_PRICELIST_MAX_NUM)
	{
		sys_err("Count overflow!");
		return;
	}

	m_data.byCount = pUpdateList->byCount;

	thecore_memcpy(m_data.aPriceInfo, pUpdateList->aPriceInfo, sizeof(TItemPriceInfo) * pUpdateList->byCount);

	int nDeletedNum;

	if (pUpdateList->byCount < SHOP_PRICELIST_MAX_NUM)
	{
		size_t sizeAddOldDataSize = SHOP_PRICELIST_MAX_NUM - pUpdateList->byCount;

		if (tmpvec.size() < sizeAddOldDataSize)
			sizeAddOldDataSize = tmpvec.size();
		if (tmpvec.size() != 0)
		{
			thecore_memcpy(m_data.aPriceInfo + pUpdateList->byCount, &tmpvec[0], sizeof(TItemPriceInfo) * sizeAddOldDataSize);
			m_data.byCount += sizeAddOldDataSize;
		}
		nDeletedNum = tmpvec.size() - sizeAddOldDataSize;
	}
	else
		nDeletedNum = tmpvec.size();

	m_bNeedQuery = true;

	sys_log(0,
			"ItemPriceListTableCache::UpdateList : OwnerID[%u] Update [%u] Items, Delete [%u] Items, Total [%u] Items",
			m_data.dwOwnerID, pUpdateList->byCount, nDeletedNum, m_data.byCount);
}

void CItemPriceListTableCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];

	snprintf(szQuery, sizeof(szQuery), "DELETE FROM myshop_pricelist%s WHERE owner_id = %u", GetTablePostfix(), m_data.dwOwnerID);
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_DESTROY, 0, NULL);

	for (int idx = 0; idx < m_data.byCount; ++idx)
	{
		snprintf(szQuery, sizeof(szQuery),
				"REPLACE myshop_pricelist%s(owner_id, item_vnum, price) VALUES(%u, %u, %lld)", // @fixme204 (INSERT INTO -> REPLACE)
				GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].dwPrice);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_SAVE, 0, NULL);
	}

	sys_log(0, "ItemPriceListTableCache::Flush : OwnerID[%u] Update [%u]Items", m_data.dwOwnerID, m_data.byCount);

	m_bNeedQuery = false;
}

#ifdef ENABLE_CSHIELD
extern int g_iCShieldCacheFlushSeconds;

CCShieldCache::CCShieldCache()
{
	m_expireTime = MIN(1800, g_iCShieldCacheFlushSeconds);
}

CCShieldCache::~CCShieldCache()
{
}

void CCShieldCache::OnFlush()
{
	char query[QUERY_MAX_LEN];
	snprintf(query, sizeof(query),
		"REPLACE INTO cshield%s (id, name, "
		"metin_count, boss_count"
#ifdef ENABLE_CAPTCHA
		", captcha_time, captcha_count"
#endif
		") "
		"VALUES (%d, '%s', "
		"%d, %d"
#ifdef ENABLE_CAPTCHA
		", %d, %d "
#endif
		")", GetTablePostfix(), m_data.playerId, m_data.name,
		m_data.metinCount, m_data.bossCount
#ifdef ENABLE_CAPTCHA
		, m_data.captchaTime, m_data.captchaCount
#endif
		);

	CDBManager::instance().ReturnQuery(query, QID_CSHIELD_SAVE, 0, NULL);

	if (g_test_server)
		sys_log(0, "CShieldCache::Flush :REPLACE %u (%s)", m_data.playerId, query);

	m_bNeedQuery = false;
}

#ifdef ENABLE_EXTERNAL_REPORTS
CCShieldReportCache::CCShieldReportCache()
{
	m_expireTime = 60;
}

CCShieldReportCache::~CCShieldReportCache()
{
}

#include <sstream>
std::string CCShieldReportCache::GetAccountIds(const char* hwidEscaped)
{
	std::ostringstream oss;
	char query[QUERY_MAX_LEN];
	snprintf(query, sizeof(query), "SELECT id FROM account.account WHERE cshield_hwid='%s'", hwidEscaped);
	std::unique_ptr<SQLMsg> msg(CDBManager::instance().DirectQuery(query));

	if (msg->Get()->uiNumRows != 0)
	{
		MYSQL_ROW row;
		bool first = true;
		while ((row = mysql_fetch_row(msg->Get()->pSQLResult)))
		{
			if (!first)
			{
				oss << ", ";
			}
			oss << row[0];
			first = false;
		}
	}

	return oss.str();
}

void CCShieldReportCache::OnFlush()
{
	char languageEscaped[3 * 2 + 1] = { 0 };
	char hwidEscaped[128 * 2 + 1] = { 0 };
	char reportEscaped[1024 * 2 + 1] = { 0 };

	CDBManager::instance().EscapeString(languageEscaped, m_data.language, strlen(m_data.language) );
	CDBManager::instance().EscapeString(hwidEscaped, m_data.cshieldHwid, strlen(m_data.cshieldHwid));
	CDBManager::instance().EscapeString(reportEscaped, m_data.report, strlen(m_data.report));

	std::string accountIds = GetAccountIds(hwidEscaped);

	char query[QUERY_MAX_LEN];
	snprintf(query, sizeof(query),
		"INSERT INTO log.cshield_log (time, type, accountIds, ip, errorcode, why, win, language, hwid, report) "
		"VALUES (NOW(), %d, '%s', '%s', %u, '%s', %u, '%s', '%s', '%s')",
		2, accountIds.c_str(), m_data.ip, m_data.errorcode, m_data.why, m_data.win, languageEscaped, hwidEscaped, reportEscaped);

	CDBManager::instance().ReturnQuery(query, QID_CSHIELD_REPORT_SAVE, 0, NULL);

	if (g_test_server)
		sys_log(0, "CShieldReportCache::Flush :INSERT %s (%s)", accountIds.c_str(), query);

	m_bNeedQuery = false;
}
#endif
#endif

CItemPriceListTableCache::~CItemPriceListTableCache()
{
}

// END_OF_MYSHOP_PRICE_LIST
