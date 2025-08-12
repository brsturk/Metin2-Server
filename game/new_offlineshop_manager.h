#ifndef __INCLUDE_HEADER_OFFLINESHOP_MANAGER__
#define __INCLUDE_HEADER_OFFLINESHOP_MANAGER__

#ifdef __ENABLE_NEW_OFFLINESHOP__
#define SUBTYPE_NOSET 255
#define OFFLINESHOP_DURATION_UPDATE_TIME PASSES_PER_SEC(60)
#define OFFLINESHOP_AUCTION_RAISE_PERCENTAGE 10 //to raise the offer will be 10% more than best offer

namespace offlineshop
{
	

	class CShopManager : public singleton<CShopManager>
	{
	public:

#ifdef ENABLE_NEW_SHOP_IN_CITIES
		typedef std::map<DWORD, ShopEntity*> SHOPENTITIES_MAP;

		typedef struct SCityShopInfo {
			SHOPENTITIES_MAP	entitiesByPID;
			SHOPENTITIES_MAP	entitiesByVID;


			void Clear()
			{
				entitiesByPID.clear();
				entitiesByVID.clear();
			}


			SCityShopInfo()
			{
				Clear();
			}


			SCityShopInfo(const SCityShopInfo& rCopy)
			{
				CopyContainer(entitiesByPID, rCopy.entitiesByPID);
				CopyContainer(entitiesByVID, rCopy.entitiesByVID);
			}

		} TCityShopInfo;
#endif

		typedef std::map<DWORD,CShop>					 SHOPMAP;
		typedef std::map<DWORD,CShopSafebox>			 SAFEBOXMAP;
		typedef std::map<DWORD,std::vector<TOfferInfo> > OFFERSMAP;
		typedef std::map<DWORD, DWORD>					 SEARCHTIMEMAP;
		typedef std::map<DWORD, CAuction>				 AUCTIONMAP;

#ifdef ENABLE_NEW_SHOP_IN_CITIES
		typedef std::vector<TCityShopInfo>				 CITIESVEC;
#endif
		
		CShopManager();
		~CShopManager();

		//booting
#ifdef ENABLE_OFFLINESHOP_REWORK
		offlineshop::CShop* PutsNewShop(TShopInfo* pInfo, TShopPosition* pPosInfo);
#else
		offlineshop::CShop* PutsNewShop(TShopInfo* pInfo);
#endif
		void					PutsAuction(const TAuctionInfo& auction);
		void					PutsAuctionOffer(const TAuctionOfferInfo& offer);

		offlineshop::CShop*		GetShopByOwnerID(DWORD dwPID);
		CShopSafebox*			GetShopSafeboxByOwnerID(DWORD dwPID);
		CAuction*				GetAuctionByOwnerID(DWORD dwPID);
		//offers
		bool					PutsNewOffer(const TOfferInfo* pInfo);

		void					RemoveSafeboxFromCache(DWORD dwOwnerID);
		void					RemoveGuestFromShops(LPCHARACTER ch);

#ifdef ENABLE_NEW_SHOP_IN_CITIES
	public:
#ifdef ENABLE_OFFLINESHOP_REWORK
		void		CreateNewShopEntities(offlineshop::CShop& rShop, TShopPosition& pos);
#else
		void		CreateNewShopEntities(offlineshop::CShop& rShop);
#endif
		void		DestroyNewShopEntities(const offlineshop::CShop& rShop);

		void		EncodeInsertShopEntity(ShopEntity& shop, LPCHARACTER ch);
		void		EncodeRemoveShopEntity(ShopEntity& shop, LPCHARACTER ch);

	private:
#ifndef ENABLE_OFFLINESHOP_REWORK
		bool		__CanUseCity(size_t index);
		bool		__CheckEntitySpawnPos(const long x, const long y, const TCityShopInfo& city);
#endif
		void		__UpdateEntity(const offlineshop::CShop& rShop);
#endif



