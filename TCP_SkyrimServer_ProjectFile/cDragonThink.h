#pragma once
#include "cCompositeComplex.h"
class cDragonThink : public cCompositeComplex
{
public:
	cDragonThink(void);
	~cDragonThink(void);
	void Process( stEnemyInfo* target );
};
