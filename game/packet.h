#ifndef __INC_PACKET_H__
#define __INC_PACKET_H__
#include "stdafx.h"

enum
{
	HEADER_CG_HANDSHAKE				= 0xff,
	HEADER_CG_PONG					= 0xfe,
	HEADER_CG_TIME_SYNC				= 0xfc,
	HEADER_CG_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_

	HEADER_CG_LOGIN					= 1,
	HEADER_CG_ATTACK				= 2,
	HEADER_CG_CHAT					= 3,
	HEADER_CG_CHARACTER_CREATE		= 4,
	HEADER_CG_CHARACTER_DELETE		= 5,
	HEADER_CG_CHARACTER_SELECT		= 6,
	HEADER_CG_MOVE					= 7,
	HEADER_CG_SYNC_POSITION			= 8,
	HEADER_CG_ENTERGAME				= 10,

	HEADER_CG_ITEM_USE				= 11,
	HEADER_CG_ITEM_DROP				= 12,
	HEADER_CG_ITEM_MOVE				= 13,
	HEADER_CG_ITEM_PICKUP			= 15,

	HEADER_CG_QUICKSLOT_ADD			= 16,
	HEADER_CG_QUICKSLOT_DEL			= 17,
	HEADER_CG_QUICKSLOT_SWAP		= 18,
	HEADER_CG_WHISPER				= 19,
	HEADER_CG_ITEM_DROP2			= 20,
#ifdef WJ_NEW_DROP_DIALOG
	HEADER_CG_ITEM_DESTROY			= 21,
	HEADER_CG_ITEM_SELL				= 22,
#endif
	HEADER_CG_ON_CLICK				= 26,
	HEADER_CG_EXCHANGE				= 27,
	HEADER_CG_CHARACTER_POSITION	= 28,
	HEADER_CG_SCRIPT_ANSWER			= 29,
	HEADER_CG_QUEST_INPUT_STRING	= 30,
	HEADER_CG_QUEST_CONFIRM			= 31,

	HEADER_CG_SHOP					= 50,
	HEADER_CG_FLY_TARGETING			= 51,
	HEADER_CG_USE_SKILL				= 52,
	HEADER_CG_ADD_FLY_TARGETING		= 53,
	HEADER_CG_SHOOT					= 54,
	HEADER_CG_MYSHOP				= 55,

	HEADER_CG_ITEM_USE_TO_ITEM		= 60,
	HEADER_CG_TARGET			 	= 61,

	HEADER_CG_TEXT					= 64,
	HEADER_CG_WARP					= 65,
	HEADER_CG_SCRIPT_BUTTON			= 66,
	HEADER_CG_MESSENGER				= 67,

	HEADER_CG_MALL_CHECKOUT			= 69,
	HEADER_CG_SAFEBOX_CHECKIN		= 70,
	HEADER_CG_SAFEBOX_CHECKOUT		= 71,

	HEADER_CG_PARTY_INVITE			= 72,
	HEADER_CG_PARTY_INVITE_ANSWER	= 73,
	HEADER_CG_PARTY_REMOVE			= 74,
	HEADER_CG_PARTY_SET_STATE		= 75,
	HEADER_CG_PARTY_USE_SKILL		= 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE		= 77,
	HEADER_CG_PARTY_PARAMETER		= 78,

	HEADER_CG_GUILD					= 80,
	HEADER_CG_ANSWER_MAKE_GUILD		= 81,

	HEADER_CG_FISHING				= 82,

	HEADER_CG_ITEM_GIVE				= 83,
#ifdef ENABLE_INGAME_ITEMSHOP
	HEADER_CG_ITEMSHOP_NEW			= 87,
#endif
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_CG_TARGET_INFO_LOAD		= 89,
#endif

	HEADER_CG_EMPIRE				= 90,

	HEADER_CG_REFINE				= 96,

	HEADER_CG_MARK_LOGIN			= 100,
	HEADER_CG_MARK_CRCLIST			= 101,
	HEADER_CG_MARK_UPLOAD			= 102,
	HEADER_CG_MARK_IDXLIST			= 104,

	HEADER_CG_HACK					= 105,
	HEADER_CG_CHANGE_NAME			= 106,
	HEADER_CG_LOGIN2				= 109,
	HEADER_CG_DUNGEON				= 110,
	HEADER_CG_LOGIN3				= 111,

	HEADER_CG_GUILD_SYMBOL_UPLOAD	= 112,
	HEADER_CG_SYMBOL_CRC			= 113,

	// SCRIPT_SELECT_ITEM
	HEADER_CG_SCRIPT_SELECT_ITEM	= 114,
	// END_OF_SCRIPT_SELECT_ITEM
#if defined(__ENABLE_RIDING_EXTENDED__)
	HEADER_CG_MOUNT_UP_GRADE		= 119,
#endif
#ifdef __ENABLE_STAGE_SYSTEM__
	HEADER_CG_STAGE_UPGRADE			= 120,
#endif

#ifdef ENABLE_DUNGEON_INFO_SYSTEM	
	HEADER_CG_DUNGEON_INFO			= 159,
#endif

	HEADER_CG_DRAGON_SOUL_REFINE	= 205,
	HEADER_CG_STATE_CHECKER			= 206,
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL			= 207,
#endif
#ifdef ENABLE_NEW_RANKING_SYSTEM
	HEADER_CG_NEW_RANKING			= 216,
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
	HEADER_CG_NEW_OFFLINESHOP		= 221,
#endif
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	HEADER_CG_ANTI_FARM				= 227,
#endif
#ifdef ENABLE_CAPTCHA
	HEADER_CG_CAPTCHA				= 237,
#endif
#ifdef ENABLE_CSHIELD
	HEADER_CG_CSHIELD				= 238,
	HEADER_CG_CSHIELD_DATA			= 239, // this packet needs to have header 239
#ifdef ENABLE_EXTERNAL_REPORTS
	HEADER_CG_CSHIELD_REPORT		= 240, // this packet needs to have header 240
#endif
#endif

	HEADER_CG_CLIENT_VERSION2		= 0xf1,

	/********************************************************/
	HEADER_GC_KEY_AGREEMENT_COMPLETED			= 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_TIME_SYNC							= 0xfc,
	HEADER_GC_PHASE								= 0xfd,
	HEADER_GC_BINDUDP							= 0xfe,
	HEADER_GC_HANDSHAKE							= 0xff,

	HEADER_GC_CHARACTER_ADD						= 1,
	HEADER_GC_CHARACTER_DEL						= 2,
	HEADER_GC_MOVE								= 3,
	HEADER_GC_CHAT								= 4,
	HEADER_GC_SYNC_POSITION						= 5,

	HEADER_GC_LOGIN_SUCCESS						= 6,
	HEADER_GC_LOGIN_SUCCESS_NEWSLOT				= 32,
	HEADER_GC_LOGIN_FAILURE						= 7,

	HEADER_GC_CHARACTER_CREATE_SUCCESS			= 8,
	HEADER_GC_CHARACTER_CREATE_FAILURE			= 9,
	HEADER_GC_CHARACTER_DELETE_SUCCESS			= 10,
	HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID	= 11,

	HEADER_GC_ATTACK							= 12,
	HEADER_GC_STUN								= 13,
	HEADER_GC_DEAD								= 14,

	HEADER_GC_MAIN_CHARACTER_OLD				= 15,
	HEADER_GC_CHARACTER_POINTS					= 16,
	HEADER_GC_CHARACTER_POINT_CHANGE			= 17,
	HEADER_GC_CHANGE_SPEED						= 18,
	HEADER_GC_CHARACTER_UPDATE					= 19,

	HEADER_GC_ITEM_DEL							= 20,
	HEADER_GC_ITEM_SET							= 21,
	HEADER_GC_ITEM_USE							= 22,
	HEADER_GC_ITEM_DROP							= 23,
	HEADER_GC_ITEM_UPDATE						= 25,

	HEADER_GC_ITEM_GROUND_ADD					= 26,
	HEADER_GC_ITEM_GROUND_DEL					= 27,

	HEADER_GC_QUICKSLOT_ADD						= 28,
	HEADER_GC_QUICKSLOT_DEL						= 29,
	HEADER_GC_QUICKSLOT_SWAP					= 30,

	HEADER_GC_ITEM_OWNERSHIP					= 31,

	HEADER_GC_WHISPER							= 34,

	HEADER_GC_MOTION							= 36,
	HEADER_GC_PARTS								= 37,

	HEADER_GC_SHOP								= 38,
	HEADER_GC_SHOP_SIGN							= 39,

	HEADER_GC_DUEL_START						= 40,
	HEADER_GC_PVP                               = 41,
	HEADER_GC_EXCHANGE							= 42,
	HEADER_GC_CHARACTER_POSITION				= 43,

