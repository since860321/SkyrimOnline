#pragma once
#include "constants.h"

class cInventory;
class cCharacter;
class cStateBar;
class cResourceMgr;
class CSprite;
class cMenuBar
{
private:
	cResourceMgr*	m_pRes;
	CSprite*		m_pCsprite;
	cStateBar*		m_pStateBar;

	cInventory*		m_Inven;
	cCharacter*		m_pPlyer;

	int				m_MenuIndex;
	int				m_ItemIndex;


	bool			m_bImage;	 // �̹��� ��뿩��
	bool			m_bToggle;   // ���



public:
	cMenuBar(void);
	~cMenuBar(void);

	void Setup(IN cResourceMgr*	pRes,cCharacter* Player,cInventory*		Inven,cStateBar* pState);
	void Render();
	void MenuSelect();
	void MenuItem();

	void SetbImage(bool Image){m_bImage = Image;}
	void SetMenuInex(int Inex){m_MenuIndex = Inex;}

};

//���콺�� ��ġ�� ��� TAP�� ������ ���콺 ��ġ�� �����ؼ�
//�� �� 
//�� �� ����
//���ҽ��� ����?
