#pragma once

#include "cCompositeComplex.h"

class cStatusThink : public cCompositeComplex
{
public:
	cStatusThink(void);
	~cStatusThink(void);
	void Process( stEnemyInfo* target );
};
