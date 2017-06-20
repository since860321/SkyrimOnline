#include "StdAfx.h"
#include "cGameTime.h"
#include "cVisibleObject.h"
#include "BaseSkinnedMesh.h"

cVisibleObject::cVisibleObject(void)
{
	m_pSkinnedMesh = NULL;
	D3DXMatrixIdentity(&m_WolrdTM);
}

cVisibleObject::~cVisibleObject(void)
{

}

void cVisibleObject::SetModel( IN BaseD3D::BaseSkinnedMesh* SkinnedMesh)
{
	m_pSkinnedMesh = SkinnedMesh;
}

void cVisibleObject::Render( void )
{
	this->SetWorldTM();
	assert( m_pSkinnedMesh != NULL && "Message : Not Model"  );
	sgDevice->SetTransform(D3DTS_WORLD , &m_WolrdTM);
	m_pSkinnedMesh->Render( m_WolrdTM, cGameTime::Instance().Get_ElapsedTime() / 800.0 );
}


