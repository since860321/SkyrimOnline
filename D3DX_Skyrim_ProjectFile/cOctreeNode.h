#pragma once

class cTerrainObject;
class cOctreeNode
{
public:
	int										m_nDepth;				//< �ش� ��尡 ��ġ�� ����
	bool									m_bRender;				//< �ø��� ���� ��������

	cOctreeNode*						m_pChild[PT_END];	//< 8���� �ڽ��� ����Ű�� ������
	std::vector<cTerrainObject*>	m_ObjectList;			//< ��忡 ���� ������Ʈ ����Ʈ
	D3DXVECTOR3					m_vMax;					//< ��Ʈ�� �ڽ�
	D3DXVECTOR3					m_vMin;

	LPD3DXMESH						m_pMesh;				//< ��Ʈ�� �ڽ�( ������ )
public:
	cOctreeNode(void);
	~cOctreeNode(void);

	void Render( void );				//< ��Ʈ���ٿ���ڽ� ����(������)

	cOctreeNode** GetChildPtr( void ) { return m_pChild; }
	D3DXVECTOR3* GetMaxPtr( void ) { return &m_vMax; }
	D3DXVECTOR3* GetMinPtr( void ) { return &m_vMin; }
	void SetRender( bool bRender ) { m_bRender = bRender; }
};

