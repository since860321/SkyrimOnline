#pragma once

#include "cCompositeComplex.h"

class cMoveSelect : public cCompositeComplex
{
public:
	cMoveSelect(void);
	 ~cMoveSelect(void);
	void Process( stEnemyInfo* target );
};
