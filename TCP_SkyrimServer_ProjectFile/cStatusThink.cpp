#include "cStatusThink.h"
#include "cDragonBattle.h"
#include "cDragonReady.h"

#ifndef UINT_MAX
#define UINT_MAX ((unsigned int)-1)
#endif  

cStatusThink::cStatusThink(void)
{
	cComposite* newData;
	
	newData = new cDragonBattle;
	Add( newData );

	newData = new cDragonReady;
	Add( newData );

}

cStatusThink::~cStatusThink(void)
{
}
void cStatusThink::Process( stEnemyInfo* target )
{
	if(target->pTarget == NULL)
	{

		Data[1]->Process(target);	
	}
	else
	{
		target->dwRecoveryTime = UINT_MAX;
		if(target->vPos.y > 0) target->vPos.y = 0;
		Data[0]->Process(target);
	}
}
