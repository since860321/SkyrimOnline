#pragma once

#include "cCompositeSimple.h"

class cMove_Chase : public cCompositeSimple
{
public:
	cMove_Chase(void);
	 ~cMove_Chase(void);

	void Process( stEnemyInfo* target );
};
