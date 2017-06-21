#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"

class cMove_Chase : public cCompositeSimple
{
public:
	cMove_Chase(void);
	 ~cMove_Chase(void);

	void Process( stEnemyInfo* target );
};
