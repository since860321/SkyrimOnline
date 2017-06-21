#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeComplex.h"

class cStatusThink : public cCompositeComplex
{
public:
	cStatusThink(void);
	~cStatusThink(void);
	void Process( stEnemyInfo* target );
};
