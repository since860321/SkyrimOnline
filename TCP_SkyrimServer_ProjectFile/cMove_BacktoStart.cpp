#include "defines.h"
#include "cMove_BacktoStart.h"


cMove_BacktoStart::cMove_BacktoStart(void)
{
}

cMove_BacktoStart::~cMove_BacktoStart(void)
{
}


void cMove_BacktoStart::Process( stEnemyInfo* targetMonster )
{
	D3DXVECTOR3 vTarget	=	targetMonster->vStartPos;
	D3DXVECTOR3 vMoster	=	targetMonster->vPos;
	D3DXVECTOR3 vPos = vTarget - vMoster;

	if(vPos.x * vPos.x + vPos.y * vPos.y+ vPos.z * vPos.z < targetMonster->fMoveSpeed * targetMonster->fMoveSpeed)
	{
		targetMonster->vPos = vTarget;
		targetMonster->vTargetPos	=	vTarget;
		
	}
	else
	{
	D3DXVec3Normalize(&vPos,&vPos);

	

	targetMonster->vPos += vPos  * (targetMonster->fMoveSpeed);
	targetMonster->vTargetPos	=	vTarget;
	
	}
	
	if(targetMonster->vPos.y < 100)
	{
	targetMonster->nAnimationIndex = (int)D_mtforwardground;
	}
	else
	{
	targetMonster->nAnimationIndex = (int)D_mtidle_hover;
	}



}