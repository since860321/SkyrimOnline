#include "cPosThink.h"

cPosThink::cPosThink(void)
{
}

cPosThink::~cPosThink(void)
{
}

void cPosThink::Process( stEnemyInfo* targetMonster )
{

	if(targetMonster->pTarget == NULL) return;

#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector vStarttoTarget = targetMonster->pTarget->transformInfo.vPos - targetMonster->vStartPos;
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vStarttoTarget =targetMonster->pTarget->transformInfo.vPos - targetMonster->vStartPos;
#endif //_S_MOD_D3DX9_API_CUSTOM_

	float StartLength = vStarttoTarget.x * vStarttoTarget.x + vStarttoTarget.y * vStarttoTarget.y + vStarttoTarget.z * vStarttoTarget.z;

	if(StartLength  >	targetMonster->fPerceptionLength * targetMonster->fPerceptionLength * 2)
	{
		targetMonster->pTarget = NULL;
		return;
	}	

}