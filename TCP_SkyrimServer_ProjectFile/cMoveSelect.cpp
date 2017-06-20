#include "StdAfx.h"
#include "cMoveSelect.h"
#include "cMove_Chase.h"
#include "cMove_BacktoStart.h"
cMoveSelect::cMoveSelect(void)
{
	cComposite* newData;
	newData = new cMove_Chase;
	Add( newData );

	newData = new cMove_BacktoStart;
	Add( newData );
}

cMoveSelect::~cMoveSelect(void)
{


	
}

void cMoveSelect::Process( stEnemyInfo* target )
{
	if(target->pTarget == NULL)
	{
		Data[1]->Process(target);
	}
	else
	{
		Data[0]->Process(target);
	}
}