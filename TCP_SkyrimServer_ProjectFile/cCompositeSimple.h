#pragma once 

#include "cComposite.h"

class cCompositeSimple : public cComposite
{
public:
	cCompositeSimple(void);
	virtual ~cCompositeSimple(void);
	virtual void Process( stEnemyInfo* target )  = 0; 
};
