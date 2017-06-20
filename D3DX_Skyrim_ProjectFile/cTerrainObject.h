#pragma once
#include "cstaticobject.h"

//< cObjParser ���� �Ľ��� ���� �Ž� ������Ʈ Ŭ�����Դϴ�. ( ���� )

class cObjFileMesh;
class cTerrainObject : public cStaticObject
{
	cObjFileMesh*		m_pMesh;

public:
	bool					m_bRender;

public:
	cTerrainObject( void );
	~cTerrainObject(void);

	void Setup( cObjFileMesh* pMesh ); 
	virtual void Render( void ) override;
	void SetRender( bool bRender ) { m_bRender = bRender; }
	cObjFileMesh* GetMeshPtr( void ) { return m_pMesh; }
};

