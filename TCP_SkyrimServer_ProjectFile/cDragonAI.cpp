#include "cDragonAI.h"
#include "cDragonThink.h"

cDragonAI::cDragonAI(void)
{
		Composite = new cDragonThink;
}

cDragonAI::~cDragonAI(void)
{
		delete Composite;
}

void cDragonAI::AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster)
{

#ifdef _S_MOD_D3DX9_API_CUSTOM_
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vStarttoTarget = PlayerCharacter->transformInfo.vPos - Monster->vStartPos;

	float StartLength = vStarttoTarget.x * vStarttoTarget.x + vStarttoTarget.y * vStarttoTarget.y + vStarttoTarget.z * vStarttoTarget.z;

	D3DXVECTOR3 vPos  =	PlayerCharacter->transformInfo.vPos - Monster->vPos;

	float fLength = vPos.x * vPos.x + vPos.y * vPos.y + vPos.z * vPos.z;

	float	fViewLength =  Monster->fPerceptionLength * Monster->fPerceptionLength;

	if(Monster->pTarget == NULL && StartLength  < fViewLength * 1.5f && fLength < fViewLength) Monster->pTarget  = PlayerCharacter;
	

	Composite->Process(Monster);
#endif //_S_MOD_D3DX9_API_CUSTOM_
}