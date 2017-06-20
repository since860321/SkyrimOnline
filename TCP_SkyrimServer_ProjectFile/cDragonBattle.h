#pragma once

#include "cCompositeComplex.h"

class cDragonBattle : public cCompositeComplex
{
public:
	cDragonBattle(void);
	~cDragonBattle(void);
	void Process( stEnemyInfo* target );
};
