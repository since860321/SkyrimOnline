#pragma once


#include "cCompositeSimple.h"

class cMove_BackMove : public cCompositeSimple
{
public:
	cMove_BackMove(void);
	~cMove_BackMove(void);
	void Process( stEnemyInfo* target );
};
