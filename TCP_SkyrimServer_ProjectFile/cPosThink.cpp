#include "cPosThink.h"

cPosThink::cPosThink(void)
{
}

cPosThink::~cPosThink(void)
{
}

void cPosThink::Process( stEnemyInfo* targetMonster )
{
#ifdef _S_MOD_D3DX9_API_CUSTOM_
#else //_S_MOD_D3DX9_API_CUSTOM_
	if(targetMonster->pTarget == NULL) return;

	D3DXVECTOR3 vStarttoTarget =targetMonster->pTarget->transformInfo.vPos - targetMonster->vStartPos;

	float StartLength = vStarttoTarget.x * vStarttoTarget.x + vStarttoTarget.y * vStarttoTarget.y + vStarttoTarget.z * vStarttoTarget.z;

	if(StartLength  >	targetMonster->fPerceptionLength * targetMonster->fPerceptionLength * 2)
	{
		targetMonster->pTarget = NULL;
		return;
	}	
#endif //_S_MOD_D3DX9_API_CUSTOM_
}