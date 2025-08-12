#ifndef __INC_METIN_II_GAME_SHOP_H__
#define __INC_METIN_II_GAME_SHOP_H__

enum
{
	SHOP_MAX_DISTANCE = 1000
};

class CGrid;

/* ---------------------------------------------------------------------------------- */
class CShop
{
	public:
		typedef struct shop_item
		{
			DWORD	vnum;
			long long	price;
#ifdef ENABLE_CHEQUE_SYSTEM
			int		cheque = 0;
#endif
//			BYTE	count;
			short	count;
#ifdef ENABLE_MULTISHOP
			DWORD	wPriceVnum;
			DWORD	wPrice;
#endif
			LPITEM	pkItem;
			int		itemid;

			shop_item()
			{
				vnum = 0;
				price = 0;
				count = 0;
#ifdef ENABLE_MULTISHOP
				wPriceVnum = 0;
				wPrice = 0;
#endif
				itemid = 0;
				pkItem = NULL;
			}
		} SHOP_ITEM;

		CShop();
		virtual ~CShop(); // @fixme139 (+virtual)

		bool			Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable * pItemTable);
//		void			SetShopItems(TShopItemTable * pItemTable, BYTE bItemCount);
		void			SetShopItems(TShopItemTable * pItemTable, short bItemCount);
		virtual void	SetPCShop(LPCHARACTER ch);
		virtual bool	IsPCShop()	{ return m_pkPC ? true : false; }

		virtual bool	AddGuest(LPCHARACTER ch,DWORD owner_vid, bool bOtherEmpire);
		void			RemoveGuest(LPCHARACTER ch);

		virtual long long	Buy(LPCHARACTER ch, BYTE pos);

		void			BroadcastUpdateItem(BYTE pos);
		int				GetNumberByVnum(DWORD dwVnum);
		virtual bool	IsSellingItem(DWORD itemID);

		DWORD	GetVnum() { return m_dwVnum; }
		DWORD	GetNPCVnum() { return m_dwNPCVnum; }

	protected:
		void	Broadcast(const void * data, int bytes);

	protected:
		DWORD				m_dwVnum;
		DWORD				m_dwNPCVnum;

		CGrid *				m_pGrid;

		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		std::vector<SHOP_ITEM>		m_itemVector;

		LPCHARACTER			m_pkPC;
};

#endif
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
