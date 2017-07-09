#include "cTargetalive.h"

cTargetalive::cTargetalive(void)
{
}

cTargetalive::~cTargetalive(void)
{
}
void cTargetalive::Process( stEnemyInfo* targetMonster )
{
	if(targetMonster->pTarget == NULL) return;

	if(targetMonster->pTarget->nHP <= 0 )
	{
		targetMonster->pTarget = NULL;
	}	

}