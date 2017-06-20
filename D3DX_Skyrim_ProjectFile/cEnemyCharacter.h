#pragma once
#include "cCharacter.h"

class cEnemyCharacter : public cCharacter
{
	stMonsterInfo m_stEnemyData;
public:
	cEnemyCharacter(void);
	virtual ~cEnemyCharacter(void);
	void Control();

	void Update();//<<���� ������Ʈ �ȿ� ���� ������Ʈ �����ø�˴ϴ�.

	stMonsterInfo* GetMonsterInfo()	{return &m_stEnemyData;}
	void SetMonsterInfo(stMonsterInfo EnemyData) {m_stEnemyData = EnemyData;}

	void HPDamage(int nDamage); // ���Ϳ��� �����ϴ� �Լ� - ���Լ��� ���� ������� �ش�.
};

