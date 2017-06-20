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


	bool			m_bImage;	 // 이미지 사용여부
	bool			m_bToggle;   // 토글



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

//마우스의 위치를 잡고 TAP을 누르면 마우스 위치를 저장해서
//양 옆 
//상 하 판정
//리소스는 직접?
