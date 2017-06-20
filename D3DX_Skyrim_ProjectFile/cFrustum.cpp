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
		//< ���� 8�� ����
		D3DXVECTOR3		v[8];

		v[0] = D3DXVECTOR3(	-1, 1, 0 );
		v[1] = D3DXVECTOR3(	1, 1, 0 );
		v[2] = D3DXVECTOR3( -1, -1, 0 );
		v[3] = D3DXVECTOR3( 1, -1, 0 );
		v[4] = D3DXVECTOR3( -1, 1, 1 );
		v[5] = D3DXVECTOR3( 1, 1, 1 );
		v[6] = D3DXVECTOR3( -1, -1, 1 );
		v[7] = D3DXVECTOR3( 1, -1, 1 );

		//< ����ȯ 2�� -> ���� ��ǥ ����
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

		//< �������� �����ؼ� ��� 6�� ����( �븻�� �ٱ� : �ð�������� ���� )
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
	//< ���� 8�� ����
	D3DXVECTOR3		v[8];

	v[0] = D3DXVECTOR3(	-1, 1, 0 );
	v[1] = D3DXVECTOR3(	1, 1, 0 );
	v[2] = D3DXVECTOR3( -1, -1, 0 );
	v[3] = D3DXVECTOR3( 1, -1, 0 );
	v[4] = D3DXVECTOR3( -1, 1, 1 );
	v[5] = D3DXVECTOR3( 1, 1, 1 );
	v[6] = D3DXVECTOR3( -1, -1, 1 );
	v[7] = D3DXVECTOR3( 1, -1, 1 );

	//< ����ȯ 2�� -> ���� ��ǥ ����
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

	//< �������� �����ؼ� ��� 6�� ����( �븻�� �ٱ� : �ð�������� ���� )
	D3DXPlaneFromPoints( &m_Plane[0],  &v[0], &v[1], &v[2] ); 
	D3DXPlaneFromPoints( &m_Plane[1],  &v[6], &v[5], &v[4] ); 
	D3DXPlaneFromPoints( &m_Plane[2],  &v[1], &v[5], &v[3] ); 
	D3DXPlaneFromPoints( &m_Plane[3],  &v[2], &v[6], &v[4] ); 
	D3DXPlaneFromPoints( &m_Plane[4],  &v[1], &v[4], &v[5] ); 
	D3DXPlaneFromPoints( &m_Plane[5],  &v[2], &v[3], &v[6] ); 
}

bool cFrustum::CheckInFrustum( IN const D3DXVECTOR3* vCenter, IN const float* fRadius )
{
	//6���� ����� �߽��� ���� �����.
	//� ������ 6���� ��鿡 ����־��µ� �ϳ��� <0 �̸� �����ȿ� ���Ե��� �ʴ°��̴�.
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