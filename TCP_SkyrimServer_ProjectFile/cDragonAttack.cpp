#include "cDragonAttack.h"
#include "cWinSocketServer.h"

cDragonAttack::cDragonAttack(void)
{
}

cDragonAttack::~cDragonAttack(void)
{
}

void cDragonAttack::Process( stEnemyInfo* target )
{
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
	DWORD dwTime = GetTickCount();

	if(target->dwAttackTime	<	dwTime)
	{	
		target->dwAttackTime = dwTime + target->dwAttackCoolTime;

#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
		sgNetwork.SendToServer(target->pTarget->nID,target->dwAttackDamage);
#endif //_S_LINUX_EPOLL_

		target->nAnimationIndex = (int)D_ground_bite;

	}
#endif //_S_LINUX_EPOLL_
}