	public:
//packets exchanging db
//ITEMS
/*db*/	void		SendShopBuyDBPacket(DWORD dwBuyerID, DWORD dwOwnerID,DWORD dwItemID);
/*db*/	void		SendShopLockBuyItemDBPacket(DWORD dwBuyerID, DWORD dwOwnerID,DWORD dwItemID);

/*db*/	bool		RecvShopLockedBuyItemDBPacket(DWORD dwBuyerID, DWORD dwOwnerID,DWORD dwItemID);
/*db*/	bool		RecvShopBuyDBPacket(DWORD dwBuyerID, DWORD dwOwnerID,DWORD dwItemID);
/*db*/	void		SendShopCannotBuyLockedItemDBPacket(DWORD dwOwnerID, DWORD dwItemID); //topatch

/*db*/	void		SendShopEditItemDBPacket(DWORD dwOwnerID, DWORD dwItemID, const TPriceInfo& rPrice);
/*db*/	bool		RecvShopEditItemDBPacket(DWORD dwOwnerID, DWORD dwItemID, const TPriceInfo& rPrice);
			

/*db*/	void		SendShopRemoveItemDBPacket(DWORD dwOwnerID, DWORD dwItemID);
/*db*/	bool		RecvShopRemoveItemDBPacket(DWORD dwOwnerID, DWORD dwItemID);
		

/*db*/	void		SendShopAddItemDBPacket(DWORD dwOwnerID, const TItemInfo& rItemInfo);
/*db*/	bool		RecvShopAddItemDBPacket(DWORD dwOwnerID, const TItemInfo& rItemInfo);
		
//SHOPS 
/*db*/	void		SendShopForceCloseDBPacket(DWORD dwPID);
/*db*/	bool		RecvShopForceCloseDBPacket(DWORD dwPID);
/*db*/	bool		RecvShopExpiredDBPacket(DWORD dwPID);
		
#ifdef ENABLE_OFFLINESHOP_REWORK
/*db*/	void		SendShopCreateNewDBPacket(const TShopInfo&, const TShopPosition& pos, std::vector<TItemInfo>& vec);
/*db*/	bool		RecvShopCreateNewDBPacket(const TShopInfo&, TShopPosition& pos, std::vector<TItemInfo>& vec);
#else
/*db*/	void		SendShopCreateNewDBPacket(const TShopInfo&, std::vector<TItemInfo>& vec);
/*db*/	bool		RecvShopCreateNewDBPacket(const TShopInfo&, std::vector<TItemInfo>& vec);
#endif

/*db*/	void		SendShopChangeNameDBPacket(DWORD dwOwnerID, const char* szName);
/*db*/	bool		RecvShopChangeNameDBPacket(DWORD dwOwnerID, const char* szName);

#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
	bool RecvShopExtendTimeClientPacket(LPCHARACTER ch, DWORD dwTime);
	void SendShopExtendTimeDBPacket(DWORD dwOwnerID, DWORD dwTime);
	bool RecvShopExtendTimeDBPacket(DWORD dwOwnerID, DWORD dwTime);
#endif


//OFFER
/*db*/	void		SendShopOfferNewDBPacket(const TOfferInfo& offer);
/*db*/	bool		RecvShopOfferNewDBPacket(const TOfferInfo& offer);
		
/*db*/	void		SendShopOfferNotifiedDBPacket(DWORD dwOfferID, DWORD dwOwnerID);
/*db*/	bool		RecvShopOfferNotifiedDBPacket(DWORD dwOfferID, DWORD dwOwnerID);

/*db*/	void		SendShopOfferAcceptDBPacket(const TOfferInfo& offer);
/*db*/	bool		RecvShopOfferAcceptDBPacket(DWORD dwOfferID, DWORD dwOwnerID);

/*db*/	void		SendShopOfferCancelDBPacket(const TOfferInfo& offer);
/*db*/	bool		RecvShopOfferCancelDBPacket(DWORD dwOfferID, DWORD dwOwnerID, bool isRemovingItem);//offlineshop-updated 05/08/19

		
/*db*/	void		SendShopSafeboxGetItemDBPacket(DWORD dwOwnerID, DWORD dwItemID);
/*db*/	void		SendShopSafeboxGetValutesDBPacket(DWORD dwOwnerID, const TValutesInfo& valutes);
/*db*/  bool		SendShopSafeboxAddItemDBPacket(DWORD dwOwnerID, const CShopItem& item);
/*db*/	bool		RecvShopSafeboxAddItemDBPacket(DWORD dwOwnerID, DWORD dwItemID, const TItemInfoEx& item);
/*db*/	bool		RecvShopSafeboxAddValutesDBPacket(DWORD dwOwnerID, const TValutesInfo& valute);
/*db*/	bool		RecvShopSafeboxLoadDBPacket(DWORD dwOwnerID, const TValutesInfo& valute, const std::vector<DWORD>& ids, const std::vector<TItemInfoEx>& items);
//patch 08-03-2020
/*db*/  bool		RecvShopSafeboxExpiredItemDBPacket(DWORD dwOwnerID, DWORD dwItemID);


//AUCTION
/*db*/	void		SendAuctionCreateDBPacket(const TAuctionInfo& auction);
/*db*/	void		SendAuctionAddOfferDBPacket(const TAuctionOfferInfo& offer);

/*db*/	bool		RecvAuctionCreateDBPacket(const TAuctionInfo& auction);
/*db*/	bool		RecvAuctionAddOfferDBPacket(const TAuctionOfferInfo& offer);
/*db*/	bool		RecvAuctionExpiredDBPacket(DWORD dwID);


//packets echanging clients
//SHOPS
/*cli.*/bool		RecvShopCreateNewClientPacket(LPCHARACTER ch, TShopInfo& rShopInfo, std::vector<TShopItemInfo> & vec);
/*cli.*/bool		RecvShopChangeNameClientPacket(LPCHARACTER ch, const char* szName);
/*cli.*/bool		RecvShopForceCloseClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvShopRequestListClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvShopOpenClientPacket(LPCHARACTER ch, DWORD dwOwnerID);
/*cli.*/bool		RecvShopOpenMyShopClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvShopBuyItemClientPacket(LPCHARACTER ch, DWORD dwOwnerID, DWORD dwItemID, bool isSearch);
#ifdef ENABLE_NEW_SHOP_IN_CITIES
/*cli.*/bool		RecvShopClickEntity(LPCHARACTER ch, DWORD dwShopEntityVID);
#endif

/*cli.*/void		SendShopListClientPacket(LPCHARACTER ch);
/*cli.*/void		SendShopOpenClientPacket(LPCHARACTER ch, CShop* pkShop);
/*cli.*/void		SendShopOpenMyShopClientPacket(LPCHARACTER ch);
/*cli.*/void		SendShopOpenMyShopNoShopClientPacket(LPCHARACTER ch);
/*cli.*/void		SendShopBuyItemFromSearchClientPacket(LPCHARACTER ch, DWORD dwOwnerID, DWORD dwItemID);
		
/*cli.*/void		SendShopForceClosedClientPacket(DWORD dwOwnerID);


