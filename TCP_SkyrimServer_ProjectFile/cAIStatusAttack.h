#pragma once // ���������� AI

#include "cAIStatus.h"



class cAIStatusAttack : public cAIStatus
{
public:
	cAIStatusAttack(void);
	~cAIStatusAttack(void);
	void AISet(stClientInfo*  PlayerCharacter,stEnemyInfo*  Monster);
};
