#include "cMove_Chase.h"

cMove_Chase::cMove_Chase(void)
{
}

cMove_Chase::~cMove_Chase(void)
{
}

void cMove_Chase::Process( stEnemyInfo* targetMonster )
{
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector vTarget	=	targetMonster->pTarget->transformInfo.vPos;
	vector vMoster	=	targetMonster->vPos;
	vector vPos = vTarget - vMoster;
	vector::normalize(&vPos,&vPos);

	// 코드 추가해야함	
#else //_S_MOD_D3DX9_API_CUSTOM_
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
#endif //_S_MOD_D3DX9_API_CUSTOM_
}