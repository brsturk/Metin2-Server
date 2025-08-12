#include "stdafx.h"
#include "ItemShopNewManager.h"
#ifdef __WIN32__
#include <mysql/mysql.h>
#include <memory>
#endif

#include "char.h"
#include "desc.h"
#include "db.h"
#include "config.h"
#include "guild.h"
#include <vector>
#include "buffer_manager.h"
#include "desc_client.h"
#include "item.h"
#include "item_manager.h"
#include "utils.h"
#include "p2p.h"


ItemShopNewManager::ItemShopNewManager()
{
	m_Categories.clear();
	m_Items.clear();
}


ItemShopNewManager::~ItemShopNewManager()
{
	m_Categories.clear();
	m_Items.clear();
}

void ItemShopNewManager::Initialize()
{
	m_Categories.clear();
	m_Items.clear();

	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT *, UNIX_TIMESTAMP(limitedTime) as limitedTimeStamp FROM %s.itemshop ORDER BY limited DESC, id ASC;", "player"));
	
	if (msg->uiSQLErrno != 0)
	{
		sys_err("<ItemShopNewManager> Initialize: Could not load items!");
		return;
	}
		
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(msg->Get()->pSQLResult)))
	{
		auto item = std::make_shared<TItemShopItem>();

		str_to_number(item->id, row[0]);
		str_to_number(item->vnum, row[1]);
		str_to_number(item->sockets[0], row[2]);
		str_to_number(item->sockets[1], row[3]);
		str_to_number(item->sockets[2], row[4]);
		
		//str_to_number(item->sockets[3], row[5]); // We dont have socket3
		
		str_to_number(item->coins, row[6]);
		str_to_number(item->category, row[7]);
		str_to_number(item->limited, row[8]);
		str_to_number(item->limitedCount, row[10]);

		//row[9] == DATETIME, we need timeStamp -> 11
		str_to_number(item->limitedTime, row[11]);
		
		m_Items.emplace_back(std::make_pair(item->id, item));
	}

	std::unique_ptr<SQLMsg> msg2(DBManager::instance().DirectQuery("SELECT * FROM %s.itemshop_categories ORDER BY id ASC;", "player"));

	if (msg2->uiSQLErrno != 0)
	{
		sys_err("<ItemShopNewManager> Initialize: Could not load categories!");
		return;
	}

	while ((row = mysql_fetch_row(msg2->Get()->pSQLResult)))
	{
		auto cat = std::make_shared<TItemShopCategory>();

		str_to_number(cat->id, row[0]);
		strncpy(cat->name, row[1], sizeof(cat->name));
		str_to_number(cat->main_category, row[2]);
		str_to_number(cat->color_id, row[3]);

		m_Categories.insert(std::make_pair(cat->id, cat));
	}

	sys_log(0, "<ItemShopNewManager> Initialize Finished");
}

