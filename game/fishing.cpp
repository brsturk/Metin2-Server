//#define __FISHING_MAIN__
#include "stdafx.h"
#include "constants.h"
#include "fishing.h"
#include "locale_service.h"

#ifndef __FISHING_MAIN__
#include "item_manager.h"
#include "config.h"
#include "packet.h"
#include "sectree_manager.h"
#include "char.h"
#include "char_manager.h"
#include "log.h"
#include "questmanager.h"
#include "buffer_manager.h"
#include "desc_client.h"
#include "locale_service.h"
#include "affect.h"
#include "unique_item.h"
#endif

#define ENABLE_FISHINGROD_RENEWAL
namespace fishing
{
	enum {
		MAX_FISH = 1, // Tek bal©¥k turu
		NUM_USE_RESULT_COUNT = 1, // Kullan©¥m sonucu tek esya
		FISH_BONE_VNUM = 27799, // Kullan©¥lmayacak, ama korundu
		SHELLFISH_VNUM = 27987,
		EARTHWORM_VNUM = 27801,
		WATER_STONE_VNUM_BEGIN = 28030,
		WATER_STONE_VNUM_END = 28043,
		FISH_NAME_MAX_LEN = 64,
		MAX_PROB = 1, // Tek olas©¥l©¥k tablosu
	};

	enum {
		USED_NONE, // Basar©¥s©¥zl©¥k durumunda hicbir sey dusmez
		MAX_USED_FISH
	};

	enum {
		FISHING_TIME_NORMAL,
		FISHING_TIME_COUNT,
		MAX_FISHING_TIME_COUNT = 1, // Tek zaman dilimi
	};

	int aFishingTime[FISHING_TIME_COUNT][MAX_FISHING_TIME_COUNT] = {
		{ 40 } // %40 basar©¥ sans©¥
	};

	struct SFishInfo {
		char name[FISH_NAME_MAX_LEN];
		DWORD vnum;
		DWORD dead_vnum;
		DWORD grill_vnum;
		int prob[MAX_PROB];
		int difficulty;
		int time_type;
		int length_range[3]; // MIN MAX EXTRA_MAX
		int used_table[NUM_USE_RESULT_COUNT];
	};

	SFishInfo fish_info[MAX_FISH] = {
		{ "Bal©¥k", 27803, 27803, 27803, {100}, 1, FISHING_TIME_NORMAL, {50, 100, 150}, {USED_NONE} }
	};

	int g_prob_sum[MAX_PROB] = {100}; // Tek bal©¥k, %100 olas©¥l©¥k
	int g_prob_accumulate[MAX_PROB][MAX_FISH] = {{100}};

	void Initialize() {
		sys_log(0, "FISH: %-24s vnum %5lu prob %4d len %d %d %d",
				fish_info[0].name,
				fish_info[0].vnum,
				fish_info[0].prob[0],
				fish_info[0].length_range[0],
				fish_info[0].length_range[1],
				fish_info[0].length_range[2]);
	}

	int DetermineFish(LPCHARACTER ch) {
		return 0; // Her zaman tek bal©¥k (index 0)
	}