	HEADER_GC_PING								= 44,
	HEADER_GC_SCRIPT							= 45,
	HEADER_GC_QUEST_CONFIRM						= 46,

	HEADER_GC_MOUNT								= 61,
	HEADER_GC_OWNERSHIP							= 62,
	HEADER_GC_TARGET			 				= 63,

	HEADER_GC_WARP								= 65,

	HEADER_GC_ADD_FLY_TARGETING					= 69,
	HEADER_GC_CREATE_FLY						= 70,
	HEADER_GC_FLY_TARGETING						= 71,
	HEADER_GC_SKILL_LEVEL_OLD					= 72,
	HEADER_GC_SKILL_LEVEL						= 76,

	HEADER_GC_MESSENGER							= 74,
	HEADER_GC_GUILD								= 75,

	HEADER_GC_PARTY_INVITE						= 77,
	HEADER_GC_PARTY_ADD							= 78,
	HEADER_GC_PARTY_UPDATE						= 79,
	HEADER_GC_PARTY_REMOVE						= 80,
	HEADER_GC_QUEST_INFO						= 81,
	HEADER_GC_REQUEST_MAKE_GUILD				= 82,
	HEADER_GC_PARTY_PARAMETER					= 83,

	HEADER_GC_SAFEBOX_SET						= 85,
	HEADER_GC_SAFEBOX_DEL						= 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD			= 87,
	HEADER_GC_SAFEBOX_SIZE						= 88,

	HEADER_GC_FISHING							= 89,

	HEADER_GC_EMPIRE							= 90,

	HEADER_GC_PARTY_LINK						= 91,
	HEADER_GC_PARTY_UNLINK						= 92,

	HEADER_GC_REFINE_INFORMATION_OLD			= 95,

	HEADER_GC_VIEW_EQUIP						= 99,

	HEADER_GC_MARK_BLOCK						= 100,
	HEADER_GC_MARK_IDXLIST						= 102,

	HEADER_GC_TIME								= 106,
	HEADER_GC_CHANGE_NAME						= 107,

	HEADER_GC_DUNGEON							= 110,

	HEADER_GC_WALK_MODE							= 111,
	HEADER_GC_SKILL_GROUP						= 112,
	HEADER_GC_MAIN_CHARACTER					= 113,

	HEADER_GC_SEPCIAL_EFFECT					= 114,

	HEADER_GC_NPC_POSITION						= 115,

	HEADER_GC_LOGIN_KEY							= 118,
	HEADER_GC_REFINE_INFORMATION				= 119,
	HEADER_GC_CHANNEL							= 121,

	HEADER_GC_TARGET_UPDATE						= 123,
	HEADER_GC_TARGET_DELETE						= 124,
	HEADER_GC_TARGET_CREATE						= 125,

	HEADER_GC_AFFECT_ADD						= 126,
	HEADER_GC_AFFECT_REMOVE						= 127,

	HEADER_GC_MALL_OPEN							= 122,
	HEADER_GC_MALL_SET							= 128,
	HEADER_GC_MALL_DEL							= 129,

	HEADER_GC_LAND_LIST							= 130,
	HEADER_GC_LOVER_INFO						= 131,
	HEADER_GC_LOVE_POINT_UPDATE					= 132,

	HEADER_GC_SYMBOL_DATA						= 133,

	// MINING
	HEADER_GC_DIG_MOTION						= 134,
	// END_OF_MINING

	HEADER_GC_DAMAGE_INFO						= 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO				= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
	// END_OF_SUPPORT_BGM

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
	HEADER_GC_TARGET_INFO						= 140,
#endif
#ifdef ENABLE_INGAME_ITEMSHOP
	HEADER_GC_NEW_ITEMSHOP						= 142,
#endif
	HEADER_GC_AUTH_SUCCESS						= 150,
	HEADER_GC_PANAMA_PACK						= 151,

	//HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS					= 152,
	HEADER_GC_HYBRIDCRYPT_SDB					= 153, // SDB means Supplmentary Data Blocks
	//HYBRID CRYPT

#if defined(__ENABLE_RIDING_EXTENDED__)
	HEADER_GC_MOUNT_UP_GRADE					= 155,
	HEADER_GC_MOUNT_UP_GRADE_CHAT				= 156,
#endif
#ifdef __ENABLE_STAGE_SYSTEM__
	HEADER_GC_STAGE_SYSTEM						= 157,
#endif

#ifdef ENABLE_GUILD_TOKEN_AUTH
	HEADER_GC_GUILD_TOKEN						= 161,
#endif
#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_MANAGER						= 174,
#endif
#ifdef ENABLE_NEW_BIOLOG
	HEADER_GC_BIOLOG_UPDATE						= 183,
#endif
#ifdef ENABLE_DUNGEON_INFO_SYSTEM	
	HEADER_GC_DUNGEON_INFO						= 187,
	HEADER_GC_DUNGEON_RANKING					= 188,
#endif

	// ROULETTE
	HEADER_GC_ROULETTE							= 200,
	// END_ROULETTE

	HEADER_GC_SPECIFIC_EFFECT					= 208,

	HEADER_GC_DRAGON_SOUL_REFINE				= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS				= 210,
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL						= 214,
#endif
#ifdef ENABLE_NEW_RANKING_SYSTEM
	HEADER_GC_NEW_RANKING						= 218,
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
	HEADER_GC_NEW_OFFLINESHOP					= 221,
#endif
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	HEADER_GC_ANTI_FARM							= 226,
#endif
#ifdef ENABLE_CAPTCHA
	HEADER_GC_CAPTCHA							= 237,
#endif
#ifdef ENABLE_CSHIELD
	HEADER_GC_CSHIELD							= 238,
#endif

	/////////////////////////////////////////////////////////////////////////////
	HEADER_GG_LOGIN						= 1,
	HEADER_GG_LOGOUT					= 2,
	HEADER_GG_RELAY						= 3,
	HEADER_GG_NOTICE					= 4,
	HEADER_GG_SHUTDOWN					= 5,
	HEADER_GG_GUILD						= 6,
	HEADER_GG_DISCONNECT				= 7,
	HEADER_GG_SHOUT						= 8,
	HEADER_GG_SETUP						= 9,
	HEADER_GG_MESSENGER_ADD				= 10,
	HEADER_GG_MESSENGER_REMOVE			= 11,
	HEADER_GG_FIND_POSITION				= 12,
	HEADER_GG_WARP_CHARACTER			= 13,
	HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX	= 15,
	HEADER_GG_TRANSFER					= 16,
	HEADER_GG_XMAS_WARP_SANTA			= 17,
	HEADER_GG_XMAS_WARP_SANTA_REPLY		= 18,
	HEADER_GG_RELOAD_CRC_LIST			= 19,
	HEADER_GG_LOGIN_PING				= 20,
	HEADER_GG_CHECK_CLIENT_VERSION		= 21,
	HEADER_GG_BLOCK_CHAT				= 22,
//	HEADER_GG_BLANK						= 23,
//	HEADER_GG_BLANK						= 24,
	HEADER_GG_SIEGE						= 25,
	HEADER_GG_MONARCH_NOTICE			= 26,
	HEADER_GG_MONARCH_TRANSFER			= 27,
//	HEADER_GG_BLANK						= 28,
	HEADER_GG_CHECK_AWAKENESS			= 29,
#ifdef ENABLE_FULL_NOTICE	
	HEADER_GG_BIG_NOTICE				= 30,
#endif
#ifdef ENABLE_INGAME_ITEMSHOP
	HEADER_GG_ITEMSHOP					= 35,
#endif
#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
	HEADER_GG_OFFLINE_SHOP_NOTIFICATION	= 31,
#endif
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	HEADER_GG_ANTI_FARM					= 42,
#endif
#ifdef ENABLE_LOG_NOTIFICATIONS
	HEADER_GG_CSHIELD_LOG_NOTIFICATION	= 43,
#endif

};

#pragma pack(1)
typedef struct SPacketGGSetup
{
	BYTE	bHeader;
	WORD	wPort;
	BYTE	bChannel;
} TPacketGGSetup;

typedef struct SPacketGGLogin
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	BYTE	bEmpire;
	long	lMapIndex;
	BYTE	bChannel;
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	char	cMAIf[MA_LENGTH + 1];
	int8_t	i8BlockState;
#endif
} TPacketGGLogin;

typedef struct SPacketGGLogout
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	bool	bAFisWarping;
#endif
} TPacketGGLogout;

typedef struct SPacketGGRelay
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lSize;
} TPacketGGRelay;

