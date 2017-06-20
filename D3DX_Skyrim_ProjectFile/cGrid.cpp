#include "StdAfx.h"
#include "cAxis.h"
#include "cGrid.h"

cGrid::cGrid(void)
{
	memset( m_Vertices, 0, sizeof(stGridVertex)*GRIDSIZE*GRIDSIZE );
	m_pVB = NULL;
	m_pAxis = NULL;
}


cGrid::~cGrid(void)
{
	SAFE_RELEASE( m_pVB );
	SAFE_DELETE_ARRAY( m_pAxis );
}

void cGrid::Setup( HWND hWnd )
{
	//< 로컬좌표랑 컬러값 세팅
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < GRIDSIZE ; j ++ )
		{
			m_Vertices[i*GRIDSIZE + j].vPos.x = (j - GRIDSIZE/2)*GRIDWIDTH;
			m_Vertices[i*GRIDSIZE + j].vPos.y = 0.0f;
			m_Vertices[i*GRIDSIZE + j].vPos.z = (GRIDSIZE - 1 - i - GRIDSIZE/2)*GRIDWIDTH;
			m_Vertices[i*GRIDSIZE + j].color = 0xffaaaaaa;
		}
	}

	//< 버텍스 버퍼 할당
	if( FAILED ( sgDevice->CreateVertexBuffer( (GRIDSIZE)*(GRIDSIZE-1)*4*sizeof(stGridVertex), 0, GRID_D3DFVF, D3DPOOL_DEFAULT, &m_pVB, 0 ) ))
	{
		MessageBox( hWnd, "버텍스버퍼 할당 실패", "실패", MB_OK );
		return ;
	}

	stGridVertex Vertex[(GRIDSIZE)*(GRIDSIZE-1)*4]; 
	int nCnt = 0;
	//< 가로처리
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < (GRIDSIZE-1)*2 ; j ++ )
		{
			Vertex[nCnt] = m_Vertices[int((j+1)/2)+GRIDSIZE*i];
			nCnt ++;
		}
	}
	//< 세로 처리
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < (GRIDSIZE-1)*2 ; j ++ )
		{
			Vertex[nCnt] = m_Vertices[int((j+1)/2)*GRIDSIZE+i];
			nCnt ++;
		}
	}

	//< 버텍스 버퍼에 값 넣기
	void *pAddr;
	m_pVB->Lock( 0, (GRIDSIZE)*(GRIDSIZE-1)*4*sizeof(stGridVertex), &pAddr, 0 );
	memcpy( pAddr, Vertex, (GRIDSIZE)*(GRIDSIZE-1)*4*sizeof(stGridVertex) );
	m_pVB->Unlock();

	//< 축세팅
	m_pAxis = new cAxis[AXIS_END];
	m_pAxis[AXIS_X].Setup( hWnd, 0x00ff0000, AXIS_X );
	m_pAxis[AXIS_Y].Setup( hWnd, 0x0000ff00, AXIS_Y );
	m_pAxis[AXIS_Z].Setup( hWnd, 0x000000ff, AXIS_Z );
}

void cGrid::SetGridWidth( float fGridWidth )
{
	//< 로컬좌표랑 컬러값 세팅
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < GRIDSIZE ; j ++ )
		{
			m_Vertices[i*GRIDSIZE + j].vPos.x = (j - GRIDSIZE/2)*fGridWidth;
			m_Vertices[i*GRIDSIZE + j].vPos.y = 0.0f;
			m_Vertices[i*GRIDSIZE + j].vPos.z = (GRIDSIZE - 1 - i - GRIDSIZE/2)*fGridWidth;
			m_Vertices[i*GRIDSIZE + j].color = 0xffaaaaaa;
		}
	}

	stGridVertex Vertex[(GRIDSIZE)*(GRIDSIZE-1)*4]; 
	int nCnt = 0;
	//< 가로처리
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < (GRIDSIZE-1)*2 ; j ++ )
		{
			Vertex[nCnt] = m_Vertices[int((j+1)/2)+GRIDSIZE*i];
			nCnt ++;
		}
	}
	//< 세로 처리
	for( int i = 0 ; i < GRIDSIZE ; i ++ )
	{
		for( int j = 0 ; j < (GRIDSIZE-1)*2 ; j ++ )
		{
			Vertex[nCnt] = m_Vertices[int((j+1)/2)*GRIDSIZE+i];
			nCnt ++;
		}
	}

	//< 버텍스 버퍼에 값 넣기
	void *pAddr;
	m_pVB->Lock( 0, (GRIDSIZE)*(GRIDSIZE-1)*4*sizeof(stGridVertex), &pAddr, 0 );
	memcpy( pAddr, Vertex, (GRIDSIZE)*(GRIDSIZE-1)*4*sizeof(stGridVertex) );
	m_pVB->Unlock();
}

void cGrid::Render( void )
{
	//< 월드 스페이스
	sgDevice->SetRenderState( D3DRS_LIGHTING, false );

	D3DXMATRIXA16 matPos;
	D3DXMatrixTranslation( &matPos, 0.0f, 0.0f, 0.0f );
	sgDevice->SetTransform( D3DTS_WORLD, &matPos );

	sgDevice->SetStreamSource( 0, m_pVB, 0, sizeof(stGridVertex) );
	sgDevice->SetFVF( GRID_D3DFVF );
	sgDevice->DrawPrimitive( D3DPT_LINELIST, 0, GRIDSIZE*(GRIDSIZE+1)*2 );

	//< 축 그리기
	for( int i = 0 ; i < AXIS_END ; i ++ )
	{
		m_pAxis[i].Render();
	}

	sgDevice->SetRenderState( D3DRS_LIGHTING, true );

}