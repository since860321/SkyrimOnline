#pragma once // 상태패턴상속용 클래스 AI의 종류를구현


#include "cComposite.h"

class cAIStatus
{
protected:
	cComposite* Composite;
public:
	cAIStatus(void);
	virtual ~cAIStatus(void);
	virtual void AISet(stClientInfo*  PlayerCharacterInfo,stEnemyInfo*  MonsterInfo) = 0;
};
