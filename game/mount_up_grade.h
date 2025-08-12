#pragma once

#include "../../common/CommonDefines.h"

#if defined(__ENABLE_RIDING_EXTENDED__)
#include "horse_rider.h"

class CMountUpGrade : public singleton<CMountUpGrade>
{
public:
	CMountUpGrade();
	~CMountUpGrade();

	void OpenMountUpGrade(LPCHARACTER ch) const;

	bool SetLevel(LPCHARACTER ch) const;
	bool Common(LPCHARACTER ch) const;

	void Chat(LPCHARACTER ch, const uint8_t type, const uint16_t value) const;
	void Send(LPCHARACTER ch, const uint8_t iSubHeader, const uint8_t m_level, const uint8_t unused1, const uint32_t unused2) const;

protected:
	enum EMountUpGradeChatType : uint8_t
	{
		CHAT_TYPE_BANN_WHILE_MOUNTING,
		CHAT_TYPE_LEVEL_UP_YANG_OR_FEED_NOT_ENOUGH,
		CHAT_TYPE_LEVEL_UP_PERCENT_SUCCESSFUL,

		CHAT_TYPE_MAX
	};

	enum EMountUpGradeChatValue : uint8_t
	{
		CHAT_TYPE_NO_VALUE
	};

	enum EMountUpGradeItem : uint32_t
	{
		ITEM_1_ID = 25040,	// Kutsama kagidi
		ITEM_2_ID = 71084,	// Efsun nesnesi
		ITEM_3_ID = 71001,	// Kotu ruh kovma kagidi
		ITEM_4_ID = 71094,	// Munzevi tavsiyesi
		ITEM_COUNT_PER_LEVEL = 2
	};

	constexpr static uint32_t REQUIRED_YANG = 500000;

	enum EMountUpGradeGCSubheaderType : uint8_t
	{
		SUBHEADER_GC_MOUNT_UP_GRADE_OPEN,
		SUBHEADER_GC_MOUNT_UP_GRADE_REFRESH
	};

public:
	enum EMountUpGradeCGSubheaderType : uint8_t
	{
		SUBHEADER_CG_MOUNT_UP_GRADE_LEVEL_UP
	};
};

#endif