typedef struct SPacketGGNotice
{
	BYTE	bHeader;
	long	lSize;
} TPacketGGNotice;

typedef struct SPacketGGMonarchNotice
{
	BYTE	bHeader;
	BYTE	bEmpire;
	long	lSize;
} TPacketGGMonarchNotice;

//FORKED_ROAD
typedef struct SPacketGGForkedMapInfo
{
	BYTE	bHeader;
	BYTE	bPass;
	BYTE	bSungzi;
} TPacketGGForkedMapInfo;
//END_FORKED_ROAD
typedef struct SPacketGGShutdown
{
	BYTE	bHeader;
} TPacketGGShutdown;

typedef struct SPacketGGGuild
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
} TPacketGGGuild;

enum
{
	GUILD_SUBHEADER_GG_CHAT,
	GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS,
};

typedef struct SPacketGGGuildChat
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGGuildChat;

typedef struct SPacketGGParty
{
	BYTE	header;
	BYTE	subheader;
	DWORD	pid;
	DWORD	leaderpid;
} TPacketGGParty;

enum
{
	PARTY_SUBHEADER_GG_CREATE,
	PARTY_SUBHEADER_GG_DESTROY,
	PARTY_SUBHEADER_GG_JOIN,
	PARTY_SUBHEADER_GG_QUIT,
};

typedef struct SPacketGGDisconnect
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGDisconnect;

typedef struct SPacketGGShout
{
	BYTE	bHeader;
	BYTE	bEmpire;
	char	szText[CHAT_MAX_LEN + 1];
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	char    szCountry[COUNTRY_NAME_MAX_LEN + 1]{};
	int     hyperlinks{};
#endif
} TPacketGGShout;

typedef struct SPacketGGXmasWarpSanta
{
	BYTE	bHeader;
	BYTE	bChannel;
	long	lMapIndex;
} TPacketGGXmasWarpSanta;

typedef struct SPacketGGXmasWarpSantaReply
{
	BYTE	bHeader;
	BYTE	bChannel;
} TPacketGGXmasWarpSantaReply;

typedef struct SPacketGGMessenger
{
	BYTE        bHeader;
	char        szAccount[CHARACTER_NAME_MAX_LEN + 1];
	char        szCompanion[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGMessenger;

typedef struct SPacketGGFindPosition
{
	BYTE header;
	DWORD dwFromPID;
	DWORD dwTargetPID;
} TPacketGGFindPosition;

typedef struct SPacketGGWarpCharacter
{
	BYTE header;
	DWORD pid;
	long x;
	long y;
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	int mapIndex;
#endif
} TPacketGGWarpCharacter;

//  HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX	    = 15,

typedef struct SPacketGGGuildWarMapIndex
{
	BYTE bHeader;
	DWORD dwGuildID1;
	DWORD dwGuildID2;
	long lMapIndex;
} TPacketGGGuildWarMapIndex;

typedef struct SPacketGGTransfer
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lX, lY;
} TPacketGGTransfer;

typedef struct SPacketGGLoginPing
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGLoginPing;

typedef struct SPacketGGBlockChat
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lBlockDuration;
} TPacketGGBlockChat;

typedef struct command_text
{
	BYTE	bHeader;
} TPacketCGText;

typedef struct command_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketCGHandshake;

typedef struct command_login
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
#ifdef ENABLE_ANTI_MULTIPLE_FARM
	char	cMAIf[MA_LENGTH + 1];
#endif
} TPacketCGLogin2;

typedef struct command_login3
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	DWORD	adwClientKey[4];
#ifdef ENABLE_CSHIELD
	char	cshieldKey[64 + 1];
	char	cshieldHwid[128 + 1];
#endif
} TPacketCGLogin3;

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	index;
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	char	country[COUNTRY_NAME_MAX_LEN + 1];
#endif
} TPacketCGPlayerSelect;

typedef struct command_player_delete
{
	BYTE	header;
	BYTE	index;
	char	private_code[8];
} TPacketCGPlayerDelete;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE	shape;
	BYTE	Con;
	BYTE	Int;
	BYTE	Str;
	BYTE	Dex;
} TPacketCGPlayerCreate;

typedef struct command_player_create_success
{
	BYTE		header;
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketGCPlayerCreateSuccess;

typedef struct command_attack
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwVID;
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

enum EMoveFuncType
{
	FUNC_WAIT,
	FUNC_MOVE,
	FUNC_ATTACK,
	FUNC_COMBO,
	FUNC_MOB_SKILL,
	_FUNC_SKILL,
	FUNC_MAX_NUM,
	FUNC_SKILL = 0x80,
};

typedef struct command_move
{
	BYTE	bHeader;
	BYTE	bFunc;
	BYTE	bArg;
	BYTE	bRot;
	long	lX;
	long	lY;
	DWORD	dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketCGSyncPositionElement;

typedef struct command_sync_position
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketCGSyncPosition;

typedef struct command_chat
{
	BYTE	header;
	WORD	size;
	BYTE	type;
} TPacketCGChat;

typedef struct command_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	char 	szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_entergame
{
	BYTE	header;
} TPacketCGEnterGame;

typedef struct command_item_use
{
	BYTE 	header;
	TItemPos 	Cell;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE	header;
	TItemPos	Cell;
	TItemPos	TargetCell;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
#if defined(ENABLE_CHEQUE_SYSTEM) && !defined(DISABLE_CHEQUE_DROP)
	DWORD	cheque;
#endif
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
#if defined(ENABLE_CHEQUE_SYSTEM) && !defined(DISABLE_CHEQUE_DROP)
	DWORD	cheque;
#endif
//	BYTE	count;
	short count;
} TPacketCGItemDrop2;

#ifdef WJ_NEW_DROP_DIALOG
typedef struct command_item_destroy
{
	BYTE		header;
	TItemPos	Cell;
} TPacketCGItemDestroy;

typedef struct command_item_sell
{
	BYTE		header;
	TItemPos	Cell;
} TPacketCGItemSell;
#endif

typedef struct command_item_move
{
	BYTE 	header;
	TItemPos	Cell;
	TItemPos	CellTo;
//	BYTE	count;
	short	count;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE 	header;
	DWORD	vid;
} TPacketCGItemPickup;

typedef struct command_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
} TPacketCGQuickslotAdd;

typedef struct command_quickslot_del
{
	BYTE	header;
	BYTE	pos;
} TPacketCGQuickslotDel;

typedef struct command_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	change_pos;
} TPacketCGQuickslotSwap;

enum
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2
};

typedef struct command_shop_buy
{
	BYTE	count;
} TPacketCGShopBuy;

typedef struct command_shop_sell
{
	BYTE	pos;
	BYTE	count;
} TPacketCGShopSell;

typedef struct command_shop
{
	BYTE	header;
//	BYTE	subheader;
	short	subheader;
} TPacketCGShop;

typedef struct command_on_click
{
	BYTE	header;
	DWORD	vid;
} TPacketCGOnClick;

enum
{
	EXCHANGE_SUBHEADER_CG_START,	/* arg1 == vid of target character */
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ELK_ADD,	/* arg1 == amount of gold */
	EXCHANGE_SUBHEADER_CG_ACCEPT,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_CG_CANCEL,	/* arg1 == not used */
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,
#endif
};

typedef struct command_exchange
{
	BYTE	header;
	BYTE	sub_header;
	long long	arg1;
	BYTE	arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE	header;
	BYTE	position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE	header;
	BYTE	answer;
	//char	file[32 + 1];
	//BYTE	answer[16 + 1];
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
	BYTE        header;
	unsigned int	idx;
} TPacketCGScriptButton;

typedef struct command_quest_input_string
{
	BYTE header;
	char msg[64+1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;

typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64+1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketGCHandshake;

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_LOGIN,
	PHASE_SELECT,
	PHASE_LOADING,
	PHASE_GAME,
	PHASE_DEAD,

	PHASE_CLIENT_CONNECTING,
	PHASE_DBCLIENT,
	PHASE_P2P,
	PHASE_AUTH,
};

typedef struct packet_phase
{
	BYTE	header;
	BYTE	phase;
} TPacketGCPhase;

typedef struct packet_bindudp
{
	BYTE	header;
	DWORD	addr;
	WORD	port;
} TPacketGCBindUDP;

enum
{
	LOGIN_FAILURE_ALREADY	= 1,
	LOGIN_FAILURE_ID_NOT_EXIST	= 2,
	LOGIN_FAILURE_WRONG_PASS	= 3,
	LOGIN_FAILURE_FALSE		= 4,
	LOGIN_FAILURE_NOT_TESTOR	= 5,
	LOGIN_FAILURE_NOT_TEST_TIME	= 6,
	LOGIN_FAILURE_FULL		= 7
};

typedef struct packet_login_success
{
	BYTE		bHeader;
	TSimplePlayer	players[PLAYER_PER_ACCOUNT];
	DWORD		guild_id[PLAYER_PER_ACCOUNT];
	char		guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN+1];

	DWORD		handle;
	DWORD		random_key;
} TPacketGCLoginSuccess;

