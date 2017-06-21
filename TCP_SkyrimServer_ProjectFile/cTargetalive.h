#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"
class cTargetalive : public cCompositeSimple
{
public:
	cTargetalive(void);
	~cTargetalive(void);
	void Process( stEnemyInfo* target );
};

