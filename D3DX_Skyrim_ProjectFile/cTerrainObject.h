#pragma once
#include "cstaticobject.h"

//< cObjParser 에서 파싱한 단일 매쉬 오브젝트 클래스입니다. ( 형석 )

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

