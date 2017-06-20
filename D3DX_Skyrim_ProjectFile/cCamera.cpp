#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
{
	m_pPlayer = NULL;
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup( HWND hWnd , IN cCharacter* Player)
{
	//< 뷰스페이스
	m_vEye		= D3DXVECTOR3( 100.0f, 200.0f, 500.0f );
	m_vLook		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	m_vUp		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &m_vUp );
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

	//< 투영
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, WINSIZEX/(float)WINSIZEY, 1.0f, 100000.0f );
	sgDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	//< 화면 중심으로 세팅
	m_Pt.x = WINSIZEX/2;
	m_Pt.y = WINSIZEY/2;
	ClientToScreen( hWnd, &m_Pt );
	SetCursorPos(m_Pt.x, m_Pt.y);

	m_pPlayer = Player;

	D3DXVec3Normalize(&m_vView , &(m_vLook - m_vEye) );

}

void cCamera::Move( WORD x, WORD y )
{
	float fSpeed  = 5 *  CAMERA_SPEED_MOVE;
	D3DXVECTOR3 vView = m_vLook - m_vEye;
	D3DXVec3Normalize( &vView, &vView ); 
	D3DXVECTOR3 vCross;
	D3DXVec3Cross( &vCross, &vView, &m_vUp );
	m_vEye += -vCross*((m_Pt.x-x)*fSpeed);
	m_vLook += -vCross*((m_Pt.x-x)*fSpeed);
	m_vEye += -m_vUp*((m_Pt.y-y)*fSpeed);
	m_vLook += -m_vUp*((m_Pt.y-y)*fSpeed);

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &m_vUp );
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

	m_Pt.x = x;
	m_Pt.y = y;
}

void cCamera::Rotation( WORD x, WORD y )
{
	float fSpeed  = 5 *  CAMERA_SPEED_ROTATION;

	D3DXVECTOR3 vView = m_vLook - m_vEye;
	D3DXVec3Normalize( &vView, &vView ); 
	D3DXVECTOR3 vCross;
	D3DXVec3Cross( &vCross, &vView, &m_vUp );
	m_vEye += -vCross*((m_Pt.x-x)*fSpeed);
	m_vEye += -m_vUp*((m_Pt.y-y)*fSpeed);

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &m_vUp );
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

	m_Pt.x = x;
	m_Pt.y = y;
}

void cCamera::ZoomIn( void )
{
	float fSpeed  = 30 *  CAMERA_SPEED_ZOOM;

	D3DXVECTOR3 vView = m_vLook - m_vEye;
	D3DXVec3Normalize( &vView, &vView ); 
	m_vEye += vView*fSpeed;
	m_vLook += vView*fSpeed;

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &m_vUp );
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

}

void cCamera::ZoomOut( void )
{
	float fSpeed  = 30 *  CAMERA_SPEED_ZOOM;

	D3DXVECTOR3 vView = m_vLook - m_vEye;
	D3DXVec3Normalize( &vView, &vView ); 
	m_vEye -= vView*fSpeed;
	m_vLook -= vView*fSpeed;

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLook, &m_vUp );
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

}

void cCamera::SetCursor( WORD x, WORD y )
{
	m_Pt.x = x;
	m_Pt.y = y;
}

//<< 형구 카메라 설정

void cCamera::Setup_FPS()
{
	m_vView	  = m_pPlayer->GetCharacterInfo()->m_vView;

	m_vEye	  = m_pPlayer->GetCharacterInfo()->m_vEye;
	m_vEye   += -(m_vView *90);
	m_vEye.y += 475;

	m_vLook    = m_pPlayer->GetCharacterInfo()->m_vLook;
	m_vLook   += (m_vView *5);
	m_vLook.y += 30;
	
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLook, &m_vUp);
	sgDevice->SetTransform( D3DTS_VIEW, &m_matView );

}

