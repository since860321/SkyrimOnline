#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"

class cDragonAttack : public cCompositeSimple
{
public:
	cDragonAttack(void);
	~cDragonAttack(void);
	void Process( stEnemyInfo* target );
};
