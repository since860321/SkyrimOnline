#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeComplex.h"

class cMoveSelect : public cCompositeComplex
{
public:
	cMoveSelect(void);
	 ~cMoveSelect(void);
	void Process( stEnemyInfo* target );
};
