#include "desc.h"

class DESC_P2P : public DESC
{
	public:
		virtual ~DESC_P2P();

		virtual void	Destroy();
		virtual void	SetPhase(int iPhase);
		bool		Setup(LPFDWATCH _fdw, socket_t fd, const char * host, WORD wPort);
};
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
