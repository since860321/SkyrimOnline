#pragma once

#include "cCompositeSimple.h"

class cPosThink : public cCompositeSimple
{
public:
	cPosThink(void);
	~cPosThink(void);

	
	void Process( stEnemyInfo* target );
};
