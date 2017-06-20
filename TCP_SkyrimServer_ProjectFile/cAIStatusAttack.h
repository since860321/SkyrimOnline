#pragma once // 공격적몬스터 AI

#include "cAIStatus.h"



class cAIStatusAttack : public cAIStatus
{
public:
	cAIStatusAttack(void);
	~cAIStatusAttack(void);
	void AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster);
};
