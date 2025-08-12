#ifndef __INC_METIN2_COMMON_DEFINES_H__
#define __INC_METIN2_COMMON_DEFINES_H__
#pragma once

/*

	Project		:	OldBase 1-99
	Beginning	:	10.03.2025
	
	Author		:	Layerswork (layerswork.com.tr)

	Last Update	:	26.07.2025

*/

enum eCommonDefines {
	MAP_ALLOW_LIMIT = 32, // 32 default
};

// ## No Touch Defines #############################################################################################################

#define _IMPROVED_PACKET_ENCRYPTION_
#ifdef _IMPROVED_PACKET_ENCRYPTION_
#define USE_IMPROVED_PACKET_DECRYPTED_BUFFER
#endif
#define __UDP_BLOCK__

// ## General Defines ##############################################################################################################

#define ENABLE_NO_MOUNT_CHECK								// Binekler için hasar sistemi
#define ENABLE_D_NJGUILD									//
#define ENABLE_FULL_NOTICE									// Official full motice
#define ENABLE_NEWSTUFF										// Official new stuff
#define ENABLE_PORT_SECURITY								// Port security
#define ENABLE_BELT_INVENTORY_EX							//
#define ENABLE_CMD_WARP_IN_DUNGEON							//
#define ENABLE_EXTEND_INVEN_SYSTEM							// 4 sayfalı envanter sistemi
#define ENABLE_MOUNT_COSTUME_SYSTEM							// Official giyilebilir binek
#define ENABLE_MOUNT_COSTUME_EX_SYSTEM						//
#define ENABLE_WEAPON_COSTUME_SYSTEM						// Official silah kostümü
#define ENABLE_PET_SYSTEM_EX								//
#define ENABLE_CHECK_SELL_PRICE								//
#define ENABLE_MOVE_CHANNEL									// Official hızlı kanal değiştirme
#define ENABLE_QUIVER_SYSTEM								// Official eldiven sistemi
#define ENABLE_REDUCED_ENTITY_VIEW							//
#define ENABLE_GUILD_TOKEN_AUTH								//
#define ENABLE_DB_SQL_LOG									//
#define __PET_SYSTEM__										// Official pet sistemi
#define USE_ACTIVE_PET_SEAL_EFFECT							//
#define PET_SEAL_ACTIVE_SOCKET_IDX 2						//
#define USE_PET_SEAL_ON_LOGIN								//
#define DISABLE_STOP_RIDING_WHEN_DIE						// 
#define ENABLE_HIGHLIGHT_NEW_ITEM							// 
#define ENABLE_EXTEND_ITEM_AWARD 							// 
#define ENABLE_QUEST_DIE_EVENT								// Die event??
#define ENABLE_QUEST_BOOT_EVENT								// Boot event??
#define ENABLE_QUEST_DND_EVENT								// Dns event??

// ## Dev Defines ###################################################################################################################

