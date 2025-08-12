#include "stdafx.h"

#ifdef ENABLE_NEW_BIOLOG
#include "NewBiolog.h"
#include "affect.h"
#include "buffer_manager.h"
#include "char.h"
#include "desc.h" 
#include "item.h" 
#include "utils.h"
#include "config.h"
#include "db.h" 

EVENTINFO(biolog_ntf_info)
{
	DynamicCharacterPtr player;

	biolog_ntf_info()
	: player()
	{
	}
};

EVENTFUNC(biolog_ntf_event)
{
	const auto pInfo = dynamic_cast<biolog_ntf_info*>(event->info);
	if (pInfo == NULL)
	{
		sys_err("biolog_ntf_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = pInfo->player;
 
	if (!ch || !ch->GetDesc()) { return 0; }

	if (ch->GetBioNtf() == 1 && ch->GetBioTime() <= get_global_time())
	{
		char buf[52];
		int len = snprintf(buf, sizeof(buf), "[LS;2004]");

		TPacketGCWhisper pack;
		pack.bHeader = HEADER_GC_WHISPER;
		pack.bType = WHISPER_TYPE_SYSTEM;
		pack.wSize = sizeof(TPacketGCWhisper) + len;
		strlcpy(pack.szNameFrom, "[BIOLOG]", sizeof(pack.szNameFrom));
		ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
		ch->GetDesc()->Packet(buf, len);
		// ch->CloseBiologNtf();
		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CHARACTER::OpenBiologNtf()
{
	CloseBiologNtf();

	auto* pInfo = AllocEventInfo<biolog_ntf_info>();
	pInfo->player = this;

	m_pkBiologNtfEvent = event_create(biolog_ntf_event, pInfo, 1);
}

void CHARACTER::CloseBiologNtf()
{
	if (m_pkBiologNtfEvent)
	{
		event_cancel(&m_pkBiologNtfEvent);
		m_pkBiologNtfEvent = nullptr;
	}
}

auto CNewBiologManager::Initialize() -> bool {
	const std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT * FROM player.biolog_info%s", get_table_postfix()));
	if (pMsg->Get()->uiNumRows==0) {
		sys_err("Biolog tablosu bos!");
		return false;
	}
	MYSQL_ROW	row;
	BYTE mission = 0;
	while (nullptr != (row = mysql_fetch_row(pMsg->Get()->pSQLResult))) {
		DWORD col = 0;
		tBioInfo   info = {};
		str_to_number(info.dwMobVnum, row[col++]); //idx olarak güncelledi.
		str_to_number(info.bLevel, row[col++]);

		for (BYTE i(0); i < BIOLOG_MAX_AFF; ++i) {
			BYTE aff_type = 0; WORD aff_val = 0;
			str_to_number(aff_type, row[col++]);
			str_to_number(aff_val, row[col++]);
			if (aff_type == 0) { continue; }
			info.aff_map.emplace_back(aff_type, aff_val);
		}

		std::sort(info.aff_map.begin(), info.aff_map.end(), [](const std::pair<BYTE, WORD>& lhs, const std::pair<BYTE, WORD>& rhs){
			return lhs.first < rhs.first;
		});

		str_to_number(info.dwItemVnum, row[col++]);
		str_to_number(info.dwSoulVnum, row[col++]);
		str_to_number(info.bReqCount, row[col++]);
		str_to_number(info.bMin, row[col++]);
		str_to_number(info.bChance, row[col++]);
		m_BioInfoMap.emplace(mission, info);
		++mission;
	}

	return true;
}

auto CNewBiologManager::SendToClient(const LPCHARACTER ch) const -> void {
	if (!ch || !ch->GetDesc() || m_BioInfoMap.empty()) { return; }

	const auto& it = m_BioInfoMap.find(ch->GetBioLevel());
	if (it == m_BioInfoMap.cend()) { return; }
	tBiologGCInformation pInfo = {};
	pInfo.bHeader = HEADER_GC_BIOLOG_UPDATE;
	pInfo.dwItemVnum = it->second.dwItemVnum;
	pInfo.dwSoulVnum = it->second.dwSoulVnum;
	pInfo.bGivenCount = ch->GetBioGivenCount();
	pInfo.bState = ch->GetBioState();
	pInfo.bRequiredCount = it->second.bReqCount;
	pInfo.bChance = it->second.bChance;
	pInfo.iTime = ch->GetBioTime() - get_global_time();

	BYTE idx = 0;
	for (const auto& elm : it->second.aff_map)
	{
		pInfo.bAffect[idx] = elm.first;
		pInfo.bAffValue[idx] = elm.second;
		++idx;
	}

	TEMP_BUFFER buf;
	buf.write(&pInfo, sizeof(pInfo));

	if (buf.size() <= 0) { return; }
	ch->GetDesc()->Packet(buf.read_peek(), buf.size());
}

auto CNewBiologManager::EndBiolog(LPCHARACTER ch, const BYTE idx, const bool isItem) -> bool {
	if (!ch || !ch->GetDesc()) { return false; }
	// if (idx ==  99) {
	// -> idx 1 = 92 sonrasi idx 2 = 92 oncesi
	if (isItem) {
		if (idx == 1 && ch->GetBioLevel() >= 7) {
			ch->ChatPacket(1, "[LS;2006]");
			return false;
		}

		if (idx == 0 && ch->GetBioLevel() < 7) {
			ch->ChatPacket(1, "[LS;2006]");
			return false;
		}
	}
	const auto& it = m_BioInfoMap.find(idx);
	if (it != m_BioInfoMap.cend()) {
		for (BYTE i = 0; i < sizeof(it->second.aff_type) / sizeof(*it->second.aff_type); ++i)
		{
			if (it->second.aff_type[i] == 0) { continue;  }
			ch->ChatPacket(1, "2 - eklenen degerler : aff : %d - val : %d - idx : %d", it->second.aff_type[i], it->second.aff_value[i], AFFECT_BIOLOG_START_IDX + ch->GetBioLevel());
			ch->AddAffect(AFFECT_BIOLOG_START_IDX + ch->GetBioLevel(), aApplyInfo[it->second.aff_type[i]].bPointType, it->second.aff_value[i], 0, INFINITE_AFFECT_DURATION, 0, false);
		}
		ch->SetBioTime(0);
		ch->SetBioGivenCount(0);
		ch->SetBioLevel(ch->GetBioLevel() + 1); // yeni gorev
		SendToClient(ch);
		ch->SetBioState(GIVE_STATE);
		ch->ChatPacket(1, "[LS;2007]");
	}
	return true;
}

// 92 ve 94 indexler 8 - 9
auto CNewBiologManager::GiveBiolog(LPCHARACTER ch, const bool isTimeItem, const bool isExtractItem) const -> void {
	if (!ch || !ch->GetDesc() || m_BioInfoMap.empty()) { return; }
	const auto& it = m_BioInfoMap.find(ch->GetBioLevel());
	if (it == m_BioInfoMap.cend()) { return; }
	BYTE prob = number(1,100);
	const auto state = ch->GetBioState();


	if (state == GIVE_STATE)
	{
	// kontroller //
		if (ch->GetLevel() < it->second.bLevel) {
			ch->ChatPacket(1, "[LS;2008]");
			return;
		}

		if (ch->CountSpecifyItem(it->second.dwItemVnum) < 1) {
			ch->ChatPacket(1, "[LS;2009]");
			return;
		}

		if (isTimeItem) {
			if (ch->CountSpecifyItem(TIME_ITEM_VNUM) < 1) {
				ch->ChatPacket(1, "[LS;2011]");
				return;
			}
			ch->SetBioTime(0);
			ch->RemoveSpecifyItem(TIME_ITEM_VNUM, 1);
		}

		if (isExtractItem) {// arastirma ozutu %100 gecme
			if (ch->CountSpecifyItem(EXTRACT_ITEM_VNUM) < 1) {
				ch->ChatPacket(1, "[LS;2012]");
				return;
			}
			 prob = 0;
			 ch->RemoveSpecifyItem(EXTRACT_ITEM_VNUM, 1);
		}

		if (ch->GetBioTime() > get_global_time()) {
			ch->ChatPacket(1, "[LS;2013]");
			return;
		}
	// kontroller //

		if (prob <= it->second.bChance) {
			ch->SetBioGivenCount(ch->GetBioGivenCount() + 1);
			ch->ChatPacket(1, "[LS;2014]");
		}
		else {
			ch->ChatPacket(1, "[LS;2015]"); 
		}
		ch->RemoveSpecifyItem(it->second.dwItemVnum);

		if (it->second.bMin > 0) {
			int bioMin = it->second.bMin;
			// if (ch->IsAffectFlag(AFF_PREMIUM_PLAYER)) {
				// bioMin /= 2; //Premium biolog
				// ch->ChatPacket(1, "<Premium> Premium oyuncu oldugunuz icin biyolog sureniz yari yariya azaltildi.");
			// }

			ch->SetBioTime(get_global_time() + bioMin *60);
		}

		if (ch->GetBioNtf() == 1) {
			ch->OpenBiologNtf();
		}

		if (ch->GetBioGivenCount() >= it->second.bReqCount) {
			if (ch->GetBioLevel() == 8)
			{
				ch->SetBioState(SELECT_STATE);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenBioSelect %d", ch->GetBioLevel()); 
				return;
			}


			if (it->second.dwSoulVnum != 0) {
				ch->SetBioState(SOUL_STATE); // ruh tasi
			}
			else {

					for (const auto& affPair : it->second.aff_map) {
						const auto& type = affPair.first; // aff_map'in anahtarı
						const auto& val = affPair.second; // aff_map'in değeri

						ch->AddAffect(AFFECT_BIOLOG_START_IDX + ch->GetBioLevel(), aApplyInfo[type].bPointType, val, 0, INFINITE_AFFECT_DURATION, 0, false);
					}

				ch->SetBioState(GIVE_STATE);
				ch->SetBioLevel(ch->GetBioLevel() + 1); // yeni gorev

				ch->ChatPacket(1, "[LS;2007]");
			}
			ch->SetBioTime(0);
			ch->SetBioGivenCount(0);
		}
	}
	else if(state == SOUL_STATE) // soul state
	{
		if (ch->CountSpecifyItem(it->second.dwSoulVnum) < 1) {
			ch->ChatPacket(1, "[LS;2009]");
			return;
		}
		ch->RemoveSpecifyItem(it->second.dwSoulVnum);
		if (ch->GetBioLevel() == 9)
		{
			ch->SetBioState(SELECT_STATE);
			ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenBioSelect %d", ch->GetBioLevel()); 
			return;
		}
		for (const auto& affPair : it->second.aff_map) {
			const auto& type = affPair.first; // aff_map'teki anahtar
			const auto& val = affPair.second; // aff_map'teki değer

			ch->AddAffect(AFFECT_BIOLOG_START_IDX + ch->GetBioLevel(), aApplyInfo[type].bPointType, val, 0, INFINITE_AFFECT_DURATION, 0, false);
		}

		ch->SetBioState(GIVE_STATE);
		ch->SetBioLevel(ch->GetBioLevel() + 1); // yeni gorev

		ch->ChatPacket(1, "[LS;2007]");

		if (ch->GetBioNtf() == 1) {
			ch->OpenBiologNtf();
		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenBioSelect %d", ch->GetBioLevel());
	}

	SendToClient(ch);
}

auto CNewBiologManager::GiveFastBiolog(LPCHARACTER ch, const bool isTimeItem, const bool isExtractItem) -> void {
	if (!ch || !ch->GetDesc() || m_BioInfoMap.empty()) { return; }

	const auto& it = m_BioInfoMap.find(ch->GetBioLevel());
	if (it == m_BioInfoMap.cend()) { return; }

	if (ch->GetBioState() != GIVE_STATE) {
		ch->ChatPacket(1, "Lutfen diger butonu kullanin.");
		return;
	}

	if (ch->GetLevel() < it->second.bLevel) {
		ch->ChatPacket(1, "[LS;2008]");
		return;
	}

	if (ch->IsBioFastGive()) {
		ch->ChatPacket(1, "Toplu verme kapatildi!");
		ch->SetBioFastGive(false);
		return;
	}

	if ((thecore_pulse() - ch->GetFastBioTime()) < PASSES_PER_SEC(5)) {
		ch->ChatPacket(1, "[LS;2016]");
		return;
	}

	ch->SetFastBioTime();

	while(true) {
		BYTE prob = number(1,100);
		bool isBreak = false;
		std::string info = "";
		if (ch->GetBioTime() > get_global_time() && !isTimeItem) {	
			info = "[LS;2013]"; 
			isBreak = true;
		}

		if (ch->CountSpecifyItem(it->second.dwItemVnum) < 1) {
			info = "[LS;2009]"; 
			isBreak = true;
		}

		if (isTimeItem) {
			if (ch->CountSpecifyItem(TIME_ITEM_VNUM) < 1) {
			info = "[LS;2017]"; 
			isBreak = true;
			}
		}

		if (isExtractItem) {// arastirma ozutu %100 gecme
			if (ch->CountSpecifyItem(EXTRACT_ITEM_VNUM) < 1) {
			info = "[LS;2018]"; 
			isBreak = true;
			}
		}

		if (isBreak) {
			ch->SetBioFastGive(false);
			ch->ChatPacket(1, info.c_str());
			return;
		}
		
	// controls end //

		if (isExtractItem) {
			 prob = 0;
			 ch->RemoveSpecifyItem(EXTRACT_ITEM_VNUM, 1);
		}

		if (isTimeItem) {
			ch->SetBioTime(get_global_time()+0);
			ch->RemoveSpecifyItem(TIME_ITEM_VNUM, 1);
		}

		if (prob <= it->second.bChance) {
			ch->SetBioGivenCount(ch->GetBioGivenCount() + 1);
			ch->ChatPacket(1, "[LS;2014]");
		}
		else {
			ch->ChatPacket(1, "[LS;2015]"); 
		}
		ch->RemoveSpecifyItem(it->second.dwItemVnum);

		if (it->second.bMin > 0) {
			ch->SetBioTime(get_global_time()+ it->second.bMin*60);
		}

		if (ch->GetBioGivenCount() >= it->second.bReqCount) {
			if (ch->GetBioLevel() == 8)
			{
				ch->SetBioState(SELECT_STATE);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenBioSelect %d", ch->GetBioLevel()); 
				return;
			}
			if (it->second.dwSoulVnum != 0) {
				ch->SetBioState(SOUL_STATE); // ruh tasi
				ch->SetBioFastGive(false);
				ch->SetBioTime(0);
				ch->SetBioGivenCount(0);
				SendToClient(ch);
				return;
			}
			else {
				EndBiolog(ch, ch->GetBioLevel());
				if (ch->GetBioLevel() < m_BioInfoMap.size()) {
					ch->ChatPacket(1, "[LS;2019]");
				}
				else { 
					CloseBiologPanel(ch);
				}
				ch->SetBioFastGive(false);
				return;
			}

			break;
		}
	}

	ch->SetBioFastGive(true);
	SendToClient(ch);
	if (ch->GetBioNtf() == 1) {
		ch->OpenBiologNtf();
	}
}

auto CNewBiologManager::SelectSpecial(LPCHARACTER ch, const BYTE bSelect) const -> void
{
	if (!ch || !ch->GetDesc()) { return; }
	if (ch->GetBioState() != SELECT_STATE) { return;  }
	const auto& it = m_BioInfoMap.find(ch->GetBioLevel());
	if (it == m_BioInfoMap.cend()) { return; }

	ch->SetBioTime(0);
	ch->SetBioGivenCount(0);
	if (ch->GetBioLevel() == 8) {
		ch->SetBioState(GIVE_STATE);
	}else {
		ch->SetBioState(SOUL_STATE);
	}

	// ch->ChatPacket(1,"select : %d - pointtype : %d - val : %d", bSelect, aApplyInfo[it->second.aff_type[bSelect]].bPointType, it->second.aff_value[bSelect]);
	// ch->ChatPacket(1, "Gorev tamamlandi, ozellikler karaktere islendi.");
	if (bSelect >= it->second.aff_map.size()) {
		sys_err("napar bu? %s", ch->GetName());
		return;
		
	}
	const auto& info = it->second.aff_map[bSelect];
	ch->AddAffect(AFFECT_BIOLOG_START_IDX + ch->GetBioLevel(), aApplyInfo[info.first].bPointType, info.second, 0, INFINITE_AFFECT_DURATION, 0, false);

	ch->SetBioLevel(ch->GetBioLevel() + 1);
	if (ch->GetBioLevel() >= m_BioInfoMap.size()) {
		// CloseBiologPanel(ch);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseBiologWindow");
	}

	SendToClient(ch);
}

auto CNewBiologManager::OpenBiologPanel(LPCHARACTER ch) -> void {
	if (ch->GetBioLevel() < m_BioInfoMap.size()) {
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenBioPanel");
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ReminderStatus %u", ch->GetBioNtf());
	}
	else
		ch->ChatPacket(1, "[LS;2020]");
}

auto CNewBiologManager::CloseBiologPanel(LPCHARACTER ch) -> void {
	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseBiologWindow");
}

#endif