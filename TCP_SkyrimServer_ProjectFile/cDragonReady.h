#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeComplex.h"

class cDragonReady : public cCompositeComplex
{
public:
	cDragonReady(void);
	~cDragonReady(void);
	void Process( stEnemyInfo* target );
};