typedef struct packet_auth_success
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
	BYTE	bResult;
} TPacketGCAuthSuccess;

typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct packet_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

enum
{
	ADD_CHARACTER_STATE_DEAD		= (1 << 0),
	ADD_CHARACTER_STATE_SPAWN		= (1 << 1),
	ADD_CHARACTER_STATE_GUNGON		= (1 << 2),
	ADD_CHARACTER_STATE_KILLER		= (1 << 3),
	ADD_CHARACTER_STATE_PARTY		= (1 << 4),
};

enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,

	CHR_EQUIPPART_NUM,
};

typedef struct packet_add_char
{
	BYTE	header;
	DWORD	dwVID;

	float	angle;
	long	x;
	long	y;
	long	z;

	BYTE	bType;
	DWORD	wRaceNum; // @fixme501
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD dwAffectFlag[2];
} TPacketGCCharacterAdd;

typedef struct packet_char_additional_info
{
	BYTE	header;
	DWORD	dwVID;
	char	name[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	awPart[CHR_EQUIPPART_NUM]; // @fixme502
	BYTE	bEmpire;
	DWORD	dwGuildID;
	DWORD	dwLevel;
	short	sAlignment;
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#ifdef ENABLE_QUIVER_SYSTEM
	DWORD	dwArrow;
#endif
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	char	country[COUNTRY_NAME_MAX_LEN + 1];
#endif
} TPacketGCCharacterAdditionalInfo;

typedef struct packet_update_char
{
	BYTE	header;
	DWORD	dwVID;

	DWORD	awPart[CHR_EQUIPPART_NUM]; // @fixme502
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD dwAffectFlag[2];

	DWORD	dwGuildID;
	short	sAlignment;
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#ifdef ENABLE_QUIVER_SYSTEM
	DWORD	dwArrow;
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_del_char
{
	BYTE	header;
	DWORD	id;
} TPacketGCCharacterDelete;

typedef struct packet_chat
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	id;
	BYTE	bEmpire{};
#if defined(__BL_CLIENT_LOCALE_STRING__)
	bool	bCanFormat{true};
#endif
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	char	szCountry[COUNTRY_NAME_MAX_LEN + 1]{};
#endif
} TPacketGCChat;

typedef struct packet_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	BYTE	bType;
	char	szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
#if defined(__BL_CLIENT_LOCALE_STRING__)
	bool	bCanFormat{true};
#endif
#if defined(__BL_MULTI_LANGUAGE_PREMIUM__)
	BYTE	bEmpire{};
	char	szCountry[COUNTRY_NAME_MAX_LEN + 1]{};
#endif
} TPacketGCWhisper;

typedef struct packet_main_character
{
	BYTE        header;
	DWORD	dwVID;
	DWORD	wRaceNum; // @fixme501
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	DWORD	wRaceNum; // @fixme501
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	DWORD	wRaceNum; // @fixme501
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	float	fBGMVol;
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

typedef struct packet_points
{
	BYTE	header;
	long long	points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_skill_level
{
	BYTE		bHeader;
	TPlayerSkill	skills[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct packet_point_change
{
	int		header;
	DWORD	dwVID;
	BYTE	type;
	long long	amount;
	long long	value;
} TPacketGCPointChange;

typedef struct packet_stun
{
	BYTE	header;
	DWORD	vid;
} TPacketGCStun;

typedef struct packet_dead
{
	BYTE	header;
	DWORD	vid;
} TPacketGCDead;

struct TPacketGCItemDelDeprecated
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
//	BYTE	count;
	short	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

typedef struct packet_item_set
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
//	BYTE	count;
	short	count;
	DWORD	flags;
	DWORD	anti_flags;
	bool	highlight;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_SOULBIND_SYSTEM
	long	sealbind;
#endif
} TPacketGCItemSet;

typedef struct packet_item_del
{
	BYTE	header;
	BYTE	pos;
} TPacketGCItemDel;

struct packet_item_use
{
	BYTE	header;
	TItemPos Cell;
	DWORD	ch_vid;
	DWORD	victim_vid;
	DWORD	vnum;
};

struct packet_item_move
{
	BYTE	header;
	TItemPos Cell;
	TItemPos CellTo;
};

typedef struct packet_item_update
{
	BYTE	header;
	TItemPos Cell;
//	BYTE	count;
	short	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_SOULBIND_SYSTEM
	long	sealbind;
#endif
} TPacketGCItemUpdate;

typedef struct packet_item_ground_add
{
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	packet_item_ground_add()
	{
		memset(&alSockets, 0, sizeof(alSockets));
		memset(&aAttrs, 0, sizeof(aAttrs));
	}
#endif

	BYTE	bHeader;
	long 	x, y, z;
	DWORD	dwVID;
	DWORD	dwVnum;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	long	alSockets[ITEM_SOCKET_MAX_NUM];//3
	TPlayerItemAttribute aAttrs[ITEM_ATTRIBUTE_MAX_NUM];//7
#endif
} TPacketGCItemGroundAdd;

typedef struct packet_item_ownership
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_item_ground_del
{
	BYTE	bHeader;
	DWORD	dwVID;
} TPacketGCItemGroundDel;

struct packet_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
};

struct packet_quickslot_del
{
	BYTE	header;
	BYTE	pos;
};

struct packet_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	pos_to;
};

struct packet_motion
{
	BYTE	header;
	DWORD	vid;
	DWORD	victim_vid;
	WORD	motion;
};

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
	SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
	SHOP_SUBHEADER_GC_OK,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
	SHOP_SUBHEADER_GC_SOLDOUT,
	SHOP_SUBHEADER_GC_INVENTORY_FULL,
	SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#ifdef ENABLE_CHEQUE_SYSTEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_CHEQUE,
#endif
#ifdef ENABLE_MULTISHOP
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM,
#endif
};

struct packet_shop_item
{
	DWORD		vnum;
	long long	price;
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		cheque;
#endif
//	BYTE		count;
	short		count;
#ifdef ENABLE_MULTISHOP
	DWORD		wPriceVnum;
	DWORD		wPrice;
#endif
	BYTE		display_pos;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

typedef struct packet_shop_start
{
	DWORD		owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

typedef struct packet_shop_start_ex
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
} TPacketGCShopStartEx;

typedef struct packet_shop_update_item
{
	BYTE			pos;
	struct packet_shop_item	item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	long long			iPrice;
} TPacketGCShopUpdatePrice;

typedef struct packet_shop
{
	BYTE		header;
	WORD		size;
	BYTE		subheader;
} TPacketGCShop;

struct packet_exchange
{
	BYTE	header;
	BYTE	sub_header;
	BYTE	is_me;
	long long	arg1;
//	DWORD	arg1;	// vnum
	TItemPos	arg2;	// cell
	DWORD	arg3;	// count
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

enum EPacketTradeSubHeaders
{
	EXCHANGE_SUBHEADER_GC_START,	/* arg1 == vid */
	EXCHANGE_SUBHEADER_GC_ITEM_ADD,	/* arg1 == vnum  arg2 == pos  arg3 == count */
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,
	EXCHANGE_SUBHEADER_GC_GOLD_ADD,	/* arg1 == gold */
	EXCHANGE_SUBHEADER_GC_ACCEPT,	/* arg1 == accept */
	EXCHANGE_SUBHEADER_GC_END,		/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_ALREADY,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_LESS_GOLD,	/* arg1 == not used */
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,
#endif
};

struct packet_position
{
	BYTE	header;
	DWORD	vid;
	BYTE	position;
};

typedef struct packet_ping
{
	BYTE	header;
} TPacketGCPing;

struct packet_script
{
	BYTE	header;
	WORD	size;
	BYTE	skin;
	WORD	src_size;
#ifdef ENABLE_QUEST_CATEGORY
	BYTE	quest_flag;
#endif
};

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

struct packet_mount
{
	BYTE	header;
	DWORD	vid;
	DWORD	mount_vid;
	BYTE	pos;
	DWORD	x, y;
};

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	long		lX;
	long		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

typedef struct packet_ownership
{
	BYTE		bHeader;
	DWORD		dwOwnerVID;
	DWORD		dwVictimVID;
} TPacketGCOwnership;

typedef struct packet_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketGCSyncPositionElement;

typedef struct packet_sync_position
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketGCSyncPosition;

typedef struct packet_fly
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwStartVID;
	DWORD	dwEndVID;
} TPacketGCCreateFly;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE		bHeader;
	BYTE		bType;
} TPacketCGShoot;

