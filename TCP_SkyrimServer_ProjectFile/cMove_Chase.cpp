#include "StdAfx.h"
#include "cMove_Chase.h"

cMove_Chase::cMove_Chase(void)
{
}

cMove_Chase::~cMove_Chase(void)
{
}

void cMove_Chase::Process( stEnemyInfo* targetMonster )
{

	D3DXVECTOR3 vTarget	=	targetMonster->pTarget->transformInfo.vPos;
	D3DXVECTOR3 vMoster	=	targetMonster->vPos;
	D3DXVECTOR3 vPos = vTarget - vMoster;
	D3DXVec3Normalize(&vPos,&vPos);

	targetMonster->vTargetPos	=	vTarget;
	targetMonster->vPos += vPos * targetMonster->fMoveSpeed ;
	
	if(targetMonster->vPos.y < 100)
	{
	targetMonster->nAnimationIndex = (int)D_mtforwardground;
	}
	else
	{
	targetMonster->nAnimationIndex = (int)D_mtfastforward_flap;
	}
}