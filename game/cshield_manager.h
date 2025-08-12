#pragma once
#include "../../common/service.h"

class CShieldManager : public singleton<CShieldManager>
{
public:
	bool		Initialize();
	void		Destroy();
	void		CShieldUpdate();
	void		CShieldCaptchaUpdate();
};