typedef struct packet_duel_start
{
	BYTE	header;
	WORD	wSize;
} TPacketGCDuelStart;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE
};

typedef struct packet_pvp
{
	BYTE        bHeader;
	DWORD       dwVIDSrc;
	DWORD       dwVIDDst;
	BYTE        bMode;
} TPacketGCPVP;

typedef struct command_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVID;
} TPacketCGUseSkill;

typedef struct command_target
{
	BYTE	header;
	DWORD	dwVID;
} TPacketCGTarget;

typedef struct packet_target
{
	BYTE	header;
	DWORD	dwVID;
#ifdef ENABLE_MOB_TARGET_HP
	DWORD	dwHp;
	DWORD	dwHpMax;
#else
	BYTE	bHPPercent;
#endif
} TPacketGCTarget;

#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
typedef struct packet_target_info
{
	BYTE	header;
	DWORD	dwVID;
	DWORD	race;
	DWORD	dwVnum;
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	WORD	count;
#else
	BYTE	count;
#endif
} TPacketGCTargetInfo;

typedef struct packet_target_info_load
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTargetInfoLoad;
#endif

typedef struct packet_warp
{
	BYTE	bHeader;
	long	lX;
	long	lY;
	long	lAddr;
	WORD	wPort;
} TPacketGCWarp;

typedef struct command_warp
{
	BYTE	bHeader;
} TPacketCGWarp;

struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
#ifdef ENABLE_QUEST_CATEGORY
	WORD c_index;
#endif
	BYTE flag;
};

enum
{
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
};

typedef struct packet_messenger
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_guild_list
{
	BYTE connected;
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildList;

typedef struct packet_messenger_guild_login
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogin;

typedef struct packet_messenger_guild_logout
{
	BYTE length;

	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogout;

typedef struct packet_messenger_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

typedef struct packet_messenger_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerListOnline;

enum
{
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE,
	MESSENGER_SUBHEADER_CG_INVITE_ANSWER,
};

typedef struct command_messenger
{
	BYTE header;
	BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_add_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddByVID;

typedef struct command_messenger_add_by_name
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketCGMessengerAddByName;

typedef struct command_messenger_remove
{
	char login[LOGIN_MAX_LEN+1];
	//DWORD account;
} TPacketCGMessengerRemove;

typedef struct command_safebox_checkout
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_party_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketCGPartyParameter;

typedef struct paryt_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketGCPartyParameter;

typedef struct packet_party_add
{
	BYTE	header;
	DWORD	pid;
	char	name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct command_party_invite
{
	BYTE	header;
	DWORD	vid;
} TPacketCGPartyInvite;

typedef struct packet_party_invite
{
	BYTE	header;
	DWORD	leader_vid;
} TPacketGCPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE	header;
	DWORD	leader_vid;
	BYTE	accept;
} TPacketCGPartyInviteAnswer;

typedef struct packet_party_update
{
	BYTE	header;
	DWORD	pid;
	BYTE	role;
	BYTE	percent_hp;
	short	affects[7];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

typedef struct packet_party_link
{
	BYTE header;
	DWORD pid;
	DWORD vid;
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
	BYTE header;
	DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE header;
	DWORD pid;
	BYTE byRole;
	BYTE flag;
} TPacketCGPartySetState;

enum
{
	PARTY_SKILL_HEAL = 1,
	PARTY_SKILL_WARP = 2
};

typedef struct command_party_use_skill
{
	BYTE header;
	BYTE bySkillIndex;
	DWORD vid;
} TPacketCGPartyUseSkill;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketCGSafeboxSize;

typedef struct packet_safebox_wrong_password
{
	BYTE	bHeader;
} TPacketCGSafeboxWrongPassword;

typedef struct command_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketGCEmpire;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
	BYTE        bHeader;
	BYTE        bState;
	long	lMoney;
} TPacketCGSafeboxMoney;

typedef struct packet_safebox_money_change
{
	BYTE	bHeader;
	long	lMoney;
} TPacketGCSafeboxMoneyChange;

// Guild

enum
{
	GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH,
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
	GUILD_SUBHEADER_GC_CHANGE_EXP,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
	GUILD_SUBHEADER_GC_WAR,
	GUILD_SUBHEADER_GC_GUILD_NAME,
	GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
	GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
	GUILD_SUBHEADER_GC_WAR_SCORE,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

enum GUILD_SUBHEADER_CG
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

typedef struct packet_guild_name_t
{
	BYTE header;
	WORD size;
	BYTE subheader;
	DWORD	guildID;
	char	guildName[GUILD_NAME_MAX_LEN];
} TPacketGCGuildName;

typedef struct packet_guild_war
{
	DWORD	dwGuildSelf;
	DWORD	dwGuildOpp;
	BYTE	bType;
	BYTE 	bWarState;
} TPacketGCGuildWar;

typedef struct command_guild
{
	BYTE header;
	BYTE subheader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_guild_use_skill
{
	DWORD	dwVnum;
	DWORD	dwPID;
} TPacketCGGuildUseSkill;

// Guild Mark
typedef struct command_mark_login
{
	BYTE    header;
	DWORD   handle;
	DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE	header;
	DWORD	gid;
	BYTE	image[16*12*4];
#ifdef ENABLE_GUILD_TOKEN_AUTH
	uint64_t	token;
#endif
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
	BYTE	header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
	BYTE	header;
	BYTE	imgIdx;
	DWORD	crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE    header;
	DWORD	bufSize;
	WORD	count;
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE	header;
	DWORD	bufSize;
	BYTE	imgIdx;
	DWORD	count;
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	guild_id;
#ifdef ENABLE_GUILD_TOKEN_AUTH
	uint64_t	token;
#endif
} TPacketCGGuildSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE header;
	DWORD guild_id;
	DWORD crc;
	DWORD size;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

// Fishing

typedef struct command_fishing
{
	BYTE header;
	BYTE dir;
} TPacketCGFishing;

typedef struct packet_fishing
{
	BYTE header;
	BYTE subheader;
	DWORD info;
	BYTE dir;
} TPacketGCFishing;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
	FISHING_SUBHEADER_GC_FISH,
};

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
//	BYTE	byItemCount;
	short	byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE	bHeader;
#ifdef ENABLE_CSHIELD
	DWORD	errorcode;
	BYTE	win;
	char	language[3 + 1];
	char	report[1024 + 1];
#else
	char	szBuf[255 + 1];
#endif
} TPacketCGHack;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE bHeader;
	WORD size;
	BYTE subheader;
} TPacketGCDungeon;

typedef struct packet_dungeon_dest_position
{
	long x;
	long y;
} TPacketGCDungeonDestPosition;

typedef struct SPacketGCShopSign
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketCGMyShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
//	int		bCount;
	short	bCount;
} TPacketCGMyShop;

typedef struct SPacketGCTime
{
	BYTE	bHeader;
	time_t	time;
} TPacketGCTime;

enum
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE	header;
	DWORD	vid;
	BYTE	mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE        header;
	BYTE        skill_group;
} TPacketGCChangeSkillGroup;

typedef struct SPacketCGRefine
{
	BYTE	header;
	BYTE	pos;
	BYTE	type;
} TPacketCGRefine;

typedef struct SPacketCGRequestRefineInfo
{
	BYTE	header;
	BYTE	pos;
} TPacketCGRequestRefineInfo;

typedef struct SPacketGCRefineInformaion
{
	BYTE	header;
	BYTE	type;
	BYTE	pos;
	DWORD	src_vnum;
	DWORD	result_vnum;
//	BYTE	material_count;
	short	material_count;
	long long		cost;
	int		prob;
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TPacketGCRefineInformation;

struct TNPCPosition
{
	BYTE bType;
	char name[CHARACTER_NAME_MAX_LEN+1];
	long x;
	long y;
};

typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;

	// array of TNPCPosition
} TPacketGCNPCPosition;

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;

typedef struct command_client_version2
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion2;

typedef struct packet_channel
{
	BYTE header;
	BYTE channel;
} TPacketGCChannel;

#ifdef ENABLE_GUILD_TOKEN_AUTH
struct TPacketGCGuildToken {
	uint8_t header;
	uint64_t token;
};
#endif

