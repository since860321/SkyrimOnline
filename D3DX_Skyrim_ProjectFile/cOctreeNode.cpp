#include "StdAfx.h"
#include "cTerrainObject.h"
#include "cFrustum.h"
#include "cOctreeNode.h"


cOctreeNode::cOctreeNode(void)
{
	for( size_t i = 0 ; i < PT_END ; ++ i )
	{
		m_pChild[i] = NULL;
	}
	memset( &m_vMax, 0, sizeof(m_vMax) );
	memset( &m_vMin, 0, sizeof(m_vMin) );

	m_pMesh = NULL;
}


cOctreeNode::~cOctreeNode(void)
{
	SAFE_RELEASE( m_pMesh );
}

void cOctreeNode::Render( void )
{
	//< ����� �θƽ��� �ٿ�� ���Ǿ �����ϰ� �ٿ�� ���Ǿ �������� �ȿ� ������ üũ�ؼ� ������ ���� ������
	D3DXVECTOR3 vCenter;
	vCenter.x = ( m_vMax.x + m_vMin.x )/2.0f;
	vCenter.y = ( m_vMax.y + m_vMin.y )/2.0f;
	vCenter.z = ( m_vMax.z + m_vMin.z )/2.0f;
	float radius = sqrtf( (m_vMax.x - vCenter.x)*(m_vMax.x - vCenter.x) + (m_vMax.y - vCenter.y)*(m_vMax.y - vCenter.y) + (m_vMax.z - vCenter.z)*(m_vMax.z - vCenter.z));
	if( sgFrustum.CheckInFrustum( &vCenter, &radius ) == false ) { return; }

	for( size_t i = 0 ; i < m_ObjectList.size(); i ++ )
	{
		m_ObjectList[i]->Render();
	}

	//< �ٿ���ڽ� ���( ������ )	////////////////////////////////////////////////
	cDevice::GetInstance().SetWireFrameTexture( WF_BLUE ); //< ��� ���̾�������

	if( cDevice::GetInstance().m_bWireFrameMode == true )
	{
		D3DXMATRIXA16 matPos;
		sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		D3DXMatrixTranslation( &matPos, vCenter.x, vCenter.y, vCenter.z );
		sgDevice->SetTransform( D3DTS_WORLD, &matPos );

		D3DXCreateBox( sgDevice, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z, &m_pMesh, NULL ); 
		m_pMesh->DrawSubset( 0 );
	//	sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
	/////////////////////////////////////////////////////////////////////////////////////
}