		//ITEMS
/*cli.*/bool		RecvShopAddItemClientPacket(LPCHARACTER ch, const TItemPos& item, const TPriceInfo& price);
/*cli.*/bool		RecvShopRemoveItemClientPacket(LPCHARACTER ch, DWORD dwItemID);
/*cli.*/bool		RecvShopEditItemClientPacket(LPCHARACTER ch, DWORD dwItemID, const TPriceInfo& price);

		//FILTER
/*cli.*/bool		RecvShopFilterRequestClientPacket(LPCHARACTER ch, const TFilterInfo& filter);
/*cli.*/void		SendShopFilterResultClientPacket(LPCHARACTER ch, const std::vector<TItemInfo>& items);


		//OFFERS
/*cli.*/bool		RecvShopCreateOfferClientPacket(LPCHARACTER ch, TOfferInfo& offer);
/*cli.*/bool		RecvShopEditOfferClientPacket(LPCHARACTER ch, const TOfferInfo& offer);/*unused*/
/*cli.*/bool		RecvShopAcceptOfferClientPacket(LPCHARACTER ch, DWORD dwOfferID);
/*cli.*/bool		RecvShopCancelOfferClientPacket(LPCHARACTER ch, DWORD dwOfferID, DWORD dwOwnerID);
/*cli.*/bool		RecvOfferListRequestPacket(LPCHARACTER ch);

		
		//SAFEBOX
/*cli.*/bool		RecvShopSafeboxOpenClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvShopSafeboxGetItemClientPacket(LPCHARACTER ch, DWORD dwItemID);
/*cli.*/bool		RecvShopSafeboxGetValutesClientPacket(LPCHARACTER ch, const TValutesInfo& valutes);
/*cli.*/bool		RecvShopSafeboxCloseClientPacket(LPCHARACTER ch);
		

