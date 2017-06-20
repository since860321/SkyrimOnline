#pragma once
#include "cCompositeSimple.h"
class cTargetalive : public cCompositeSimple
{
public:
	cTargetalive(void);
	~cTargetalive(void);
	void Process( stEnemyInfo* target );
};

