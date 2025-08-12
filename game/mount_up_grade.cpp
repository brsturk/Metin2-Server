#include "stdafx.h"

#if defined(__ENABLE_RIDING_EXTENDED__)
#	include "char.h"
#	include "desc.h"
#	include "mount_up_grade.h"

CMountUpGrade::CMountUpGrade() = default;
CMountUpGrade::~CMountUpGrade() = default;

/* Open Window */
void CMountUpGrade::OpenMountUpGrade(LPCHARACTER ch) const
{
	if (!ch)
	{
		sys_err("CMountUpGrade::OpenMountUpGrade - Unknown identifier");
		return;
	}

	const uint8_t m_level = ch->GetHorseLevel();

	/* Send to Client */
	Send(ch, EMountUpGradeGCSubheaderType::SUBHEADER_GC_MOUNT_UP_GRADE_OPEN, m_level, 0, 0);
}

void CMountUpGrade::Send(LPCHARACTER ch, const uint8_t iSubHeader, const uint8_t m_level, const uint8_t /*unused*/, const uint32_t /*unused*/) const
{
	if (!ch)
	{
		sys_err("CMountUpGrade::Send - Unknown identifier");
		return;
	}

	if (iSubHeader > SUBHEADER_GC_MOUNT_UP_GRADE_REFRESH)
	{
		sys_err("CMountUpGrade::Send - Unknown iSubHeader (Name: %s) - (iSubHeader: %d)", ch->GetName(), iSubHeader);
		return;
	}

	auto* d = ch->GetDesc();
	if (!d) return;

	TPacketGCMountUpGrade p {
		static_cast<uint8_t>(HEADER_GC_MOUNT_UP_GRADE),
		static_cast<uint8_t>(iSubHeader),
		static_cast<uint8_t>(m_level),
	};

	d->Packet(&p, sizeof(p));
}

void CMountUpGrade::Chat(LPCHARACTER ch, const uint8_t type, const uint16_t value) const
{
	if (!ch)
	{
		sys_err("CMountUpGrade::Chat - Unknown identifier");
		return;
	}

	auto* d = ch->GetDesc();
	if (!d) return;

	TPacketGCMountUpGradeChat p {
		static_cast<uint8_t>(HEADER_GC_MOUNT_UP_GRADE_CHAT),
		static_cast<uint8_t>(type),
		static_cast<uint16_t>(value),
	};

	d->Packet(&p, sizeof(p));
}

bool CMountUpGrade::Common(LPCHARACTER ch) const
{
	if (!ch)
	{
		sys_err("CMountUpGrade::Common - Unknown identifier");
		return false;
	}

	if (ch->GetHorseLevel() >= HORSE_MAX_LEVEL)
		return false;

	if (ch->IsRiding())
	{
		Chat(ch, EMountUpGradeChatType::CHAT_TYPE_BANN_WHILE_MOUNTING, EMountUpGradeChatValue::CHAT_TYPE_NO_VALUE);
		return false;
	}

	return true;
}

bool CMountUpGrade::SetLevel(LPCHARACTER ch) const
{
	if (!ch)
	{
		sys_err("CMountUpGrade::SetLevel - Unknown identifier");
		return false;
	}

	if (!Common(ch))
		return false;

	const uint8_t m_level = ch->GetHorseLevel();

	constexpr uint32_t ITEM_ID_1 = 25040;	// Kutsama kagidi
	constexpr uint32_t ITEM_ID_2 = 71084;	// Efsun nesnesi
	constexpr uint32_t ITEM_ID_3 = 71001;	// Kotu ruh kovma kagidi
	constexpr uint32_t ITEM_ID_4 = 71094;	// Munzevi tavsiyesi

	constexpr uint16_t ITEM_COUNT = 2;
	constexpr uint32_t REQUIRED_YANG = 500000;	// 500k

//	Her seviyede yukardaki itemlerden 2x adet ve 500k isteyecek sekildi ayarlandi.

	if (ch->CountSpecifyItem(ITEM_ID_1) < ITEM_COUNT ||
		ch->CountSpecifyItem(ITEM_ID_2) < ITEM_COUNT ||
		ch->CountSpecifyItem(ITEM_ID_3) < ITEM_COUNT ||
		ch->CountSpecifyItem(ITEM_ID_4) < ITEM_COUNT ||
		ch->GetGold() < REQUIRED_YANG)
	{
		Chat(ch, EMountUpGradeChatType::CHAT_TYPE_LEVEL_UP_YANG_OR_FEED_NOT_ENOUGH, EMountUpGradeChatValue::CHAT_TYPE_NO_VALUE);
		return false;
	}

	ch->RemoveSpecifyItem(ITEM_ID_1, ITEM_COUNT);
	ch->RemoveSpecifyItem(ITEM_ID_2, ITEM_COUNT);
	ch->RemoveSpecifyItem(ITEM_ID_3, ITEM_COUNT);
	ch->RemoveSpecifyItem(ITEM_ID_4, ITEM_COUNT);
	ch->PointChange(POINT_GOLD, -REQUIRED_YANG);

	ch->SetHorseLevel(m_level + 1);

	ch->ComputePoints();
	ch->SkillLevelPacket();

	Chat(ch, EMountUpGradeChatType::CHAT_TYPE_LEVEL_UP_PERCENT_SUCCESSFUL, ch->GetHorseLevel());

	Send(ch, EMountUpGradeGCSubheaderType::SUBHEADER_GC_MOUNT_UP_GRADE_REFRESH,
		ch->GetHorseLevel(), 0, 0);

	return true;
}

#endif