	void FishingReact(LPCHARACTER ch) {
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_REACT;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingSuccess(LPCHARACTER ch) {
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_SUCCESS;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingFail(LPCHARACTER ch) {
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_FAIL;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingPractice(LPCHARACTER ch) {
		LPITEM rod = ch->GetWear(WEAR_WEAPON);
		if (rod && rod->GetType() == ITEM_ROD) {
			if (rod->GetRefinedVnum() > 0 && rod->GetSocket(0) < rod->GetValue(2) && number(1, rod->GetValue(1)) == 1) {
				rod->SetSocket(0, rod->GetSocket(0) + 1);
				ch->ChatPacket(CHAT_TYPE_INFO, "[LS;796;%d;%d]", rod->GetSocket(0), rod->GetValue(2));
				if (rod->GetSocket(0) == rod->GetValue(2)) {
					ch->ChatPacket(CHAT_TYPE_INFO, "[LS;797]");
					ch->ChatPacket(CHAT_TYPE_INFO, "[LS;798]");
				}
			}
			rod->SetSocket(2, 0);
		}
	}

	bool PredictFish(LPCHARACTER ch) {
		return false; // Tahmin ozelligi kald©¥r©¥ld©¥
	}

	EVENTFUNC(fishing_event) {
		fishing_event_info * info = dynamic_cast<fishing_event_info *>(event->info);
		if (!info) {
			sys_err("fishing_event> <Factor> Null pointer");
			return 0;
		}

		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->pid);
		if (!ch) return 0;

		LPITEM rod = ch->GetWear(WEAR_WEAPON);
		if (!(rod && rod->GetType() == ITEM_ROD)) {
			ch->m_pkFishingEvent = NULL;
			return 0;
		}

		switch (info->step) {
			case 0:
				++info->step;
				info->hang_time = get_dword_time();
				info->fish_id = DetermineFish(ch);
				FishingReact(ch);
				return PASSES_PER_SEC(6);

			default:
				++info->step;
				if (info->step > 5) info->step = 5;
				ch->m_pkFishingEvent = NULL;
				FishingFail(ch);
				rod->SetSocket(2, 0);
				return 0;
		}
	}

	LPEVENT CreateFishingEvent(LPCHARACTER ch) {
		fishing_event_info* info = AllocEventInfo<fishing_event_info>();
		info->pid = ch->GetPlayerID();
		info->step = 0;
		info->hang_time = 0;

		int time = number(10, 40);
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_START;
		p.info = ch->GetVID();
		p.dir = (BYTE)(ch->GetRotation() / 5);
		ch->PacketAround(&p, sizeof(TPacketGCFishing));

		return event_create(fishing_event, info, PASSES_PER_SEC(time));
	}

	int GetFishingLevel(LPCHARACTER ch) {
		LPITEM rod = ch->GetWear(WEAR_WEAPON);
		if (!rod || rod->GetType() != ITEM_ROD) return 0;
		return rod->GetSocket(2) + rod->GetValue(0);
	}

	int Compute(DWORD fish_id, DWORD ms, DWORD* item, int level) {
		if (fish_id >= MAX_FISH) {
			sys_err("Wrong FISH ID : %d", fish_id);
			return -2;
		}

		if (number(1, 100) <= aFishingTime[fish_info[fish_id].time_type][0]) {
			if (number(1, fish_info[fish_id].difficulty) <= level) {
				*item = fish_info[fish_id].vnum;
				return 0; // Basar©¥l©¥
			}
			return -3; // Seviye yetersiz
		}
		return -1; // Basar©¥s©¥z
	}

	int GetFishLength(int fish_id) {
		return number(50, 100); // 50-100 cm aras©¥nda rastgele uzunluk
	}

	void Take(fishing_event_info* info, LPCHARACTER ch) {
		if (info->step == 1) {
			DWORD item_vnum = 0;
			int ret = Compute(info->fish_id, 0, &item_vnum, GetFishingLevel(ch));

			switch (ret) {
				case -2:
				case -3:
				case -1:
					FishingFail(ch);
					LogManager::instance().FishLog(ch->GetPlayerID(), 0, info->fish_id, GetFishingLevel(ch), 0);
					break;

				case 0:
					if (item_vnum) {
						FishingSuccess(ch);
						TPacketGCFishing p;
						p.header = HEADER_GC_FISHING;
						p.subheader = FISHING_SUBHEADER_GC_FISH;
						p.info = item_vnum;
						ch->GetDesc()->Packet(&p, sizeof(TPacketGCFishing));

						LPITEM item = ch->AutoGiveItem(item_vnum, 1, -1, false);
						if (item) {
							item->SetSocket(0, GetFishLength(info->fish_id));
							ch->ChatPacket(CHAT_TYPE_INFO, "[LS;800;%d]", item->GetSocket(0)/100.f);
						}
						LogManager::instance().FishLog(ch->GetPlayerID(), 0, info->fish_id, GetFishingLevel(ch), 0, true, item ? item->GetSocket(0) : 0);
					} else {
						FishingFail(ch);
						LogManager::instance().FishLog(ch->GetPlayerID(), 0, info->fish_id, GetFishingLevel(ch), 0);
					}
					break;
			}
		} else {
			FishingFail(ch);
			TPacketGCFishing p;
			p.header = HEADER_GC_FISHING;
			p.subheader = FISHING_SUBHEADER_GC_STOP;
			p.info = ch->GetVID();
			ch->PacketAround(&p, sizeof(p));
		}

		if (info->step) {
			FishingPractice(ch);
		}
	}

	void UseFish(LPCHARACTER ch, LPITEM item) {
		if (item->GetVnum() != fish_info[0].vnum) return;
		item->SetCount(item->GetCount() - 1);
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;807]");
	}

	void Grill(LPCHARACTER ch, LPITEM item) {
		if (item->GetVnum() != fish_info[0].vnum) return;
		int count = item->GetCount();
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2092]");
		item->SetCount(0);
		ch->AutoGiveItem(fish_info[0].grill_vnum, count);
	}

