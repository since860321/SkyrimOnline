#pragma once

#include "cCompositeComplex.h"

class cDragonReady : public cCompositeComplex
{
public:
	cDragonReady(void);
	~cDragonReady(void);
	void Process( stEnemyInfo* target );
};
