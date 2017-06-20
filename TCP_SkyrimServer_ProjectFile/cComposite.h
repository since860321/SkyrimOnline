#pragma once

//#include "cEnemyCharacter.h"

class cComposite
{
public:
	cComposite(void);
	virtual ~cComposite(void);
	virtual void Process( stEnemyInfo* target )  = 0;
};
