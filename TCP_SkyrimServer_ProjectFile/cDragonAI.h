#pragma once
#include "cAIStatus.h"
class cDragonAI  : public cAIStatus
{
public:
	cDragonAI(void);
	~cDragonAI(void);
	void AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster);
};
