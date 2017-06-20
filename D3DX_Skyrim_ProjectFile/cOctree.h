#pragma once

//< 정적인 오브젝트 컬링을 위한 옥트리를 정의합니다. (형석)

class cTerrainObject;
class cPlayerCharacter;
class cOctreeNode;
class cOctree
{
	int						m_nDepth;	//< 옥트리 깊이
	cOctreeNode*		m_pRoot;	//< 옥트리 루트노드

public:
	cOctree(void);
	~cOctree(void);

	void Setup( IN int nDepth ); //< 깊이만큼 옥트리 생성
	void PushObject( cOctreeNode* pTree, cTerrainObject* pObject ); //< 옥트리 노드리스트에 정적 오브젝트 등록
	void Render( cOctreeNode* pTree ); //< 옥트리 출력( 디버깅용 )
	void Culling( cOctreeNode* pTree ); //< 프러스텀 컬링을 하여 랜더할 대상을 지정함
	void DestroyTree( cOctreeNode* pTree ); //< 옥트리 해제
	void Collision( cOctreeNode* pTree, cPlayerCharacter* pPlayer ); //< 랜더 대상 지형과 플레이어 충돌처리

	//<properties
	inline cOctreeNode* GetRootPtr( void ) { return m_pRoot; }

private:
	void Partition( INOUT cOctreeNode* pTree, IN int nDepth, IN D3DXVECTOR3 vMax, IN D3DXVECTOR3 vMin ); //< 뎁스만큼 공간분할
	bool CheckIntersect( cOctreeNode* pTree, cTerrainObject* pObject ); //< 오브젝트가 옥트리 노드에 걸리는지 체크
	bool CheckIntersect( cTerrainObject* pObject, cPlayerCharacter* pPlayer ); //< 플레이어가 오브젝트 바운더리에 걸리는지 체크
};

