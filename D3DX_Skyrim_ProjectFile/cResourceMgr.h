#pragma once
#include "constants.h"

class cStateBar;
class cInventory;
class cObjFileMesh;
class cCharacter;
class cMenuBar;
class CSprite;
class cResourceMgr
{
private:
	std::vector<CSprite*>	m_vResourceMgr; 
	CSprite*	m_pSprite; 

	cMenuBar*	m_pMenuBar;
	bool		push;


	cCharacter*		m_pPlayer;//< ĳ���� 

	int nWidth;

	cObjFileMesh*	m_pWeapon;
	
	cStateBar*		m_pStateBar;

	bool			m_KeyDown[KEYMAX];

	bool			m_check;
	// 
	// 	bool bLive; // ���� �÷���
public:
	cResourceMgr(void);
	~cResourceMgr(void);

	void SetUp(cCharacter* character,cCharacter* Enemy,cInventory* pInven); // �ʱ�ȭ
	void Insert(IN char* FileName, IN int nWidth, IN int nHeight, IN D3DCOLOR td3dColor); // ��������Ʈ ����
	void Render();  // ����
	void Delete(); // ����
	void ImageLoad(); // �̹��� �ε� (�Ѳ����� ��ηε�)
	void KeyInput(); //Ű
	bool ToggleKey(int vKey);

	std::vector<CSprite*>	GetRes(){return m_vResourceMgr;}

	void SetPhsh(bool t_push){push = t_push;}


};
