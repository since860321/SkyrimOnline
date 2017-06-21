#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

#include "cAIStatus.h"
class cDragonAI  : public cAIStatus
{
public:
	cDragonAI(void);
	~cDragonAI(void);
	void AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster);
};
