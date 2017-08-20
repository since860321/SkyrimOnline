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
#ifdef _S_MOD_D3DX9_API_CUSTOM_
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vToCharacter = (target->pTarget->transformInfo.vPos) - (target->vPos);

	float fToCharacterLength  = vToCharacter.x * vToCharacter.x + vToCharacter.y * vToCharacter.y + vToCharacter.z * vToCharacter.z;

	float fAttackLange = target->fAttackLange * target->fAttackLange;

#ifdef _S_LINUX_EPOLL_
	unsigned int dwTime;// = GetTickCount();
#else //_S_LINUX_EPOLL_
	DWORD dwTime = GetTickCount();
#endif //_S_LINUX_EPOLL_

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
#endif //_S_MOD_D3DX9_API_CUSTOM_
}