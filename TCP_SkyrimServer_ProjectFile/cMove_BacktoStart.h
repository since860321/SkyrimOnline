#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"

class cMove_BacktoStart  : public cCompositeSimple
{
public:
	cMove_BacktoStart(void);
	 ~cMove_BacktoStart(void);
	void Process( stEnemyInfo* target );
};
