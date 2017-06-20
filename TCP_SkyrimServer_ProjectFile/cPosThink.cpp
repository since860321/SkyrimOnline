#include "StdAfx.h"
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

	D3DXVECTOR3 vStarttoTarget =targetMonster->pTarget->transformInfo.vPos - targetMonster->vStartPos;
	
	float StartLength = vStarttoTarget.x * vStarttoTarget.x + vStarttoTarget.y * vStarttoTarget.y + vStarttoTarget.z * vStarttoTarget.z;

	if(StartLength  >	targetMonster->fPerceptionLength * targetMonster->fPerceptionLength * 2)
	{
		targetMonster->pTarget = NULL;
		return;
	}	

}