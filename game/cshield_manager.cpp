#include "stdafx.h"
#include "config.h"

#ifdef ENABLE_CSHIELD
#include "cshield_manager.h"

static LPEVENT cshield_event = NULL;
static LPEVENT captcha_event = NULL;

EVENTINFO(EventsManagerInfoData)
{
	CShieldManager* pEvents;

	EventsManagerInfoData()
		: pEvents(0)
	{
	}
};

#ifdef AUTOUPDATE_INTERVAL
EVENTFUNC(cshield_event_timer)
{
	if (cshield_event == NULL)
		return 0;

	if (cshield_event->info == NULL)
		return 0;

	EventsManagerInfoData* info = dynamic_cast<EventsManagerInfoData*>(cshield_event->info);

	if (info == NULL)
		return 0;

	CShieldManager* pInstance = info->pEvents;

	if (pInstance == NULL)
		return 0;

	CShieldManager::instance().CShieldUpdate();
	return PASSES_PER_SEC(AUTOUPDATE_INTERVAL);
}
#endif

#ifdef AUTOUPDATE_CAPTCHA_INTERVAL
EVENTFUNC(captcha_event_timer)
{
	if (captcha_event == NULL)
		return 0;

	if (captcha_event->info == NULL)
		return 0;

	EventsManagerInfoData* info = dynamic_cast<EventsManagerInfoData*>(captcha_event->info);

	if (info == NULL)
		return 0;

	CShieldManager* pInstance = info->pEvents;

	if (pInstance == NULL)
		return 0;

	CShieldManager::instance().CShieldCaptchaUpdate();
	return PASSES_PER_SEC(AUTOUPDATE_CAPTCHA_INTERVAL * 60 * 60);
}
#endif

void CShieldManager::CShieldUpdate()
{
#ifdef SUPPRESS_UPDATE_OUTPUT
	UpdateCShield(VERIFICATION_KEY, true);
#else
	UpdateCShield(VERIFICATION_KEY, false);
#endif
}

void CShieldManager::CShieldCaptchaUpdate()
{
#ifdef SUPPRESS_UPDATE_OUTPUT
	UpdateCaptcha(VERIFICATION_KEY, true);
#else
	UpdateCaptcha(VERIFICATION_KEY, false);
#endif
}

bool CShieldManager::Initialize()
{
	if (cshield_event != NULL)
	{
		event_cancel(&cshield_event);
		cshield_event = NULL;
	}

	EventsManagerInfoData* info = AllocEventInfo<EventsManagerInfoData>();
	info->pEvents = this;

	InitCShield(VERIFICATION_KEY);

#ifdef AUTOUPDATE_INTERVAL
	cshield_event = event_create(cshield_event_timer, info, PASSES_PER_SEC(AUTOUPDATE_INTERVAL));
#endif
#ifdef AUTOUPDATE_CAPTCHA_INTERVAL
	captcha_event = event_create(captcha_event_timer, info, PASSES_PER_SEC(AUTOUPDATE_CAPTCHA_INTERVAL * 60 * 60));
#endif
	return true;
}

void CShieldManager::Destroy()
{
	if (cshield_event != NULL)
	{
		event_cancel(&cshield_event);
		cshield_event = NULL;
	}

	if (captcha_event != NULL)
	{
		event_cancel(&captcha_event);
		captcha_event = NULL;
	}
}
#endif
