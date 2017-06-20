#pragma once

#include "cCompositeSimple.h"

class cDragonAttack : public cCompositeSimple
{
public:
	cDragonAttack(void);
	~cDragonAttack(void);
	void Process( stEnemyInfo* target );
};
