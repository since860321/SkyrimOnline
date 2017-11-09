#include "cThisAlive.h"
#include "cWinSocketServer.h"

cThisAlive::cThisAlive(void)
{
}

cThisAlive::~cThisAlive(void)
{
}
void cThisAlive::Process( stEnemyInfo* targetMonster )
{

	if(targetMonster->bAlive ==  true)
	{
		if(targetMonster->dwHpReduce >= targetMonster->dwHpMax)
		{
			targetMonster->bAlive =  false;
			targetMonster->vTargetPos	=	targetMonster->vStartPos;
			targetMonster->pTarget		=	NULL;
			targetMonster->dwHp			=	targetMonster->dwHpMax;
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
			targetMonster->dwGenerationTime	=	GetTickCount() + targetMonster->dwGenerationCoolTime; 
#endif //_S_LINUX_EPOLL_
			targetMonster->nAnimationIndex = (int)D_mtidle_hover;

			stEnemyTransportInfo sInfo;
			sInfo.bAlive			=	targetMonster->bAlive;
			sInfo.nAnimationIndex	=	targetMonster->nAnimationIndex;
			sInfo.nEnemyIndex = targetMonster->nEnemyIndex;
			sInfo.nHpReduce = targetMonster->dwHpReduce;
			sInfo.vPos = targetMonster->vPos;
			sInfo.vTarget = targetMonster->vTargetPos;
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
			sgNetwork.SendToServer( sInfo );
#endif //_S_LINUX_EPOLL_

			targetMonster->vPos			=	targetMonster->vStartPos;
			targetMonster->vPos.y		=	1000;
			targetMonster->dwHpReduce	=	0;
			return;
		}
	}
	else
	{
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
		if(targetMonster->dwGenerationTime	<	GetTickCount())
		{
			targetMonster->bAlive =  true;
		}
#endif //_S_LINUX_EPOLL_
	}
}