typedef struct SEquipmentItemSet
{
	DWORD	vnum;
	BYTE	count;
//	short	count;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
	BYTE  header;
	DWORD vid;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketViewEquip;

typedef struct SLandPacketElement
{
	DWORD	dwID;
	long	x, y;
	long	width, height;
	DWORD	dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE	header;
	WORD	size;
} TPacketGCLandList;

typedef struct SPacketGCTargetCreate
{
	BYTE	bHeader;
	long	lID;
	char	szName[32+1];
	DWORD	dwVID;
	BYTE	bType;
} TPacketGCTargetCreate;

typedef struct SPacketGCTargetUpdate
{
	BYTE	bHeader;
	long	lID;
	long	lX, lY;
} TPacketGCTargetUpdate;

typedef struct SPacketGCTargetDelete
{
	BYTE	bHeader;
	long	lID;
} TPacketGCTargetDelete;

typedef struct SPacketGCAffectAdd
{
	BYTE		bHeader;
	TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct SPacketGCAffectRemove
{
	BYTE	bHeader;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_lover_info
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE love_point;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE header;
	BYTE love_point;
} TPacketGCLovePointUpdate;

// MINING
typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;
// END_OF_MINING

// SCRIPT_SELECT_ITEM
typedef struct command_script_select_item
{
	BYTE header;
	DWORD selection;
} TPacketCGScriptSelectItem;
// END_OF_SCRIPT_SELECT_ITEM

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int damage;
} TPacketGCDamageInfo;

typedef struct tag_GGSiege
{
	BYTE	bHeader;
	BYTE	bEmpire;
	BYTE	bTowerCount;
} TPacketGGSiege;

typedef struct SPacketGGMonarchTransfer
{
	BYTE	bHeader;
	DWORD	dwTargetPID;
	long	x;
	long	y;
} TPacketMonarchGGTransfer;

typedef struct SPacketGGCheckAwakeness
{
	BYTE bHeader;
} TPacketGGCheckAwakeness;

typedef struct SPacketGCPanamaPack
{
	BYTE	bHeader;
	char	szPackName[256];
	BYTE	abIV[32];
} TPacketGCPanamaPack;

typedef struct SPacketGCHybridCryptKeys
{
	SPacketGCHybridCryptKeys() : m_pStream(NULL) {}
	~SPacketGCHybridCryptKeys()
	{
		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + KeyStreamLen;
	}

	BYTE* GetStreamData()
	{
		if( m_pStream )
			delete[] m_pStream;

		uDynamicPacketSize = (WORD)GetStreamSize();

		m_pStream = new BYTE[ uDynamicPacketSize ];

		memcpy( m_pStream, &bHeader, 1 );
		memcpy( m_pStream+1, &uDynamicPacketSize, 2 );
		memcpy( m_pStream+3, &KeyStreamLen, 4 );

		if( KeyStreamLen > 0 )
			memcpy( m_pStream+7, pDataKeyStream, KeyStreamLen );

		return m_pStream;
	}

	BYTE	bHeader;
	WORD    uDynamicPacketSize;
	int		KeyStreamLen;
	BYTE*   pDataKeyStream;

private:
	BYTE* m_pStream;
} TPacketGCHybridCryptKeys;

typedef struct SPacketGCPackageSDB
{
	SPacketGCPackageSDB() : m_pDataSDBStream(NULL), m_pStream(NULL) {}
	~SPacketGCPackageSDB()
	{
		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + iStreamLen;
	}

	BYTE* GetStreamData()
	{
		if( m_pStream )
			delete[] m_pStream;

		uDynamicPacketSize =  GetStreamSize();

		m_pStream = new BYTE[ uDynamicPacketSize ];

		memcpy( m_pStream, &bHeader, 1 );
		memcpy( m_pStream+1, &uDynamicPacketSize, 2 );
		memcpy( m_pStream+3, &iStreamLen, 4 );

		if( iStreamLen > 0 )
			memcpy( m_pStream+7, m_pDataSDBStream, iStreamLen );

		return m_pStream;
	}

	BYTE	bHeader;
	WORD    uDynamicPacketSize;
	int		iStreamLen;
	BYTE*   m_pDataSDBStream;

private:
	BYTE* m_pStream;
} TPacketGCPackageSDB;

#ifdef _IMPROVED_PACKET_ENCRYPTION_
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};

#endif // _IMPROVED_PACKET_ENCRYPTION_

#define MAX_EFFECT_FILE_NAME 128
typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[MAX_EFFECT_FILE_NAME];
} TPacketGCSpecificEffect;

enum EDragonSoulRefineWindowRefineType
{
	DragonSoulRefineWindow_UPGRADE,
	DragonSoulRefineWindow_IMPROVEMENT,
	DragonSoulRefineWindow_REFINE,
};

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_REFINE_GRADE,
	DS_SUB_HEADER_DO_REFINE_STEP,
	DS_SUB_HEADER_DO_REFINE_STRENGTH,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
};
typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine() : header (HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DRAGON_SOUL_REFINE_GRID_SIZE];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SPacketCGStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
} TPacketCGStateCheck;

typedef struct SPacketGCStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
	unsigned char state;
} TPacketGCStateCheck;

#ifdef ENABLE_SHOW_CHEST_DROP
enum
{
	HEADER_CG_CHEST_DROP_INFO = 215,
	HEADER_GC_CHEST_DROP_INFO = 220,
};

typedef struct SPacketCGChestDropInfo
{
	BYTE	header;
	WORD	wInventoryCell;
} TPacketCGChestDropInfo;

typedef struct SChestDropInfoTable
{
	BYTE	bPageIndex;
	BYTE	bSlotIndex;
	DWORD	dwItemVnum;
	BYTE	bItemCount;
} TChestDropInfoTable;

typedef struct SPacketGCChestDropInfo
{
	BYTE	bHeader;
	WORD	wSize;
	DWORD	dwChestVnum;
} TPacketGCChestDropInfo;
#endif

#ifdef ENABLE_DUNGEON_INFO_SYSTEM
enum ESubHeader
{
	SUBHEADER_DUNGEON_INFO_SEND,
	SUBHEADER_DUNGEON_INFO_OPEN,
};

typedef struct SPacketCGDungeonInfo
{
	uint8_t byHeader;
	uint8_t bySubHeader;
	uint8_t byIndex;
	uint8_t byRankType;
} TPacketCGDungeonInfo;

struct LevelLimit { int iLevelMin, iLevelMax, iMemberMin, iMemberMax; };
struct Item { uint32_t dwVnum; uint16_t wCount; };
struct Bonus { uint16_t byAttBonus[POINT_MAX_NUM]; uint16_t byDefBonus[POINT_MAX_NUM]; };
struct Results { uint32_t dwFinish, dwFinishTime, dwFinishDamage; };

typedef struct SPacketGCDungeonInfo
{
	SPacketGCDungeonInfo() : byHeader(HEADER_GC_DUNGEON_INFO), bySubHeader(0), byIndex(0), byType(0),
		bReset(false),
		lMapIndex(0), lEntryMapIndex(0),
		dwBossVnum(0),
		dwDuration(0), dwCooldown(0), byElement(0)
	{
		memset(&sLevelLimit, 0, sizeof(sLevelLimit));
		memset(&sRequiredItem, 0, sizeof(sRequiredItem));
		memset(&sBonus.byAttBonus, 0, sizeof(sBonus.byAttBonus));
		memset(&sBonus.byDefBonus, 0, sizeof(sBonus.byDefBonus));
		memset(&sBossDropItem, 0, sizeof(sBossDropItem));
		memset(&sResults, 0, sizeof(sResults));
	}

	uint8_t byHeader;
	uint8_t bySubHeader;
	uint16_t byIndex;
	uint16_t byType;
	bool bReset;
	long lMapIndex;
	long lEntryMapIndex;
	uint32_t dwBossVnum;
	LevelLimit sLevelLimit;
	Item sRequiredItem[EDungeonInfoData::MAX_REQUIRED_ITEMS];
	uint32_t dwDuration;
	uint32_t dwCooldown;
	uint8_t byElement;
	Bonus sBonus;
	Item sBossDropItem[EDungeonInfoData::MAX_BOSS_ITEM_SLOTS];
	Results sResults;
} TPacketGCDungeonInfo;

typedef struct SPacketGCDungeonInfoRank
{
	SPacketGCDungeonInfoRank() { strncpy(szName, "", sizeof(szName)); }
	SPacketGCDungeonInfoRank(const char* c_szName, const int c_iLevel, const uint32_t c_dwPoints) :
		iLevel(c_iLevel), dwPoints(c_dwPoints) 
	{
		strncpy(szName, c_szName, sizeof(szName));
	}

	uint8_t byHeader = HEADER_GC_DUNGEON_RANKING;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	int iLevel = 0;
	uint32_t dwPoints = 0;
} TPacketGCDungeonInfoRank;
#endif

