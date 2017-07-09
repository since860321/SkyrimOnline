#include "cDragonAttack.h"
#include "cWinSocketServer.h"

cDragonAttack::cDragonAttack(void)
{
}

cDragonAttack::~cDragonAttack(void)
{
}

void cDragonAttack::Process( stEnemyInfo* target )
{
	DWORD dwTime = GetTickCount();

	if(target->dwAttackTime	<	dwTime)
	{	
		target->dwAttackTime = dwTime + target->dwAttackCoolTime;
		
		sgNetwork.SendToServer(target->pTarget->nID,target->dwAttackDamage);

		target->nAnimationIndex = (int)D_ground_bite;

	}
}