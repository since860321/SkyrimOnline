#include "StdAfx.h"
#include "cObjFileMesh.h"


cObjFileMesh::cObjFileMesh(void)
{
	m_pTex = NULL;
	memset( &m_Mtrl, 0, sizeof(m_Mtrl) );
	memset( &m_vMax, 0, sizeof(D3DXVECTOR3) );
	memset( &m_vMin, 0, sizeof(D3DXVECTOR3) );
}


cObjFileMesh::~cObjFileMesh(void)
{
	SAFE_RELEASE( m_pTex );
}



