#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"
class cThisAlive : public cCompositeSimple
{
public:
	cThisAlive(void);
	~cThisAlive(void);
	void Process( stEnemyInfo* target );
};