		//AUCTION
/*cli.*/bool		RecvAuctionListRequestClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvAuctionOpenRequestClientPacket(LPCHARACTER ch, DWORD dwOwnerID);
/*cli.*/bool		RecvMyAuctionOpenRequestClientPacket(LPCHARACTER ch);
/*cli.*/bool		RecvAuctionCreateClientPacket(LPCHARACTER ch, DWORD dwDuration, const TPriceInfo& init_price, const TItemPos& pos);
/*cli.*/bool		RecvAuctionAddOfferClientPacket(LPCHARACTER ch, DWORD dwOwnerID, const TPriceInfo& price);
/*cli.*/bool		RecvAuctionExitFromAuction(LPCHARACTER ch, DWORD dwOwnerID);

/*cli.*/void		SendAuctionListClientPacket(LPCHARACTER ch, const std::vector<TAuctionListElement>& auctionVec);
/*cli.*/void		SendAuctionOpenAuctionClientPacket(LPCHARACTER ch, const TAuctionInfo& auction, const std::vector<TAuctionOfferInfo>& vec); 
/*cli.*/void		SendAuctionOpenMyAuctionNoAuctionClientPacket(LPCHARACTER ch);


/*cli.*/void		SendShopSafeboxRefresh(LPCHARACTER ch, const TValutesInfo& valute, const std::vector<CShopItem>& vec);
		
/*cli.*/void		RecvCloseBoardClientPacket(LPCHARACTER ch);

		//other
		void		UpdateShopsDuration();
		void		UpdateAuctionsDuration();

		//search time map (checking to avoid search abouse)
		void		ClearSearchTimeMap();
		bool		CheckSearchTime(DWORD dwPID);

		void		Destroy();
#ifdef ENABLE_OFFLINESHOP_REWORK
		int			GetMapIndexAllowsList(int iMapIndex);
#endif

	private:
		SHOPMAP			m_mapShops;
		SAFEBOXMAP		m_mapSafeboxs;
		OFFERSMAP		m_mapOffer;

		LPEVENT			m_eventShopDuration;
		SEARCHTIMEMAP	m_searchTimeMap;
		AUCTIONMAP		m_mapAuctions;

#ifdef ENABLE_NEW_SHOP_IN_CITIES
		CITIESVEC		m_vecCities;
#endif
#ifdef ENABLE_OFFLINESHOP_REWORK
		std::set<int> s_set_offlineshop_map_allows;
#endif
	};




	offlineshop::CShopManager& GetManager();
}

#endif//__ENABLE_NEW_OFFLINESHOP__
#endif //__INCLUDE_HEADER_OFFLINESHOP_MANAGER__
