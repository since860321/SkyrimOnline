#pragma once
class cObjFileMesh
{
public:
	D3DMATERIAL9						m_Mtrl;
	LPDIRECT3DTEXTURE9				m_pTex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	std::vector<stObjVertex>			m_Vertices;

	LPD3DXMESH							m_pMesh;

private:
	D3DXVECTOR3						m_vMax;
	D3DXVECTOR3						m_vMin;

public:
	cObjFileMesh(void);
	~cObjFileMesh(void);

	D3DXVECTOR3* GetMaxPtr( void ) { return &m_vMax; }
	D3DXVECTOR3* GetMinPtr( void ) { return &m_vMin; }
};

