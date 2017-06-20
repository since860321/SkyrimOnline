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

	cCharacter*		m_pPlayer;//< ĳ���� 
	cCharacter*		m_pEnemy;//< ĳ���� 


public:
	cStateBar(void);
	~cStateBar(void);
	void Setup(IN cResourceMgr*	pRes,cCharacter* Character,cCharacter* Enemy);
	void PlayerRender();
	void EnemyRender();
};


//������ 
//ü�¹�
//���׹̳� ��
//��ħ��
//�Ҹ� ����
//�����¹�
//�޼����ڽ