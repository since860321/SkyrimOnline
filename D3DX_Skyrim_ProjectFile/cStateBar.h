#pragma once
#include "constants.h"


class cCharacter;
class CSprite;
class cResourceMgr;
class cStateBar
{
private:
	cResourceMgr*	m_pRes;
	CSprite*		m_pCsprite;

	cCharacter*		m_pPlayer;//< 캐릭터 
	cCharacter*		m_pEnemy;//< 캐릭터 


public:
	cStateBar(void);
	~cStateBar(void);
	void Setup(IN cResourceMgr*	pRes,cCharacter* Character,cCharacter* Enemy);
	void PlayerRender();
	void EnemyRender();
};


//레벨바 
//체력바
//스테미너 바
//나침반
//소모량 개수
//적상태바
//메세지박스