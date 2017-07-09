#include "cDragonThink.h"
#include "cThisAlive.h"
#include "cTargetalive.h"
#include "cStatusThink.h"
#include "cPosThink.h"


cDragonThink::cDragonThink(void)
{
	cComposite* newData;
	
	newData = new cThisAlive;
	Add( newData );

	newData = new cTargetalive;
	Add( newData );

	newData = new cPosThink;
	Add( newData );

	newData = new cStatusThink;
	Add( newData );

}

cDragonThink::~cDragonThink(void)
{

}

void cDragonThink::Process( stEnemyInfo* target )
{

	Data[0]->Process(target);

	if(target->bAlive == false) return;

	for(int i = 1; i < 4 ;++i)
	{
		Data[i]->Process(target);

	}
}
