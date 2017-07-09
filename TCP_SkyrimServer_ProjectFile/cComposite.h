#pragma once

//#include "cEnemyCharacter.h"
#include "defines.h"

class cComposite
{
public:
	cComposite(void);
	virtual ~cComposite(void);
	virtual void Process( stEnemyInfo* target )  = 0;
};
