#include "StdAfx.h"
#include "cDragonBattle.h"
#include "cMove_Chase.h"
#include "cDragonAttack.h"
#include "cMove_BackMove.h"

cDragonBattle::cDragonBattle(void)
{
	cComposite* newData;
	newData = new cMove_Chase;
	Add( newData );

	newData = new cDragonAttack;
	Add( newData );

	newData = new cMove_BackMove;
	Add( newData );

}

cDragonBattle::~cDragonBattle(void)
{
}

void cDragonBattle::Process( stEnemyInfo* target )
{
	D3DXVECTOR3 vToCharacter = (target->pTarget->transformInfo.vPos) - (target->vPos);

	float fToCharacterLength  = vToCharacter.x * vToCharacter.x + vToCharacter.y * vToCharacter.y + vToCharacter.z * vToCharacter.z;

	float fAttackLange = target->fAttackLange * target->fAttackLange;

	DWORD dwTime = GetTickCount();
	
	if(fToCharacterLength < fAttackLange/2)
	{
		Data[2]->Process(target);
	}

	if(fToCharacterLength <= fAttackLange)
	{
		Data[1]->Process(target);
	}
	else
	{
		Data[0]->Process(target);
	}

}