	bool RefinableRod(LPITEM rod) {
		if (rod->GetType() != ITEM_ROD) return false;
		if (rod->IsEquipped()) return false;
		return (rod->GetSocket(0) == rod->GetValue(2));
	}

	int RealRefineRod(LPCHARACTER ch, LPITEM item) {
		if (!ch || !item) return 2;
		if (!RefinableRod(item)) {
			sys_err("REFINE_ROD_HACK pid(%u) item(%s:%d)", ch->GetPlayerID(), item->GetName(), item->GetID());
			LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), -1, 1, "ROD_HACK");
			return 2;
		}

		LPITEM rod = item;
		int iAdv = rod->GetValue(0) / 10;

		if (number(1, 100) <= rod->GetValue(3)) {
			LogManager::instance().RefineLog(ch->GetPlayerID(), rod->GetName(), rod->GetID(), iAdv, 1, "ROD");
			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(rod->GetRefinedVnum(), 1);
			if (pkNewItem) {
				BYTE bCell = rod->GetCell();
				ITEM_MANAGER::instance().RemoveItem(rod, "REMOVE (REFINE FISH_ROD)");
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, bCell));
				LogManager::instance().ItemLog(ch, pkNewItem, "REFINE FISH_ROD SUCCESS", pkNewItem->GetName());
				return 1;
			}
			return 2;
		} else {
			LogManager::instance().RefineLog(ch->GetPlayerID(), rod->GetName(), rod->GetID(), iAdv, 0, "ROD");
#ifdef ENABLE_FISHINGROD_RENEWAL
			int cur = rod->GetSocket(0);
			rod->SetSocket(0, (cur > 0) ? (cur - (cur * 10 / 100)) : 0);
			LogManager::instance().ItemLog(ch, rod, "REFINE FISH_ROD FAIL", rod->GetName());
			return 0;
#else
			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(rod->GetValue(4), 1);
			if (pkNewItem) {
				BYTE bCell = rod->GetCell();
				ITEM_MANAGER::instance().RemoveItem(rod, "REMOVE (REFINE FISH_ROD)");
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, bCell));
				LogManager::instance().ItemLog(ch, pkNewItem, "REFINE FISH_ROD FAIL", pkNewItem->GetName());
				return 0;
			}
#endif
			return 2;
		}
	}
}

#ifdef __FISHING_MAIN__
int main(int argc, char **argv) {
	using namespace fishing;
	Initialize();
	printf("%s\t%u\t%u\t%u\t%d\t%d\t%d\t%d\n",
		   fish_info[0].name,
		   fish_info[0].vnum,
		   fish_info[0].dead_vnum,
		   fish_info[0].grill_vnum,
		   fish_info[0].prob[0],
		   fish_info[0].difficulty,
		   fish_info[0].time_type,
		   fish_info[0].length_range[0]);
	return 1;
}
#endif
