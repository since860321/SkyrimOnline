#include "StdAfx.h"
#include "cMove_BackMove.h"

cMove_BackMove::cMove_BackMove(void)
{
}

cMove_BackMove::~cMove_BackMove(void)
{
}

void cMove_BackMove::Process( stEnemyInfo* targetMonster )
{

	D3DXVECTOR3 vTarget	=	targetMonster->pTarget->transformInfo.vPos;
	D3DXVECTOR3 vMoster	=	targetMonster->vPos;
	D3DXVECTOR3 vPos = vTarget - vMoster;
	D3DXVec3Normalize(&vPos,&vPos);

	targetMonster->vTargetPos	=	vTarget;
	targetMonster->vPos -= vPos * (targetMonster->fMoveSpeed/2) ;
	
	
	if(targetMonster->vPos.y < 100)
	{
	targetMonster->nAnimationIndex = (int)D_mtforwardground;
	}
	else
	{
	targetMonster->nAnimationIndex = (int)D_mtidle_hover;
	}
}
