#include "StdAfx.h"
#include "cObjFileMesh.h"
#include "cTerrainObject.h"


cTerrainObject::cTerrainObject( void )
{
}


cTerrainObject::~cTerrainObject(void)
{
}

void cTerrainObject::Setup( cObjFileMesh* pMesh )
{
	m_pMesh = pMesh;

	D3DXCreateBox( sgDevice, m_pMesh->GetMaxPtr()->x - m_pMesh->GetMinPtr()->x, m_pMesh->GetMaxPtr()->y - m_pMesh->GetMinPtr()->y, m_pMesh->GetMaxPtr()->z - m_pMesh->GetMinPtr()->z, &m_pMesh->m_pMesh, NULL ); 
}

void cTerrainObject::Render( void )
{
	if( this->m_bRender == false ) return;
	m_bRender = false; //< 옥트리에 중복되게 들어가 있을 수 있으므로 2번 출력을 방지

	sgDevice->SetTexture( 0, m_pMesh->m_pTex );
	cDevice::GetInstance().SetWireFrameTexture( WF_YELLOW ); //< 와이어프레임
	sgDevice->SetMaterial( &m_pMesh->m_Mtrl );

	D3DXMATRIXA16 matWorld, matScale, matRotX, matRotY, matRotZ, matPos;
	D3DXMatrixScaling( &matScale, m_TransformInfo.vScale.x, m_TransformInfo.vScale.y, m_TransformInfo.vScale.z );
	D3DXMatrixRotationX( &matRotX, m_TransformInfo.vRot.x );
	D3DXMatrixRotationY( &matRotY, m_TransformInfo.vRot.y );
	D3DXMatrixRotationZ( &matRotZ, m_TransformInfo.vRot.z );
	D3DXMatrixTranslation( &matPos, m_TransformInfo.vPos.x, m_TransformInfo.vPos.y, m_TransformInfo.vPos.z );
	matWorld = matScale * matRotX * matRotY * matRotZ * matPos;

	sgDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//알파 테스팅 ( 풀숲을 위한... )
	sgDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	sgDevice->SetRenderState(D3DRS_ALPHAREF, 0x09);
	sgDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	sgDevice->SetStreamSource( 0, m_pMesh->m_pVB, 0, sizeof(stObjVertex) );
	sgDevice->SetFVF( OBJ_D3DFVF );
	sgDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_pMesh->m_Vertices.size()/3 );

	//< 바운딩박스 출력( 디버깅용 )	////////////////////////////////////////////////
	//sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//D3DXMatrixTranslation( &matPos, m_TransformInfo.vPos.x, m_TransformInfo.vPos.y, m_TransformInfo.vPos.z );

	////< 박스의 월드 위치y = 바위의 월드 위치y + ( 바위의 min - 박스의 min )
	//float BoxY = m_TransformInfo.vPos.y + m_pMesh->GetMinPtr()->y + (m_pMesh->GetMaxPtr()->y - m_pMesh->GetMinPtr()->y)/2.0f;
	//D3DXMatrixTranslation( &matPos, m_TransformInfo.vPos.x, BoxY, m_TransformInfo.vPos.z );
	//sgDevice->SetTransform( D3DTS_WORLD, &matPos );

	//m_pMesh->m_pMesh->DrawSubset( 0 );
	//sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	/////////////////////////////////////////////////////////////////////////////////////
}
