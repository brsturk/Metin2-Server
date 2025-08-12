#ifndef __INC_NEW_ItemShopManager_H__
#define __INC_NEW_ItemShopManager_H__


//#include "../../common/stl.h"
//#include "../../common/length.h"
#include "../../common/tables.h"
#include "packet.h"
//#include "db.h"

#include <unordered_map>
#include <map>
#include "buffer_manager.h"
#include "char.h"

class ItemShopNewManager : public singleton<ItemShopNewManager>
{
public:
	ItemShopNewManager();
	~ItemShopNewManager();

	void Initialize();

	void Open(LPCHARACTER ch);
	uint32_t GetCoins(LPCHARACTER ch, uint8_t type);
	bool ChangeCoins(LPCHARACTER ch, uint32_t coins, uint8_t type);

	void Buy(LPCHARACTER ch, uint16_t id, uint16_t count);
	void DecreaseLeftItemCount(uint16_t id, uint16_t count);

	int ProcessP2P(const char * c_pData, int bufferLeft);
	int BuyPacket(LPCHARACTER ch, const char* data, int i_buffer_left);
	int RequestCountPacket(LPCHARACTER ch, const char* data, int i_buffer_left);
	int ProcessClientPackets(const char* c_pData, LPCHARACTER ch, int bufferLeft);

protected:
	std::vector<std::pair<uint16_t, std::shared_ptr<TItemShopItem>>> m_Items;
	std::map<uint16_t, std::shared_ptr<TItemShopCategory>> m_Categories;
};


#endif

