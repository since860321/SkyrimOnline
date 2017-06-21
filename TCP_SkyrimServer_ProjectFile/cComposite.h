#pragma once

//#include "cEnemyCharacter.h"
#include "stdafx.h"

class cComposite
{
public:
	cComposite(void);
	virtual ~cComposite(void);
	virtual void Process( stEnemyInfo* target )  = 0;
};
