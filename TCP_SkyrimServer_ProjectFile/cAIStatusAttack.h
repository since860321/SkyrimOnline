#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
 // 공격적몬스터 AI

#include "cAIStatus.h"



class cAIStatusAttack : public cAIStatus
{
public:
	cAIStatusAttack(void);
	~cAIStatusAttack(void);
	void AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster);
};