void ItemShopNewManager::Open(LPCHARACTER ch)
{
	if (ch == nullptr)
		return;

	if (ch->GetQuestFlag("itemshop.lasttime") > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2099]");
		return;
	}

	ch->SetQuestFlag("itemshop.lasttime", get_global_time() + 5);
	
	{
		uint32_t leftItemcount = m_Items.size();

		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.subHeader = IS_GC_SUB_ITEMS;

		TPacketItemShopItems subpack;
		subpack.clear = true;
		subpack.item_count = leftItemcount > 50 ? 50 : leftItemcount; // Only send 50 items per packet

		pack.size = sizeof(pack) + sizeof(TPacketItemShopItems) + sizeof(TItemShopItem) *subpack.item_count;

		TEMP_BUFFER buff;
		buff.write(&pack, sizeof(pack));
		buff.write(&subpack, sizeof(subpack));

		uint32_t currentItemCount = 0;
		for (auto item : m_Items)
		{
			if (currentItemCount == 50)
			{
				if (leftItemcount > 0)
				{
					ch->GetDesc()->Packet(buff.read_peek(), buff.size());

					buff.reset();
					currentItemCount = 0;
					subpack.item_count = leftItemcount > 50 ? 50 : leftItemcount; // Only send 50 items per packet
					subpack.clear = false;
					pack.size = sizeof(pack) + sizeof(TPacketItemShopItems) + sizeof(TItemShopItem) *subpack.item_count;

					buff.write(&pack, sizeof(pack));
					buff.write(&subpack, sizeof(subpack));
				}
			}
			buff.write(item.second.get(), sizeof(TItemShopItem));
			currentItemCount++;
			leftItemcount--;
		}

		ch->GetDesc()->Packet(buff.read_peek(), buff.size());
	}

	{
		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.subHeader = IS_GC_SUB_CATEGORIES;

		TPacketItemShopCategories subpack;
		subpack.cat_count = m_Categories.size();

		pack.size = sizeof(pack) + sizeof(TPacketItemShopCategories) + sizeof(TItemShopCategory) *subpack.cat_count;

		TEMP_BUFFER buff;
		buff.write(&pack, sizeof(pack));
		buff.write(&subpack, sizeof(subpack));

		for (auto cat : m_Categories)
			buff.write(cat.second.get(), sizeof(TItemShopCategory));

		ch->GetDesc()->Packet(buff.read_peek(), buff.size());
	}
	
	{
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT coins, silvercoins FROM %s.account WHERE id = '%d'", "account", ch->GetDesc()->GetAccountTable().id));

		MYSQL_RES *res = msg->Get()->pSQLResult;
		uint32_t goldCoins = 0;
		uint32_t silverCoins = 0;
		
		if ((msg->uiSQLErrno != 0 || !res || !msg->Get() || msg->Get()->uiNumRows < 1) == false)
		{
			MYSQL_ROW row = mysql_fetch_row(res);
			str_to_number(goldCoins, row[0]);
			str_to_number(silverCoins, row[1]);
		}
		
		
		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.size = sizeof(pack) + sizeof(TPacketItemShopCoins);
		pack.subHeader = IS_GC_SUB_COINS;

		TPacketItemShopCoins subpack;
		subpack.gold = goldCoins;
		subpack.silver = silverCoins;

		TEMP_BUFFER buff;
		buff.write(&pack, sizeof(pack));
		buff.write(&subpack, sizeof(subpack));
		ch->GetDesc()->Packet(buff.read_peek(), buff.size());

	}
	
	{
		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.size = sizeof(pack);
		pack.subHeader = IS_GC_SUB_OPEN;
		ch->GetDesc()->Packet(&pack, sizeof(pack));
	}
}

uint32_t ItemShopNewManager::GetCoins(LPCHARACTER ch, uint8_t type)
{
	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT coins, silvercoins FROM %s.account WHERE id = '%d'", "account", ch->GetDesc()->GetAccountTable().id));

	MYSQL_RES *res = msg->Get()->pSQLResult;
	if (msg->uiSQLErrno != 0 || !res || !msg->Get() || msg->Get()->uiNumRows < 1)
	{
		return 0;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	
	uint32_t goldCoins = 0;
	uint32_t silverCoins = 0;
	
	str_to_number(goldCoins, row[0]);
	str_to_number(silverCoins, row[1]);

	if (type == 0)
		return goldCoins;
	
	return silverCoins;
}

bool ItemShopNewManager::ChangeCoins(LPCHARACTER ch, uint32_t coins, uint8_t type)
{
	const char* coinsName = "";
	if (type == 0)
		coinsName = "coins";
	else
		coinsName = "silvercoins";

	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("UPDATE %s.account SET %s = %s - %d WHERE id = '%d'", "account", coinsName, coinsName, coins, ch->GetDesc()->GetAccountTable().id));
	if (msg->uiSQLErrno != 0)
	{
		sys_log(0, "<Itemshop> Error: #0002 %s", ch->GetName());
		return false;
	}
	return true;
}

