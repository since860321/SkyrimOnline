#pragma once

//< ������ ������Ʈ �ø��� ���� ��Ʈ���� �����մϴ�. (����)

class cTerrainObject;
class cPlayerCharacter;
class cOctreeNode;
class cOctree
{
	int						m_nDepth;	//< ��Ʈ�� ����
	cOctreeNode*		m_pRoot;	//< ��Ʈ�� ��Ʈ���

public:
	cOctree(void);
	~cOctree(void);

	void Setup( IN int nDepth ); //< ���̸�ŭ ��Ʈ�� ����
	void PushObject( cOctreeNode* pTree, cTerrainObject* pObject ); //< ��Ʈ�� ��帮��Ʈ�� ���� ������Ʈ ���
	void Render( cOctreeNode* pTree ); //< ��Ʈ�� ���( ������ )
	void Culling( cOctreeNode* pTree ); //< �������� �ø��� �Ͽ� ������ ����� ������
	void DestroyTree( cOctreeNode* pTree ); //< ��Ʈ�� ����
	void Collision( cOctreeNode* pTree, cPlayerCharacter* pPlayer ); //< ���� ��� ������ �÷��̾� �浹ó��

	//<properties
	inline cOctreeNode* GetRootPtr( void ) { return m_pRoot; }

private:
	void Partition( INOUT cOctreeNode* pTree, IN int nDepth, IN D3DXVECTOR3 vMax, IN D3DXVECTOR3 vMin ); //< ������ŭ ��������
	bool CheckIntersect( cOctreeNode* pTree, cTerrainObject* pObject ); //< ������Ʈ�� ��Ʈ�� ��忡 �ɸ����� üũ
	bool CheckIntersect( cTerrainObject* pObject, cPlayerCharacter* pPlayer ); //< �÷��̾ ������Ʈ �ٿ������ �ɸ����� üũ
};

