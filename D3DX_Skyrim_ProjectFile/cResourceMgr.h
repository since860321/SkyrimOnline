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


	cCharacter*		m_pPlayer;//< 캐릭터 

	int nWidth;

	cObjFileMesh*	m_pWeapon;
	
	cStateBar*		m_pStateBar;

	bool			m_KeyDown[KEYMAX];

	bool			m_check;
	// 
	// 	bool bLive; // 상태 플래그
public:
	cResourceMgr(void);
	~cResourceMgr(void);

	void SetUp(cCharacter* character,cCharacter* Enemy,cInventory* pInven); // 초기화
	void Insert(IN char* FileName, IN int nWidth, IN int nHeight, IN D3DCOLOR td3dColor); // 스프라이트 생성
	void Render();  // 랜더
	void Delete(); // 삭제
	void ImageLoad(); // 이미지 로드 (한꺼번에 모두로드)
	void KeyInput(); //키
	bool ToggleKey(int vKey);

	std::vector<CSprite*>	GetRes(){return m_vResourceMgr;}

	void SetPhsh(bool t_push){push = t_push;}


};
