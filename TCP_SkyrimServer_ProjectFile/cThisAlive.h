#pragma once
#include "cCompositeSimple.h"
class cThisAlive : public cCompositeSimple
{
public:
	cThisAlive(void);
	~cThisAlive(void);
	void Process( stEnemyInfo* target );
};
