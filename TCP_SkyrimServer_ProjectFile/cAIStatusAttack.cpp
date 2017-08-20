#include "cAIStatusAttack.h"
#include "cMoveSelect.h"

cAIStatusAttack::cAIStatusAttack(void)
{
	Composite = new cMoveSelect;
}

cAIStatusAttack::~cAIStatusAttack(void)
{
	delete Composite;
}

void cAIStatusAttack::AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster)
{
#ifdef _S_MOD_D3DX9_API_CUSTOM_
#else //_S_MOD_D3DX9_API_CUSTOM_	

	D3DXVECTOR3 vPos  =	PlayerCharacter->transformInfo.vPos - Monster->vPos;
	

	float fLength = vPos.x * vPos.x + vPos.y * vPos.y + vPos.z * vPos.z;

	float	fViewLength =  Monster->fPerceptionLength * Monster->fPerceptionLength;


	if(fLength < fViewLength) Monster->pTarget = PlayerCharacter;
	
	if(Monster->pTarget == NULL && StartLength  < fViewLength * 1.5f) Monster->pTarget  = PlayerCharacter;
	

	Composite->Process(Monster);
#endif //_S_MOD_D3DX9_API_CUSTOM_
}
