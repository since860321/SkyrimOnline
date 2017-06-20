#include "StdAfx.h"
#include "cFrustum.h"


cFrustum::cFrustum(void)
{
}


cFrustum::~cFrustum(void)
{
}

cFrustum& cFrustum::GetInstance( void )
{
	static cFrustum instance;
	return instance;
}

void cFrustum::Setup( void )
{
//	if( cDevice::GetInstance().m_bWireFrameMode == true )
//	{
		//< 정점 8개 구성
		D3DXVECTOR3		v[8];

		v[0] = D3DXVECTOR3(	-1, 1, 0 );
		v[1] = D3DXVECTOR3(	1, 1, 0 );
		v[2] = D3DXVECTOR3( -1, -1, 0 );
		v[3] = D3DXVECTOR3( 1, -1, 0 );
		v[4] = D3DXVECTOR3( -1, 1, 1 );
		v[5] = D3DXVECTOR3( 1, 1, 1 );
		v[6] = D3DXVECTOR3( -1, -1, 1 );
		v[7] = D3DXVECTOR3( 1, -1, 1 );

		//< 역변환 2번 -> 월드 좌표 생성
		D3DXMATRIXA16 matProj, matProjInv, matView, matViewInv;
		sgDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		D3DXMatrixInverse( &matProjInv, NULL, &matProj );
		sgDevice->GetTransform( D3DTS_VIEW, &matView ); 
		D3DXMatrixInverse( &matViewInv, NULL, &matView );
		m_matInv = matProjInv*matViewInv;

		for( int i = 0 ; i < 8 ; ++i )
		{
			D3DXVec3TransformCoord( &v[i], &v[i], &m_matInv );
		}

		//< 정점끼리 연결해서 평면 6개 정의( 노말은 바깥 : 시계방향으로 감음 )
		D3DXPlaneFromPoints( &m_Plane[0],  &v[0], &v[1], &v[2] ); 
		D3DXPlaneFromPoints( &m_Plane[1],  &v[6], &v[5], &v[4] ); 
		D3DXPlaneFromPoints( &m_Plane[2],  &v[1], &v[5], &v[3] ); 
		D3DXPlaneFromPoints( &m_Plane[3],  &v[2], &v[6], &v[4] ); 
		D3DXPlaneFromPoints( &m_Plane[4],  &v[1], &v[4], &v[5] ); 
		D3DXPlaneFromPoints( &m_Plane[5],  &v[2], &v[3], &v[6] ); 
//	}
}

void cFrustum::Move( void )
{
	//< 정점 8개 구성
	D3DXVECTOR3		v[8];

	v[0] = D3DXVECTOR3(	-1, 1, 0 );
	v[1] = D3DXVECTOR3(	1, 1, 0 );
	v[2] = D3DXVECTOR3( -1, -1, 0 );
	v[3] = D3DXVECTOR3( 1, -1, 0 );
	v[4] = D3DXVECTOR3( -1, 1, 1 );
	v[5] = D3DXVECTOR3( 1, 1, 1 );
	v[6] = D3DXVECTOR3( -1, -1, 1 );
	v[7] = D3DXVECTOR3( 1, -1, 1 );

	//< 역변환 2번 -> 월드 좌표 생성
	D3DXMATRIXA16 matInv, matProj, matProjInv, matView, matViewInv;
	sgDevice->GetTransform( D3DTS_PROJECTION, &matProj );
	D3DXMatrixInverse( &matProjInv, NULL, &matProj );
	sgDevice->GetTransform( D3DTS_VIEW, &matView ); 
	D3DXMatrixInverse( &matViewInv, NULL, &matView );
	matInv = matProjInv*matViewInv;

	for( int i = 0 ; i < 8 ; ++i )
	{
		D3DXVec3TransformCoord( &v[i], &v[i], &matInv );
	}

	//< 정점끼리 연결해서 평면 6개 정의( 노말은 바깥 : 시계방향으로 감음 )
	D3DXPlaneFromPoints( &m_Plane[0],  &v[0], &v[1], &v[2] ); 
	D3DXPlaneFromPoints( &m_Plane[1],  &v[6], &v[5], &v[4] ); 
	D3DXPlaneFromPoints( &m_Plane[2],  &v[1], &v[5], &v[3] ); 
	D3DXPlaneFromPoints( &m_Plane[3],  &v[2], &v[6], &v[4] ); 
	D3DXPlaneFromPoints( &m_Plane[4],  &v[1], &v[4], &v[5] ); 
	D3DXPlaneFromPoints( &m_Plane[5],  &v[2], &v[3], &v[6] ); 
}

bool cFrustum::CheckInFrustum( IN const D3DXVECTOR3* vCenter, IN const float* fRadius )
{
	//6개의 평면이 중심을 보게 만든다.
	//어떤 한점이 6개의 평면에 집어넣었는데 하나라도 <0 이면 공간안에 포함되지 않는것이다.
	for( int i = 0 ; i < 6 ; i ++ )
	{
		if( D3DXPlaneDotCoord( &m_Plane[i], vCenter ) > *fRadius )
		{
			return false;
		}
	}
	return true;
}

void cFrustum::DebugRender( void )
{
	if( cDevice::GetInstance().m_bWireFrameMode == true )
	{
		cDevice::GetInstance().SetWireFrameTexture( WF_RED );
		sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		D3DXCreateBox( sgDevice, 2, 2, 1, &m_pMesh, NULL );

		sgDevice->SetTransform( D3DTS_WORLD, &m_matInv );

		m_pMesh->DrawSubset( 0 );
		sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
}