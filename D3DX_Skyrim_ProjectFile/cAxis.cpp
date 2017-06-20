#include "StdAfx.h"
#include "cAxis.h"


cAxis::cAxis(void)
{
}


cAxis::~cAxis(void)
{
}

void cAxis::Setup( HWND hWnd, D3DCOLOR color, AXIS axis )
{
	//< 버텍스 버퍼 할당
	if( FAILED ( sgDevice->CreateVertexBuffer( 2*sizeof(stGridVertex), 0, GRID_D3DFVF, D3DPOOL_DEFAULT, &m_pVB, 0 ) ))
	{
		MessageBox( hWnd, "버텍스버퍼 할당 실패", "실패", MB_OK );
		return ;
	}

	//< 로컬좌표랑 컬러값 세팅
	if( axis == AXIS_X )
	{
		m_Vertices[0].vPos = D3DXVECTOR3( AXISLENGTH, 0, 0 );
		m_Vertices[1].vPos = D3DXVECTOR3( -AXISLENGTH, 0, 0 );
	}
	else if( axis == AXIS_Y )
	{
		m_Vertices[0].vPos = D3DXVECTOR3( 0, AXISLENGTH, 0 );
		m_Vertices[1].vPos = D3DXVECTOR3( 0, -AXISLENGTH, 0 );
	}
	else if( axis == AXIS_Z )
	{
		m_Vertices[0].vPos = D3DXVECTOR3( 0, 0, AXISLENGTH );
		m_Vertices[1].vPos = D3DXVECTOR3( 0, 0, -AXISLENGTH );
	}
	m_Vertices[0].color = m_Vertices[1].color = color;

	//< 버텍스 버퍼에 값 넣기
	void *pAddr;
	m_pVB->Lock( 0, 2*sizeof(stGridVertex), &pAddr, 0 );
	memcpy( pAddr, m_Vertices, 2*sizeof(stGridVertex) );
	m_pVB->Unlock();
}

void cAxis::Render( void )
{
	sgDevice->SetStreamSource( 0, m_pVB, 0, sizeof(stGridVertex) );
	sgDevice->SetFVF( GRID_D3DFVF );
	sgDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
}