void ItemShopNewManager::Buy(LPCHARACTER ch, uint16_t id, uint16_t count)
{
	if (!ch)
	{
		sys_err("Null character pointer in Buy function");
		return;
	}

	const auto& item = std::find_if(m_Items.begin(), m_Items.end(),
		[&id](const std::pair<uint16_t, std::shared_ptr<TItemShopItem>>& element){ return element.first == id; });

	if (item == m_Items.end())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2093]");
		return;
	}

	const auto category = m_Categories.find(item->second->category);

	if (category == m_Categories.end())
		return;

	if (count <= 0)
		return;

	if (item->second->limited)
	{
		if (item->second->limitedTime - get_global_time() < 0 && item->second->limitedCount <= 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2094]");
			return;
		}

		if (static_cast<long long>(item->second->limitedCount) - count < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2095]");
			return;
		}
	}

	if (GetCoins(ch, category->second->main_category) < item->second->coins * count)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2096]");
		return;
	}

	const auto item_table = ITEM_MANAGER::instance().GetTable(item->second->vnum);

	if (!item_table)
		return;

	if ((item_table->dwAntiFlags & ITEM_ANTIFLAG_STACK) != 0)
	{
		count = 1;
	}
	else
	{
		count = MIN(count, ITEM_MAX_COUNT);
	}

	if (ch->GetEmptyInventory(item_table->bSize) == -1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2097]");
		return;
	}

	if (!ChangeCoins(ch, item->second->coins * count, category->second->main_category))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2098]");
		return;
	}

	// Öğe oluştur ve socket değerlerini uygula
	LPITEM new_item = ch->AutoGiveItem(item->second->vnum, count, -1, false);
	if (new_item)
	{
		// Socket değerlerini uygula
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
		{
			new_item->SetSocket(i, item->second->sockets[i]);
		}

		// Hata ayıklama için log
		sys_log(0, "Buy Item: ID=%d, Vnum=%u, Count=%u, Socket0=%d, Socket1=%d, Socket2=%d",
				id, item->second->vnum, count, new_item->GetSocket(0), new_item->GetSocket(1), new_item->GetSocket(2));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Failed to create item.");
		return;
	}

	if (item->second->limited && item->second->limitedCount > 0)
	{
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("UPDATE %s.itemshop SET limitedCount = '%d' WHERE id = '%d'", "player", item->second->limitedCount - count, id));

		TPacketItemShopGG p2;
		p2.header = HEADER_GG_ITEMSHOP;
		p2.subheader = IS_GG_BUY_LIMITED;

		TPacketItemShopLimitedItem p3;
		p3.id = id;
		p3.count = count;

		TEMP_BUFFER buf2;
		buf2.write(&p2, sizeof(p2));
		buf2.write(&p3, sizeof(p3));

		P2P_MANAGER::instance().Send(buf2.read_peek(), buf2.size());

		DecreaseLeftItemCount(id, count);
	}

	{
		std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT coins, silvercoins FROM %s.account WHERE id = '%d'", "account", ch->GetDesc()->GetAccountTable().id));

		MYSQL_RES *res = msg->Get()->pSQLResult;
		uint32_t goldCoins = 0;
		uint32_t silverCoins = 0;

		if ((msg->uiSQLErrno != 0 || !res || !msg->Get() || msg->Get()->uiNumRows < 1) == false)
		{
			MYSQL_ROW row = mysql_fetch_row(res);
			str_to_number(goldCoins, row[0]);
			str_to_number(silverCoins, row[1]);
		}

		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.size = sizeof(pack) + sizeof(TPacketItemShopCoins);
		pack.subHeader = IS_GC_SUB_COINS;

		TPacketItemShopCoins subpack;
		subpack.gold = goldCoins;
		subpack.silver = silverCoins;

		TEMP_BUFFER buff;
		buff.write(&pack, sizeof(pack));
		buff.write(&subpack, sizeof(subpack));
		ch->GetDesc()->Packet(buff.read_peek(), buff.size());
	}
}

