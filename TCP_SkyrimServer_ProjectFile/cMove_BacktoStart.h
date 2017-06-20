#pragma once

#include "cCompositeSimple.h"

class cMove_BacktoStart  : public cCompositeSimple
{
public:
	cMove_BacktoStart(void);
	 ~cMove_BacktoStart(void);
	void Process( stEnemyInfo* target );
};
