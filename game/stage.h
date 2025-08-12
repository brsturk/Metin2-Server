#ifndef __STAGE_SYSTEM_H__
#define __STAGE_SYSTEM_H__

#include "char.h"
#include "affect.h"
#include "event.h"

class CStageSystem {
	public:
		static CStageSystem& Instance() {
			static CStageSystem instance;
			return instance;
		}
	
		// Oyuncunun stage seviyesini yükseltir
		void Upgrade(LPCHARACTER ch, BYTE item_pos);
	
		// Stage süresi bitmişse seviye düşürme kontrolü yapar
		void CheckStage(LPCHARACTER ch);
	
		// Girişte affectleri temizle ve sadece mevcut seviyeye ait affectleri uygular
		void ApplyStageAffects(LPCHARACTER ch);
	
		// Stage zamanlayıcısını başlatır (oyuncu online veya offline fark etmez)
		void StartStageTimer(DWORD player_id, time_t end_time);
	
	private:
		CStageSystem() {}
		CStageSystem(const CStageSystem&) = delete;
		CStageSystem& operator=(const CStageSystem&) = delete;
};

#endif // __STAGE_SYSTEM_H__