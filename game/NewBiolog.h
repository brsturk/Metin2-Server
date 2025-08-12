#ifdef ENABLE_NEW_BIOLOG

#include <unordered_map>

constexpr uint8_t BIOLOG_MAX_AFF = 4;
class CNewBiologManager final : public singleton<CNewBiologManager>
{
	enum eInfos : DWORD {
		TIME_ITEM_VNUM = 70022, // beranin kalbi
		EXTRACT_ITEM_VNUM = 71035, // ozut
	};

	enum eState : BYTE {
		GIVE_STATE = 0,
		SOUL_STATE = 1,
		SELECT_STATE = 2,
	};

	using tBioInfo = struct sBioMissions 
	{
		DWORD   dwMobVnum; // dusecek boss vnum deaktif
		BYTE	bLevel; // level
		DWORD   dwItemVnum; // istenen item
		DWORD   dwSoulVnum; // ruh tasi vnum
		BYTE	bReqCount; // istenen adet
		BYTE	aff_type[4];
		WORD	aff_value[4]; // affect deger
		BYTE	bMin; // bekleme suresi
		BYTE	bChance; // gecme sansi
		std::vector<std::pair<BYTE, WORD>> aff_map;
	};
	using BIO_MAP = std::unordered_map<BYTE, tBioInfo>;
public: 
	auto	Initialize() -> bool;
	auto	SendToClient(LPCHARACTER ch) const -> void;
	auto	GiveBiolog(LPCHARACTER ch, bool isTimeItem, bool isExtractItem) const -> void;
	auto	GiveFastBiolog(LPCHARACTER ch, bool isTimeItem, bool isExtractItem) -> void;
	auto	SelectSpecial(LPCHARACTER ch, BYTE bSelect) const -> void;
	auto	EndBiolog(LPCHARACTER ch, BYTE idx, bool isItem = false) -> bool;
	auto	CheckBiolog(LPCHARACTER ch) const -> bool;
	auto	AddMeNotification(DWORD dwID) -> void;
	auto	OpenBiologPanel(LPCHARACTER ch) -> void;
	auto	CloseBiologPanel(LPCHARACTER ch) -> void;
	auto	GetMissionSize() const -> DWORD { return m_BioInfoMap.size(); }
private:
	BIO_MAP	 m_BioInfoMap;
	LPEVENT	 nftTimer;
};
#endif