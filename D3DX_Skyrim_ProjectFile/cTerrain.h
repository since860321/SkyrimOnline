#pragma once
#include "cstaticobject.h"

//< Height Map을 이용한 지형( 형석 )

class cTerrain : public cStaticObject
{
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;		//< 인덱스 버퍼
	LPDIRECT3DTEXTURE9				m_pTexture;
	D3DXVECTOR3						m_vPos[MAPSIZE*MAPSIZE];

public:
	cTerrain(void);
	virtual ~cTerrain(void);

	void Setup( void );
	void Render( void );
	void SetHeight( IN D3DXVECTOR3* pPos );
	bool IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
		D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
		FLOAT* t, FLOAT* u, FLOAT* v );
};

