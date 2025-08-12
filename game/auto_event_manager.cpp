#include "stdafx.h"
#ifdef ENABLE_AUTO_EVENT_MANAGER
#include "config.h"
#include "auto_event_manager.h"
#ifdef ENABLE_EVENT_CONTROL
#include "eventControl.h"
#endif
#ifdef ENABLE_CAOS_EVENT
	#include "NewCaosEvent.h"
#endif

static LPEVENT running_event = NULL;

EVENTINFO(EventsManagerInfoData)
{
	CAutoEventSystem* pEvents;

	EventsManagerInfoData()
		: pEvents(0)
	{
	}
};

EVENTFUNC(automatic_event_timer)
{
	if (event == NULL)
		return 0;

	if (event->info == NULL)
		return 0;

	EventsManagerInfoData * info = dynamic_cast<EventsManagerInfoData*>(event->info);

	if (info == NULL)
		return 0;

	auto * pInstance = info->pEvents;

	if (pInstance == NULL) { return 0; }

	CAutoEventSystem::instance().PrepareChecker();
	return PASSES_PER_SEC(60);
}

void CAutoEventSystem::PrepareChecker()
{
#ifdef ENABLE_EVENT_CONTROL
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);

[[maybe_unused]] int day = vKey.tm_wday;
[[maybe_unused]] int hour = vKey.tm_hour;
[[maybe_unused]] int minute = vKey.tm_min;
[[maybe_unused]] int second = vKey.tm_sec;
	// if (g_bChannel == 99)
		// CEventController::instance().CheckEvent(day, hour);
#endif
#ifdef ENABLE_CAOS_EVENT
	CNewCaosEventManager::instance().CheckEvent(day, hour, minute);
#endif
}

void CAutoEventSystem::Check(int day, int hour, int minute, int second)
{
	return;
}

bool CAutoEventSystem::Initialize()
{
	if (running_event != NULL)
	{
		event_cancel(&running_event);
		running_event = NULL;
	}

	auto* info = AllocEventInfo<EventsManagerInfoData>();
	info->pEvents = this;

	running_event = event_create(automatic_event_timer, std::move(info), PASSES_PER_SEC(30));
	return true;
}



void CAutoEventSystem::Destroy()
{
	if (running_event != NULL)
	{
		event_cancel(&running_event);
		running_event = NULL;
	}
}
#endif