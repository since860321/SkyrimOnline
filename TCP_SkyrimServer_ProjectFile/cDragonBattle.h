#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeComplex.h"

class cDragonBattle : public cCompositeComplex
{
public:
	cDragonBattle(void);
	~cDragonBattle(void);
	void Process( stEnemyInfo* target );
};