#ifdef ENABLE_EVENT_MANAGER
typedef struct SPacketGCEventManager
{
	BYTE	header;
	DWORD	size;
} TPacketGCEventManager;
#endif

#ifdef ENABLE_NEW_BIOLOG
typedef struct sBiologGCInformation {
	BYTE	bHeader;
	DWORD	dwItemVnum;
	DWORD	dwSoulVnum;
	BYTE	bGivenCount;
	BYTE	bState;
	BYTE	bRequiredCount;
	BYTE	bAffect[4];
	WORD	bAffValue[4];
	BYTE	bChance;
	int		iTime; 
}tBiologGCInformation;
#endif

#ifdef __ENABLE_NEW_OFFLINESHOP__

//ACTIONS PACKETS
typedef struct
{
	BYTE bHeader;
	#ifdef __ENABLE_LARGE_DYNAMIC_PACKET__
	int wSize;
	#else
	WORD wSize;
	#endif
	BYTE bSubHeader;
} TPacketGCNewOfflineshop;

typedef struct
{
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
} TPacketCGNewOfflineShop;

namespace offlineshop
{
	//common
	typedef struct
	{
		BYTE		bType;
		BYTE		bSubType;

		char		szName[ITEM_NAME_MAX_LEN];
		TPriceInfo	priceStart, priceEnd;
		int			iLevelStart, iLevelEnd;

		DWORD		dwWearFlag;
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_NORM_NUM];
		#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
		bool		bPlayerName;
		#endif
	} TFilterInfo;


	typedef struct
	{
		TItemPos	pos;
		TPriceInfo	price;
	} TShopItemInfo;

	//AUCTION
	typedef struct
	{
		TAuctionInfo	auction;
		TPriceInfo		actual_best;
		DWORD			dwOfferCount;
	} TAuctionListElement;

	//offlineshop-updated 03/08/19
	typedef struct
	{
		TItemInfo item;
		char szShopName[OFFLINE_SHOP_NAME_MAX_LEN];
	} TMyOfferExtraInfo;


	//GAME TO CLIENT
	enum eSubHeaderGC
	{
		SUBHEADER_GC_SHOP_LIST,
		SUBHEADER_GC_SHOP_OPEN,
		SUBHEADER_GC_SHOP_OPEN_OWNER,
		SUBHEADER_GC_SHOP_OPEN_OWNER_NO_SHOP,
		SUBHEADER_GC_SHOP_CLOSE,
		SUBHEADER_GC_SHOP_BUY_ITEM_FROM_SEARCH,

		SUBHEADER_GC_OFFER_LIST,

		SUBHEADER_GC_SHOP_FILTER_RESULT,
		SUBHEADER_GC_SHOP_SAFEBOX_REFRESH,

		//AUCTION
		SUBHEADER_GC_AUCTION_LIST,
		SUBHEADER_GC_OPEN_MY_AUCTION,
		SUBHEADER_GC_OPEN_MY_AUCTION_NO_AUCTION,
		SUBHEADER_GC_OPEN_AUCTION,
		#ifdef ENABLE_NEW_SHOP_IN_CITIES
		SUBHEADER_GC_INSERT_SHOP_ENTITY,
		SUBHEADER_GC_REMOVE_SHOP_ENTITY,
		#endif
		#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
		SUBHEADER_GC_NOTIFICATION,
		#endif

	};


	typedef struct
	{
		DWORD	dwShopCount;
	} TSubPacketGCShopList;


	typedef struct
	{
		TShopInfo	shop;

	} TSubPacketGCShopOpen;


	typedef struct
	{
		TShopInfo	shop;
		DWORD		dwSoldCount;
		DWORD		dwOfferCount;

	} TSubPacketGCShopOpenOwner;



	typedef struct
	{
		DWORD dwOwnerID;
		DWORD dwItemID;
	} TSubPacketGCShopBuyItemFromSearch;


	typedef struct
	{
		DWORD dwCount;
	} TSubPacketGCShopFilterResult;


	typedef struct
	{
		DWORD dwOfferCount;

	} TSubPacketGCShopOfferList;



	typedef struct
	{
		TValutesInfo	valute;
		DWORD			dwItemCount;

	} TSubPacketGCShopSafeboxRefresh;

	//AUCTION
	typedef struct
	{
		DWORD dwCount;
	} TSubPacketGCAuctionList;



	typedef struct
	{
		TAuctionInfo auction;
		DWORD dwOfferCount;


	} TSubPacketGCAuctionOpen;
	#ifdef ENABLE_NEW_SHOP_IN_CITIES
	typedef struct
	{
		DWORD	dwVID;
		char	szName[OFFLINE_SHOP_NAME_MAX_LEN];
		int		iType;

		long 	x, y, z;
	} TSubPacketGCInsertShopEntity;


	typedef struct
	{
		DWORD dwVID;
	} TSubPacketGCRemoveShopEntity;
	#endif

	#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
	typedef struct
	{
		DWORD dwItemID;
		long long dwItemPrice;
		WORD dwItemCount;
	} TSubPacketGCNotification;

	typedef struct
	{
		DWORD dwTime;
	} TSubPacketCGShopExtendTime;
	#endif

	// CLIENT TO GAME
	enum eSubHeaderCG
	{
		SUBHEADER_CG_SHOP_CREATE_NEW,
		SUBHEADER_CG_SHOP_CHANGE_NAME,
		#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
		SUBHEADER_CG_SHOP_EXTEND_TIME,
		#endif
		SUBHEADER_CG_SHOP_FORCE_CLOSE,
		SUBHEADER_CG_SHOP_REQUEST_SHOPLIST,
		SUBHEADER_CG_SHOP_OPEN,
		SUBHEADER_CG_SHOP_OPEN_OWNER,
		SUBHEADER_CG_SHOP_BUY_ITEM,

		SUBHEADER_CG_SHOP_ADD_ITEM,
		SUBHEADER_CG_SHOP_REMOVE_ITEM,
		SUBHEADER_CG_SHOP_EDIT_ITEM,

		SUBHEADER_CG_SHOP_FILTER_REQUEST,

		SUBHEADER_CG_SHOP_OFFER_CREATE,
		SUBHEADER_CG_SHOP_OFFER_ACCEPT,
		SUBHEADER_CG_SHOP_OFFER_CANCEL,
		SUBHEADER_CG_SHOP_REQUEST_OFFER_LIST,

		SUBHEADER_CG_SHOP_SAFEBOX_OPEN,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_ITEM,
		SUBHEADER_CG_SHOP_SAFEBOX_GET_VALUTES,
		SUBHEADER_CG_SHOP_SAFEBOX_CLOSE,

		//AUCTION
		SUBHEADER_CG_AUCTION_LIST_REQUEST,
		SUBHEADER_CG_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_MY_AUCTION_OPEN_REQUEST,
		SUBHEADER_CG_CREATE_AUCTION,
		SUBHEADER_CG_AUCTION_ADD_OFFER,
		SUBHEADER_CG_EXIT_FROM_AUCTION,

		SUBHEADER_CG_CLOSE_BOARD,
		#ifdef ENABLE_NEW_SHOP_IN_CITIES
		SUBHEADER_CG_CLICK_ENTITY,
		#endif

	};




	typedef struct
	{
		TShopInfo shop;
	} TSubPacketCGShopCreate;


	typedef struct
	{
		char szName[OFFLINE_SHOP_NAME_MAX_LEN];
	} TSubPacketCGShopChangeName;



	typedef struct
	{
		DWORD dwOwnerID;
	} TSubPacketCGShopOpen;



	typedef struct
	{
		TItemPos	pos;
		TPriceInfo  price;
	} TSubPacketCGAddItem;


	typedef struct
	{
		DWORD dwItemID;
	} TSubPacketCGRemoveItem;



	typedef struct
	{
		DWORD dwItemID;
		TPriceInfo price;
	} TSubPacketCGEditItem;



	typedef struct
	{
		TFilterInfo filter;
	} TSubPacketCGFilterRequest;



	typedef struct
	{
		TOfferInfo offer;
	} TSubPacketCGOfferCreate;


	typedef struct
	{
		DWORD dwOfferID;
	} TSubPacketCGOfferAccept;



	typedef struct
	{
		DWORD dwOfferID;
		DWORD dwOwnerID;
	} TSubPacketCGOfferCancel;


	typedef struct
	{
		DWORD dwItemID;
	} TSubPacketCGShopSafeboxGetItem;


	typedef struct
	{
		TValutesInfo valutes;
	} TSubPacketCGShopSafeboxGetValutes;

	typedef struct
	{
		DWORD dwOwnerID;
		DWORD dwItemID;
		bool  bIsSearch;
	} TSubPacketCGShopBuyItem;

	//AUCTION
	typedef struct
	{
		DWORD dwOwnerID;
	} TSubPacketCGAuctionOpenRequest;

	typedef struct
	{
		DWORD		dwDuration;
		TItemPos	pos;

		TPriceInfo	init_price;
	} TSubPacketCGAuctionCreate;


	typedef struct
	{
		DWORD		dwOwnerID;
		TPriceInfo	price;
	} TSubPacketCGAuctionAddOffer;

	typedef struct
	{
		DWORD dwOwnerID;
	} TSubPacketCGAuctionExitFrom;

	#ifdef ENABLE_NEW_SHOP_IN_CITIES
	typedef struct
	{
		DWORD dwShopVID;
	} TSubPacketCGShopClickEntity;
	#endif
}
#endif

