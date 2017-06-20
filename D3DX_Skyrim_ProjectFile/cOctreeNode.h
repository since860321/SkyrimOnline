#pragma once

class cTerrainObject;
class cOctreeNode
{
public:
	int										m_nDepth;				//< 해당 노드가 위치한 깊이
	bool									m_bRender;				//< 컬링에 따른 랜더여부

	cOctreeNode*						m_pChild[PT_END];	//< 8개의 자식을 가리키는 포인터
	std::vector<cTerrainObject*>	m_ObjectList;			//< 노드에 속한 오브젝트 리스트
	D3DXVECTOR3					m_vMax;					//< 옥트리 박스
	D3DXVECTOR3					m_vMin;

	LPD3DXMESH						m_pMesh;				//< 옥트리 박스( 디버깅용 )
public:
	cOctreeNode(void);
	~cOctreeNode(void);

	void Render( void );				//< 옥트리바운딩박스 랜더(디버깅용)

	cOctreeNode** GetChildPtr( void ) { return m_pChild; }
	D3DXVECTOR3* GetMaxPtr( void ) { return &m_vMax; }
	D3DXVECTOR3* GetMinPtr( void ) { return &m_vMin; }
	void SetRender( bool bRender ) { m_bRender = bRender; }
};