#define ENABLE_SKILL_FLAG_PARTY								// Şaman party kutsama sistemi
#define ENABLE_DUNGEON_INFO_SYSTEM							// Zindan bilgi sistemi
#define ENABLE_TARGET_INFORMATION_SYSTEM					// Mob target info
#define FULL_YANG											// Full yang
#define ENABLE_GF_ATLAS_MARK_INFO							// Official atlas mark
#define AUTO_SHOUT											// Otomatik bağırma
#define ENABLE_DISTANCE_SKILL_SELECT						// Uzaktan skill seçme sistemi		 		*	Author	:	Layerswork	*
#define ENABLE_NEW_PASSIVE_SKILL							// Pasif skill sistemi 						*	Author	:	Layerswork	*
#define ENABLE_AUTO_EVENT_MANAGER							// Otomatik event 							*	Author	:	dracarys	*
#define ENABLE_EVENT_MANAGER								// Otomatik event modülü
#define ENABLE_NEW_BIOLOG									// Uzaktan biyolog sistemi					*	Rework	:	Layerswork	*
#define ENABLE_NEW_RANKING_SYSTEM							// Sıralama sistemi
#define WJ_NEW_DROP_DIALOG									// Hızlı item sil/sat sistemi				*	Rework	:	Layerswork	*
#define __ENABLE_NEW_OFFLINESHOP__							// Ikarus Offline Shop
#define ENABLE_OFFLINESHOP_REWORK							// Grid offline shop rework
#define ENABLE_NEW_OFFLINESHOP_LOGS							// Offline shop logs
#define ENABLE_NEW_SHOP_IN_CITIES
#define ENABLE_NEW_OFFLINESHOP_RENEWAL
#define ENABLE_RING_OF_SECRETS								// Sırlar yüzüğü (Anti-Exp)
#define ENABLE_MULTISHOP									// NPC için item ile nesne satma
#define ENABLE_COSTUME_RING_SYSTEM							// Güçlendirme yüzükleri					*	Author	:	Layerswork	*
#define FULL_SET_COMMAND_RENEWAL							// Oyun yetkilileri için /full komutu düzenleme
#define RENEWAL_PICKUP_AFFECT								// Otomatik toplama sistemi
#define ENABLE_MOB_TARGET_HP								// Mob target hp
#define ENABLE_TUNGA_BLEND_AFFECT							// Şebnem etkileri affect sistemi			*	Rework	:	Layerswork	*
#define ENABLE_SPLIT_ITEMS_REWORK							// İtem bölme sistemi						*	Rework	:	Layerswork	*
#define ENABLE_ANTI_MULTIPLE_FARM							// Multi farm block
#define ENABLE_EXTENDED_ITEMNAME_ON_GROUND					// ItemName Renewal
#define ENABLE_AFFECT_POLYMORPH_REMOVE						// Dönüşümü iptal et
#define ENABLE_SHOW_CHEST_DROP								// Sandık önizleme sistemi
#define __SPECIAL_INVENTORY_SYSTEM__						// Ek envanter sistemi
#define __SORT_INVENTORY_ITEMS__							// İtem düzenleme sistemi
#define __BACK_DUNGEON__									// Zindana geri dön							*	Author	:	dracarys	*
#define __BL_CLIENT_LOCALE_STRING__							// Official locale string					*	Author	:	Mali		*
#define __BL_MULTI_LANGUAGE__								// Official multi language					*	Author	:	Mali		* 
#define __BL_MULTI_LANGUAGE_PREMIUM__						// Multilanguage premium
#define __BL_MULTI_LANGUAGE_ULTIMATE__						// Multilanguage ultimate
#define ENABLE_CUBE_RENEWAL_WORLDARD						// Official cube
#define ENABLE_CUBE_ATTR_SOCKET								// Official cube fix
#define ENABLE_EXTENDING_COSTUME_TIME						// Kostüm ve yüzük süre uzatma sistemi
#define ENABLE_SOULBIND_SYSTEM								// Nesne kilitleme
#define CHAT_SUCCESS_UPGRADE_TEXT							// +7,+8 ve +9 nesne güncelleme duyurusu
#define CHAT_SUCCESS_LEVEL_TEXT								// Max level olunca duyuru sistemi
#define RELOAD_DROP_FLASH									// reload c komutu ile mob_drop_item.txt yenilemesi
#define ENABLE_MULTI_REFINE									// Yükseltme penceresinde istenilen sayısı max 10
#define METIN_BOSS_BONUSES									// Metinlere karşı ve bosslara karşı güçlü efsunu
#define __ENABLE_STAGE_SYSTEM__								// Aşama bonusu sistemi						*	Author	:	Layerswork	*
#define ENABLE_INGAME_ITEMSHOP								// Oyuniçi nesne market 					*	Rework	:	Layerswork	*
#define __ENABLE_RIDING_EXTENDED__							// Geliştirilmiş at geliştirme sistemi		*	Rework	:	Layerswork	*


