// vim:ts=8 sw=4
#ifndef __INC_DB_CACHE_H__
#define __INC_DB_CACHE_H__

#include "../../common/cache.h"

class CItemCache : public cache<TPlayerItem>
{
    public:
	CItemCache();
	virtual ~CItemCache();

	void Delete();
	virtual void OnFlush();
};

class CPlayerTableCache : public cache<TPlayerTable>
{
    public:
	CPlayerTableCache();
	virtual ~CPlayerTableCache();

	virtual void OnFlush();

	DWORD GetLastUpdateTime() { return m_lastUpdateTime; }
};

// MYSHOP_PRICE_LIST

class CItemPriceListTableCache : public cache< TItemPriceListTable >
{
    public:

	/// Constructor

	CItemPriceListTableCache(void);
	virtual ~CItemPriceListTableCache();

	void	UpdateList(const TItemPriceListTable* pUpdateList);

	virtual void	OnFlush(void);

    private:

	static const int	s_nMinFlushSec;		///< Minimum cache expire time
};
// END_OF_MYSHOP_PRICE_LIST

#ifdef ENABLE_CSHIELD
class CCShieldCache : public cache<TCShield>
{
public:
	CCShieldCache();
	virtual ~CCShieldCache();

	virtual void OnFlush();
};

#ifdef ENABLE_EXTERNAL_REPORTS
class CCShieldReportCache : public cache<TCShieldReport>
{
public:
	CCShieldReportCache();
	virtual ~CCShieldReportCache();

	std::string GetAccountIds(const char* szCShieldHwid);
	virtual void OnFlush();
};
#endif
#endif

#endif
