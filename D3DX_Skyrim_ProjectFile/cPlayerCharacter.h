#pragma once
#include "cCharacter.h"
class cObjFileMesh;
class cPlayerCharacter : public cCharacter
{
private:
	LPD3DXFRAME			m_pHand;
	D3DXMATRIXA16		m_matWeaponTM;
	
public:
	DWORD m_dwKey[KEY_END];
	
public:
	cPlayerCharacter(void);
	virtual ~cPlayerCharacter(void);


	//<< 업데이트 안에서 업데이트 정보 (컨트롤 외) 를 처리합니다.
	void Update();
	
	//<< 키 인풋을 관리할 프로시져
	bool WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

	//<< 키 인풋
	void KeyInput();
	   //<< 비무장 상태의 무빙 
	   void NoBattle_Move();
	   //<< 무장 상태의 무빙 
	   void Battle_Move();

	// 공격 판정 체크
	bool Atk_CollisionCheck();

	//<< 카메라 좌표 셋팅
	void Update_CameraVector();
	void GetMouseMoveValue();

	void Render( void );

};

