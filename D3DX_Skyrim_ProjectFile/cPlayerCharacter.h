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


	//<< ������Ʈ �ȿ��� ������Ʈ ���� (��Ʈ�� ��) �� ó���մϴ�.
	void Update();
	
	//<< Ű ��ǲ�� ������ ���ν���
	bool WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

	//<< Ű ��ǲ
	void KeyInput();
	   //<< ���� ������ ���� 
	   void NoBattle_Move();
	   //<< ���� ������ ���� 
	   void Battle_Move();

	// ���� ���� üũ
	bool Atk_CollisionCheck();

	//<< ī�޶� ��ǥ ����
	void Update_CameraVector();
	void GetMouseMoveValue();

	void Render( void );

};