//#define ENABLE_CSHIELD
//#ifdef ENABLE_CSHIELD
//	#define AUTOUPDATE_INTERVAL 120							// Sets the update interval in seconds
//	#define VERIFICATION_KEY "P5G6NO45ZGATBUM23HLH"			// Sets the verification key
//	#define ENABLE_CHECK_ATTACKSPEED_HACK					// Checks if player is attacking too fast
//	#define ENABLE_EXTENDED_CHECK_ATTACKSPEED_HACK			// Checks if player is attacking too fast and blocks attack packets with a lower threshold
//	// #define DISABLE_ATTACKSPEED_KICK						// Disables attackspeed kick
//	#define ENABLE_CHECK_MOVESPEED_HACK						// Checks if player is moving too fast
//	#define ENABLE_CHECK_WAIT_HACK							// Checks if player is using waithack
//	#define ENABLE_CAPTCHA 20								// Captcha appears every 20 minutes
//	#ifdef ENABLE_CAPTCHA
//		#define ENABLE_RANDOM_CAPTCHA						// Captcha appears randomly between +- 10 minutes of ENABLE_CAPTCHA time
//		#define AUTOUPDATE_CAPTCHA_INTERVAL 4				// Sets the update interval in hours to receive a new set of captchas
//	#endif
//	#define ENABLE_CAPTCHA_AND_DEV_WHITELIST				// Captcha won't be displayed to account names you set via webpanel + you can run the client in dev mode without kick
//	// #define WHITELIST_IP									// Whitelist player IP in pf after client start (needs config adaptions)
//	#define REDUCED_LOGS									// Doesn't log movement speed and async reports anymore
//	// #define SUPPRESS_UPDATE_OUTPUT						// Suppresses the "CShield data updated" messages
//	#define ENABLE_LOG_NOTIFICATIONS GM_GOD					// Enables pm notifications for GM_GOD and higher
//	#ifdef ENABLE_LOG_NOTIFICATIONS
//		#define ENABLE_LOG_NOTIFICATIONS_ERROR_CODES {102, 10006, 10007, 10008, 10009, 10010, 10011, 10012} // error codes that trigger a notification
//	#endif
//	#define ENABLE_EXTERNAL_REPORTS							// Enables external reports when a players is not logged in
//#endif

// ## Fix Defines ###################################################################################################################

#define __BL_LEVEL_FIX__									// Official level update dix			*	Author	:	Mali		*
#define ENABLE_KILL_EVENT_FIX								// ???
#define SHAMAN_SKILL_FIX									// Ejderha kükremesi becerisi düzeltmesi
#define ENABLE_SKILL_WITH_POLYMORPH_FIX						// Dönüşümde skilleri kaldırır
#define ENABLE_MOUNT_CHECK_FIX								// Bineğe hızlı in/bin engeli(1sn)
#define ENABLE_GOTO_LAG_FIX
#define ENABLE_MAP_FISH_FIX									// Balıkçı adasının dışında balık avlamanın yasaklanması

// ## Config ########################################################################################################################

#ifdef ENABLE_NEW_RANKING_SYSTEM
using ULL = unsigned long long;
#define ENABLE_NEW_RANKING_SYSTEM_LOG
#define RANKING_MAX_NUM 10
#define RANKING_UPDATE_TIME 1 								// Güncelleme süresi : 1 dakika
#define RANKING_REWARD_COUNT 3
#endif

// ## Disable Defines ################################################################################################################

// #define ENABLE_CHEQUE_SYSTEM
// #define ENABLE_SHOP_USE_CHEQUE
// #define DISABLE_CHEQUE_DROP
// #define ENABLE_WON_EXCHANGE_WINDOW
// #define ENABLE_ITEM_ATTR_COSTUME
// #define ENABLE_SEQUENCE_SYSTEM
// #define ENABLE_NO_SELL_PRICE_DIVIDED_BY_5
// #define ENABLE_SYSLOG_PACKET_SENT
// #define ENABLE_DS_GRADE_MYTH
// #define ENABLE_NO_DSS_QUALIFICATION
// #define ENABLE_PLAYER_PER_ACCOUNT5

#endif
