#pragma once
#include "cUnvisibleObject.h"
#include "cCharacter.h"
class cCharacter;

class cCamera : public cUnvisibleObject
{
	D3DXMATRIXA16	m_matView;
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLook;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3		m_vView;


	POINT			m_Pt;
	cCharacter*		m_pPlayer;

public:
	cCamera(void);
	~cCamera(void);

	void Setup( HWND hWnd , IN cCharacter* Player);

	//<< 형구 개발자용 카메라 모드
	void Move( WORD x, WORD y );
	void ZoomIn( void );
	void ZoomOut( void );
	//< 회전(마우스)
	void Rotation( WORD x, WORD y );
	void SetCursor( WORD x, WORD y );

	//<< 형구 FPS 카메라 모드
	void Setup_FPS();
	
	//<< 형구 Get
	inline D3DXVECTOR3* GetLook(){ return &m_vLook; }
	inline D3DXVECTOR3* GetEye (){ return &m_vEye;  }
	inline D3DXVECTOR3* GetView (){ return &m_vView;  }


};