void ItemShopNewManager::DecreaseLeftItemCount(uint16_t id, uint16_t count)
{
	const auto& item = std::find_if(m_Items.begin(), m_Items.end(),
		[&id](const std::pair<uint16_t, std::shared_ptr<TItemShopItem>>& element){ return element.first == id; });

	if (item == m_Items.end())
	{
		sys_err("Cant find limited item with id: %d", id);
		return;
	}

	item->second->limitedCount -= count;
}

template <class T>
bool CanDecode(T* p, int buffleft) {
	return buffleft >= (int)sizeof(T);
}

template <class T>
const char* Decode(T*& pObj, const char* data, int* pbufferLeng = nullptr, int* piBufferLeft = nullptr)
{
	pObj = (T*)data;

	if (pbufferLeng)
		*pbufferLeng += sizeof(T);

	if (piBufferLeft)
		*piBufferLeft -= sizeof(T);

	return data + sizeof(T);
}

int ItemShopNewManager::ProcessP2P(const char* c_pData, int bufferLeft)
{
	TPacketItemShopGG* ggPacket;
	bufferLeft -= sizeof(TPacketItemShopGG);
	c_pData = Decode(ggPacket, c_pData);
	int iExtra = 0;

	switch (ggPacket->subheader)
	{
	case IS_GG_RELOAD:
		{
			ItemShopNewManager::Instance().Initialize();
			break;
		}
		
	case IS_GG_BUY_LIMITED:
		{
			TPacketItemShopLimitedItem* subPacket = nullptr;
			if (CanDecode(subPacket, bufferLeft) == false)
				return -1;
			c_pData = Decode(subPacket, c_pData, &iExtra, &bufferLeft);
			ItemShopNewManager::Instance().DecreaseLeftItemCount(subPacket->id, subPacket->count);
			break;
		}
		
	default:
		break;
	}
	return iExtra;
}

int ItemShopNewManager::BuyPacket(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	
	TPacketCGItemShopBuy* pack = nullptr;
	if (!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra = 0;
	data = Decode(pack, data, &iExtra, &iBufferLeft);

	Instance().Buy(ch, pack->id, pack->count);
	return iExtra;
}

int ItemShopNewManager::RequestCountPacket(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	TPacketCGItemShopRequestCount* subpack = nullptr;
	if (!CanDecode(subpack, iBufferLeft))
		return -1;

	int iExtra = 0;
	data = Decode(subpack, data, &iExtra, &iBufferLeft);

	uint16_t id = subpack->id;

	const auto& item = std::find_if(m_Items.begin(), m_Items.end(),
		[&id](const std::pair<uint16_t, std::shared_ptr<TItemShopItem>>& element){ return element.first == id; });

	if (item != m_Items.end())
	{
		TPacketGCItemShop pack;
		pack.header = HEADER_GC_NEW_ITEMSHOP;
		pack.size = sizeof(pack) + sizeof(TPacketItemShopLimitedItem);
		pack.subHeader = IS_GC_SUB_COUNT;

		TPacketItemShopLimitedItem subpack2;
		subpack2.id = subpack->id;
		subpack2.count = item->second->limitedCount;

		TEMP_BUFFER buff;
		buff.write(&pack, sizeof(pack));
		buff.write(&subpack2, sizeof(subpack2));
		ch->GetDesc()->Packet(buff.read_peek(), buff.size());
	}

	return iExtra;
}

int ItemShopNewManager::ProcessClientPackets(const char* data, LPCHARACTER ch, int iBufferLeft)
{
	if (iBufferLeft < sizeof(TPacketCGItemShop))
		return -1;

	TPacketCGItemShop* pPack = nullptr;
	iBufferLeft -= sizeof(TPacketCGItemShop);
	data = Decode(pPack, data);

	switch (pPack->subHeader)
	{

	case IS_CG_SUB_BUY:
		return BuyPacket(ch, data, iBufferLeft);
	case IS_CG_SUB_REQUEST_COUNT:
		return RequestCountPacket(ch, data, iBufferLeft);
	default:
		sys_err("UNKNOWN SUBHEADER %d ", pPack->subHeader);
		return -1;
	}
}
