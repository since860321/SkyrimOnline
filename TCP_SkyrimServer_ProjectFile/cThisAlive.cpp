#include "StdAfx.h"
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
			targetMonster->dwGenerationTime	=	GetTickCount() + targetMonster->dwGenerationCoolTime; 
			targetMonster->nAnimationIndex = (int)D_mtidle_hover;

			stEnemyTransportInfo sInfo;
			sInfo.bAlive			=	targetMonster->bAlive;
			sInfo.nAnimationIndex	=	targetMonster->nAnimationIndex;
			sInfo.nEnemyIndex = targetMonster->nEnemyIndex;
			sInfo.nHpReduce = targetMonster->dwHpReduce;
			sInfo.vPos = targetMonster->vPos;
			sInfo.vTarget = targetMonster->vTargetPos;
			sgNetwork.SendToServer( sInfo );

			targetMonster->vPos			=	targetMonster->vStartPos;
			targetMonster->vPos.y		=	1000;
			targetMonster->dwHpReduce	=	0;
			return;
		}
	}
	else
	{
		if(targetMonster->dwGenerationTime	<	GetTickCount())
		{
			targetMonster->bAlive =  true;
		}
	}
}