#pragma once // �������ϻ�ӿ� Ŭ���� AI�� ����������


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
