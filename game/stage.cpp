#include "stdafx.h"
#include "stage.h"
#include "char.h"
#include "item.h"
#include "affect.h"
#include "packet.h"
#include "desc.h"
#include "event.h"
#include "config.h"
#include "char_manager.h"

namespace {
	void SendStageGUI(LPCHARACTER ch, bool showChat = false)
	{
		if (!ch || !ch->GetDesc())
			return;

		TPacketGCStageSystem p;
		p.header = HEADER_GC_STAGE_SYSTEM;
		p.stage_level = ch->GetStageLevel();
		p.countdown_timer = static_cast<int>(ch->GetCountdownTimer());

		ch->GetDesc()->Packet(&p, sizeof(p));

		if (showChat)
			ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2105;%d]", ch->GetStageLevel());
	}
}

void CStageSystem::Upgrade(LPCHARACTER ch, BYTE item_pos)
{
	if (!ch)
		return;

	LPITEM item = ch->GetInventoryItem(item_pos);
	if (!item || item->GetVnum() != 84021)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2101]"); // Geçersiz item
		return;
	}

	if (ch->GetStageLevel() >= STAGE_MAX_LEVEL)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2102]"); // Maksimum seviye
		return;
	}

	ch->SetStageLevel(ch->GetStageLevel() + 1);
	ch->SetCountdownTimer(time(0) + 10); // Test için 10 saniye, normalde 86400

	ApplyStageAffects(ch);
	item->SetCount(item->GetCount() - 1);
	ch->Save();

	ch->ChatPacket(CHAT_TYPE_INFO, "[LS;2103;%d]", ch->GetStageLevel());

	SendStageGUI(ch, true);
}

void CStageSystem::CheckStage(LPCHARACTER ch)
{
	if (!ch)
		return;

	int currentLevel = ch->GetStageLevel();
	if (currentLevel <= 0)
		return;

	time_t now = time(nullptr);
	time_t countdown = ch->GetCountdownTimer();
	if (countdown == 0)
		return;

	const time_t PERIOD = 10; // test için 10sn, gerçek ortamda 86400 (24 saat)

	// Süre dolmamışsa
	if (now < countdown)
		return;

	// Geçen süreye göre kaç aşama düşmesi gerektiğini hesapla
	time_t diff = now - countdown;
	int stepsToDrop = 1 + static_cast<int>(diff / PERIOD);

	int newLevel = currentLevel - stepsToDrop;
	if (newLevel < 0)
		newLevel = 0;

	// Yeni süreyi ayarla
	if (newLevel > 0)
		ch->SetCountdownTimer(now + PERIOD);
	else
		ch->SetCountdownTimer(0);

	// Seviyeyi güncelle
	ch->SetStageLevel(newLevel);

	// Affectleri güncelle
	ApplyStageAffects(ch);

	// Client'e gönder
	SendStageGUI(ch, true);
}

void CStageSystem::ApplyStageAffects(LPCHARACTER ch)
{
	if (!ch)
		return;

	int lvl = ch->GetStageLevel();

	// Önce mevcut affectleri temizle
	ch->RemoveAffect(AFFECT_STAGE_MONSTER_BONUS);
	ch->RemoveAffect(AFFECT_STAGE_EXTRA_HP);
	ch->RemoveAffect(AFFECT_STAGE_EXTRA_EXP);

	if (lvl <= 0)
		return;

	// Her aşamada canavarlara karşı güç
	ch->AddAffect(AFFECT_STAGE_MONSTER_BONUS, POINT_ATTBONUS_MONSTER, lvl, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);

	// 5. seviyeden itibaren +1000 HP
	if (lvl >= 5)
		ch->AddAffect(AFFECT_STAGE_EXTRA_HP, POINT_MAX_HP, 1000, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);

	// 10. seviyede +%10 EXP
	if (lvl == 10)
		ch->AddAffect(AFFECT_STAGE_EXTRA_EXP, POINT_EXP_DOUBLE_BONUS, 10, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
}