#ifdef ENABLE_NEW_OFFLINESHOP_RENEWAL
typedef struct SPacketGGOfflineShopNotification
{
	BYTE		bHeader;
	int			ownerId;
	int			vnum;
	long long	price;
	WORD		count;
} TPacketGGOfflineShopNotification;
#endif

#ifdef ENABLE_NEW_RANKING_SYSTEM
typedef struct SDynamicPacketInfo
{
    BYTE    bHeader;
    WORD    wSize;
    BYTE    bSubHeader;
} TDynamicPacketInfo;

/* game to client */
enum ERankingSystemSubHeader : BYTE
{
	RANKING_SYSTEM_SUBHEADER_GC_LOAD_RANKINGS,
	RANKING_SYSTEM_SUBHEADER_GC_CATEGORIES,
};

/* client to game */
enum ERankingSystemCGSubHeader : BYTE
{
	RANKING_SYSTEM_SUBHEADER_CG_LOAD,
	// RANKING_SYSTEM_SUBHEADER_CG_REWARD,
};
using TRankingSystemCGLoadInfo = struct SRankingSystemCGLoadInfo
{
	uint8_t bCategory;
};
#endif

#ifdef ENABLE_ANTI_MULTIPLE_FARM
enum EAntiFarmSubHeader
{
	AF_SH_SENDING_DATA,
	AF_SH_SEND_STATUS_UPDATE,
	AF_SH_SENDING_CONFIRM_DATA,
};

typedef struct SAntiFarmPlayerInfo
{
	SAntiFarmPlayerInfo(DWORD dwPID, bool bDropStatus) :
		dwPID(dwPID), bDropStatus(bDropStatus)
		{ memset(szName, 0, sizeof(szName)); }
	
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	bool	bDropStatus;
} TAntiFarmPlayerInfo;

typedef struct SSendAntiFarmInfo
{
	SSendAntiFarmInfo(BYTE header, int size, BYTE subheader) : 
		header(header), size(size), subheader(subheader)
		{}
	
	BYTE	header;
	int		size;
	BYTE	subheader;
} TSendAntiFarmInfo;
#endif

#ifdef ENABLE_CSHIELD
typedef struct SPacketGCCShield
{
	BYTE	header;
	bool	start;
	DWORD	num;
} TPacketGCCShield;

typedef struct SPacketCGCShield
{
	BYTE	header;
	char	key[64 + 1];
	char	data[256 + 1];
} TPacketCGCShield;

typedef struct SPacketCGCShieldData
{
	BYTE	header;
	char	version[10 + 1];
	char	binary_name[255 + 1];
	char	binary_hash[10 + 1];
} TPacketCGCShieldData;

#ifdef ENABLE_EXTERNAL_REPORTS
typedef struct SPacketCGCShieldReport
{
	BYTE	header;
	DWORD	errorcode;
	BYTE	win;
	char	language[3 + 1];
	char	cshieldHwid[128 + 1];
	char	report[1024 + 1];
} TPacketCGCShieldReport;
#endif
#endif

#ifdef ENABLE_CAPTCHA
typedef struct SPacketGCCaptcha
{
	BYTE	header;
	char	captchaImage[4096];
} TPacketGCCaptcha;

typedef struct SPacketCGCaptcha
{
	BYTE	header;
	short	captchaResult;
} TPacketCGCaptcha;
#endif

#ifdef ENABLE_LOG_NOTIFICATIONS
struct TPacketGGCShieldLogNotification {
	BYTE    bHeader;
	char	szText[CHAT_MAX_LEN + 1];
};
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
enum
{
	CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_LOADING,

	CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM,
	CUBE_RENEWAL_SUB_HEADER_CLOSE,
};

typedef struct  packet_send_cube_renewal
{
	BYTE header;
	BYTE subheader;
	DWORD index_item;
	DWORD count_item;
	DWORD index_item_improve;
}TPacketCGCubeRenewalSend;

typedef struct dates_cube_renewal
{
	DWORD npc_vnum;
	DWORD index;

	DWORD	vnum_reward;
	int		count_reward;

	bool 	item_reward_stackable;

	DWORD	vnum_material_1;
	int		count_material_1;

	DWORD	vnum_material_2;
	int		count_material_2;

	DWORD	vnum_material_3;
	int		count_material_3;

	DWORD	vnum_material_4;
	int		count_material_4;

	DWORD	vnum_material_5;
	int		count_material_5;

#ifdef FULL_YANG
	long long 	gold;
#else
	int 	gold;
#endif
	int 	percent;

	char 	category[100];
}TInfoDateCubeRenewal;

typedef struct packet_receive_cube_renewal
{
	packet_receive_cube_renewal() : header(HEADER_GC_CUBE_RENEWAL)
	{}

	BYTE header;
	BYTE subheader;
	TInfoDateCubeRenewal	date_cube_renewal;
}TPacketGCCubeRenewalReceive;
#endif

#ifdef ENABLE_INGAME_ITEMSHOP
typedef struct packet_TPacketCGItemShop
{
	uint8_t header;
	uint16_t size;
	uint8_t subHeader;
} TPacketCGItemShop;

typedef struct packet_TPacketCGItemShopBuy
{
	uint16_t id;
	uint16_t count;
} TPacketCGItemShopBuy;

typedef struct packet_TPacketCGItemShopRequestCount
{
	uint16_t id;
} TPacketCGItemShopRequestCount;

enum
{
	IS_GC_SUB_CATEGORIES = 0,
	IS_GC_SUB_ITEMS,
	IS_GC_SUB_COINS,
	IS_GC_SUB_OPEN,
	IS_GC_SUB_COUNT,

	IS_CG_SUB_BUY = 0,
	IS_CG_SUB_REQUEST_COUNT,

	IS_GG_RELOAD = 0,
	IS_GG_BUY_LIMITED,
};

typedef struct packet_itemshop_item
{
	uint16_t id;
	uint32_t vnum;
	uint8_t category;
	uint32_t coins;
	long	sockets[ITEM_SOCKET_MAX_NUM];
	bool limited;
	long limitedTime;
	uint32_t limitedCount;
} TItemShopItem;

typedef struct packet_itemshop_category
{
	uint8_t id;
	char name[40 + 1];
	uint8_t main_category;
	uint8_t color_id;
} TItemShopCategory;

typedef struct
{
	uint8_t header;
	uint16_t size;
	uint8_t subHeader;
} TPacketGCItemShop;

typedef struct
{
	uint16_t cat_count;
} TPacketItemShopCategories;

typedef struct
{
	bool clear;
	uint16_t item_count;
} TPacketItemShopItems;

typedef struct
{
	uint32_t gold;
	uint32_t silver;
} TPacketItemShopCoins;

typedef struct
{
	uint8_t header;
	uint8_t subheader;
} TPacketItemShopGG;

typedef struct
{
	uint16_t id;
	uint16_t count;
} TPacketItemShopLimitedItem;
#endif

#if defined(__ENABLE_RIDING_EXTENDED__)
typedef struct SPacketCGMountUpGrade
{
	uint8_t					header;
	uint8_t					iSubHeader;
} TPacketCGMountUpGrade;

typedef struct SPacketGCMountUpGrade
{
	uint8_t					header;
	uint8_t					subheader;
	uint8_t					horse_level;
} TPacketGCMountUpGrade;

typedef struct SPacketGCMountUpGradeChat
{
	uint8_t					header;
	uint8_t					type;
	uint16_t				value;
} TPacketGCMountUpGradeChat;
#endif

#ifdef __ENABLE_STAGE_SYSTEM__
typedef struct SPacketGCStageSystem
{
	BYTE header;
	uint8_t stage_level;
	int countdown_timer;
} TPacketGCStageSystem;

typedef struct SPacketCGStageUpgrade
{
	BYTE header;
	BYTE item_pos;
} TPacketCGStageUpgrade;
#endif

#pragma pack()
#endif
