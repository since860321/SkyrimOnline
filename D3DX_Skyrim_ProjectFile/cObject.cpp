#include "StdAfx.h"
#include "cAutoReleasePool.h"
#include "cObject.h"

cObject::~cObject(void)
{
	m_TransformInfo.vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_TransformInfo.vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_TransformInfo.vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
}

void cObject::Retain( void )
{
	++ m_nRefCount;
}

void cObject::Release( void )
{
	if( m_nRefCount > 0 )
		-- m_nRefCount;

	if( m_nRefCount == 0 )
		delete this;
}

void cObject::AutoRelease( void )
{
	cAutoReleasePool ARP = cAutoReleasePool::GetInstance();

	ARP.AddObject( this ); //< 오토릴리즈 풀에 객체 추가
} 

void cObject::SetWorldTM( void )
{
	//<<형구 - 월드 티엠 구하기 
	D3DXMATRIXA16 matPos,matscale,matRotX ,matRotY , matRotZ ;

	D3DXMatrixScaling(&matscale,m_TransformInfo.vScale.x,
								m_TransformInfo.vScale.y,
								m_TransformInfo.vScale.z);

	D3DXMatrixTranslation(&matPos,m_TransformInfo.vPos.x, 
								  m_TransformInfo.vPos.y, 
								  m_TransformInfo.vPos.z);

	D3DXMatrixRotationX(&matRotX, m_TransformInfo.vRot.x );
	D3DXMatrixRotationY(&matRotY, m_TransformInfo.vRot.y );
	D3DXMatrixRotationZ(&matRotZ, m_TransformInfo.vRot.z );

	m_WolrdTM = matscale  * (matRotX * matRotY * matRotZ) * matPos;

}
