#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cCompositeSimple.h"

class cPosThink : public cCompositeSimple
{
public:
	cPosThink(void);
	~cPosThink(void);

	
	void Process( stEnemyInfo* target );
};
