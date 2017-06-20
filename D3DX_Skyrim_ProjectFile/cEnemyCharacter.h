#pragma once
#include "cCharacter.h"

class cEnemyCharacter : public cCharacter
{
	stMonsterInfo m_stEnemyData;
public:
	cEnemyCharacter(void);
	virtual ~cEnemyCharacter(void);
	void Control();

	void Update();//<<형구 업데이트 안에 적군 업데이트 넣으시면됩니다.

	stMonsterInfo* GetMonsterInfo()	{return &m_stEnemyData;}
	void SetMonsterInfo(stMonsterInfo EnemyData) {m_stEnemyData = EnemyData;}

	void HPDamage(int nDamage); // 몬스터에게 공격하는 함수 - 이함수를 통해 대미지를 준다.
};

