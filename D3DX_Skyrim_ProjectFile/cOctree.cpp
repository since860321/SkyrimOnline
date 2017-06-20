#include "StdAfx.h"
#include "cFrustum.h"
#include "cTerrainObject.h"
#include "cPlayerCharacter.h"
#include "cObjFileMesh.h"
#include "cOctreeNode.h"
#include "cOctree.h"


cOctree::cOctree(void)
{
	m_nDepth = 0;
	m_pRoot = NULL;
}


cOctree::~cOctree(void)
{
	DestroyTree( m_pRoot );
}

void cOctree::DestroyTree( cOctreeNode* pTree )
{
	if( pTree == NULL ) return;
	
	for( int i = 0 ; i < PT_END ; i ++ )
	{
		DestroyTree( pTree->GetChildPtr()[i] );
	}

	SAFE_DELETE( pTree );
}

void cOctree::Setup( IN int nDepth )
{
	m_nDepth = nDepth;

	m_pRoot = new cOctreeNode();
	*m_pRoot->GetMaxPtr() = D3DXVECTOR3( WORLDSIZE/2.0f, WORLDSIZE/2.0f, WORLDSIZE/2.0f );
	*m_pRoot->GetMinPtr() = D3DXVECTOR3( -WORLDSIZE/2.0f, -WORLDSIZE/2.0f, -WORLDSIZE/2.0f );

	Partition( m_pRoot, 1, *m_pRoot->GetMaxPtr(), *m_pRoot->GetMinPtr() );
}

void cOctree::Partition( INOUT cOctreeNode* pTree, IN int nDepth, IN D3DXVECTOR3 vMax, IN D3DXVECTOR3 vMin )
{
	pTree->m_nDepth = nDepth; //<노드가 트리상에서 위치한 깊이

	if( m_nDepth == nDepth ) return; 

	D3DXVECTOR3 vCenter;
	vCenter.x = (vMax.x + vMin.x)/2.0f;
	vCenter.y = (vMax.y + vMin.y)/2.0f;
	vCenter.z = (vMax.z + vMin.z)/2.0f;

	//< Left Top Far
	pTree->GetChildPtr()[PT_LTF] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_LTF]->GetMaxPtr()) = D3DXVECTOR3( vCenter.x, vMax.y, vMax.z );
	*(pTree->GetChildPtr()[PT_LTF]->GetMinPtr()) = D3DXVECTOR3( vMin.x, vCenter.y, vCenter.z );
	Partition( pTree->GetChildPtr()[PT_LTF], nDepth + 1, *(pTree->GetChildPtr()[PT_LTF]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_LTF]->GetMinPtr()) );

	//< Right Top Far
	pTree->GetChildPtr()[PT_RTF] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_RTF]->GetMaxPtr()) = D3DXVECTOR3( vMax.x, vMax.y, vMax.z );
	*(pTree->GetChildPtr()[PT_RTF]->GetMinPtr()) = D3DXVECTOR3( vCenter.x, vCenter.y, vCenter.z );
	Partition( pTree->GetChildPtr()[PT_RTF], nDepth + 1, *(pTree->GetChildPtr()[PT_RTF]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_RTF]->GetMinPtr()) );

	//< Left Top Near
	pTree->GetChildPtr()[PT_LTN] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_LTN]->GetMaxPtr()) = D3DXVECTOR3( vCenter.x, vMax.y, vCenter.z );
	*(pTree->GetChildPtr()[PT_LTN]->GetMinPtr()) = D3DXVECTOR3( vMin.x, vCenter.y, vMin.z );
	Partition( pTree->GetChildPtr()[PT_LTN], nDepth + 1, *(pTree->GetChildPtr()[PT_LTN]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_LTN]->GetMinPtr()) );

	////< Right Top Near
	pTree->GetChildPtr()[PT_RTN] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_RTN]->GetMaxPtr()) = D3DXVECTOR3( vMax.x, vMax.y, vCenter.z );
	*(pTree->GetChildPtr()[PT_RTN]->GetMinPtr()) = D3DXVECTOR3( vCenter.x, vCenter.y, vMin.z );
	Partition( pTree->GetChildPtr()[PT_RTN], nDepth + 1, *(pTree->GetChildPtr()[PT_RTN]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_RTN]->GetMinPtr()) );

	//< Left Bottom Far
	pTree->GetChildPtr()[PT_LBF] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_LBF]->GetMaxPtr()) = D3DXVECTOR3( vCenter.x, vCenter.y, vMax.z );
	*(pTree->GetChildPtr()[PT_LBF]->GetMinPtr()) = D3DXVECTOR3( vMin.x, vMin.y, vCenter.z );
	Partition( pTree->GetChildPtr()[PT_LBF], nDepth + 1, *(pTree->GetChildPtr()[PT_LBF]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_LBF]->GetMinPtr()) );

	//< Right Bottom Far
	pTree->GetChildPtr()[PT_RBF] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_RBF]->GetMaxPtr()) = D3DXVECTOR3( vMax.x, vCenter.y, vMax.z );
	*(pTree->GetChildPtr()[PT_RBF]->GetMinPtr()) = D3DXVECTOR3( vCenter.x, vMin.y, vCenter.z );
	Partition( pTree->GetChildPtr()[PT_RBF], nDepth + 1, *(pTree->GetChildPtr()[PT_RBF]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_RBF]->GetMinPtr()) );

	//< Left Bottom Near
	pTree->GetChildPtr()[PT_LBN] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_LBN]->GetMaxPtr()) = D3DXVECTOR3( vCenter.x, vCenter.y, vCenter.z );
	*(pTree->GetChildPtr()[PT_LBN]->GetMinPtr()) = D3DXVECTOR3( vMin.x, vMin.y, vMin.z );
	Partition( pTree->GetChildPtr()[PT_LBN], nDepth + 1, *(pTree->GetChildPtr()[PT_LBN]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_LBN]->GetMinPtr()) );

	//< Right Bottom Near
	pTree->GetChildPtr()[PT_RBN] = new cOctreeNode();
	(*pTree->GetChildPtr()[PT_RBN]->GetMaxPtr()) = D3DXVECTOR3( vMax.x, vCenter.y, vCenter.z );
	*(pTree->GetChildPtr()[PT_RBN]->GetMinPtr()) = D3DXVECTOR3( vCenter.x, vMin.y, vMin.z );
	Partition( pTree->GetChildPtr()[PT_RBN], nDepth + 1, *(pTree->GetChildPtr()[PT_RBN]->GetMaxPtr()), *(pTree->GetChildPtr()[PT_RBN]->GetMinPtr()) );
}

void cOctree::PushObject( cOctreeNode* pTree, cTerrainObject* pObject )
{
	if( pTree->m_nDepth == this->m_nDepth )
	{
		if( CheckIntersect( pTree, pObject ) )
		{
			//< 노드안에 오브젝트 등록
			pTree->m_ObjectList.push_back( pObject );
		}
		return ;
	}
	else
	{
		for( size_t i = 0 ; i < PT_END ; ++ i )
		{
			PushObject( pTree->GetChildPtr()[i], pObject );
		}
	}
}

bool cOctree::CheckIntersect( cOctreeNode* pTree, cTerrainObject* pObject )
{
	//< 나의 MinMax가 삽입되는 오브젝트 "월드"의 MinMax를 포용할 수 있는지 체크
	
	if( pTree == NULL ) return false;

	D3DXMATRIXA16 matPos;
	D3DXVECTOR3 vMaxPos, vMinPos;
	D3DXMatrixTranslation( &matPos, pObject->GetTransformInfo()->vPos.x, pObject->GetTransformInfo()->vPos.y, pObject->GetTransformInfo()->vPos.z );
	D3DXVec3TransformCoord( &vMaxPos, pObject->GetMeshPtr()->GetMaxPtr(), &matPos );
	D3DXVec3TransformCoord( &vMinPos, pObject->GetMeshPtr()->GetMinPtr(), &matPos );

	//< 위에서 봤을때 겹치고, 옆에서봤을때 겹치면 무조건 겹친다.
	RECT rcTemp;
	RECT rcNodeTop = { pTree->GetMinPtr()->x, pTree->GetMinPtr()->z, pTree->GetMaxPtr()->x, pTree->GetMaxPtr()->z };
	RECT rcObjTop = { vMinPos.x, vMinPos.z, vMaxPos.x, vMaxPos.z }; 
	RECT rcNodeSide = { pTree->GetMinPtr()->x, pTree->GetMinPtr()->y, pTree->GetMaxPtr()->x, pTree->GetMaxPtr()->y };
	RECT rcObjSide = { vMinPos.x, vMinPos.y, vMaxPos.x, vMaxPos.y }; 
	if( IntersectRect( &rcTemp, &rcNodeTop, &rcObjTop ) && IntersectRect( &rcTemp, &rcNodeSide, &rcObjSide ) )
	{
		return true;
	}

	return false;
}

void cOctree::Render( cOctreeNode* pTree )
{
	if( pTree == NULL ) return;

	pTree->Render(); 

	for( int i = 0 ; i < PT_END ; i ++ )
	{
		Render( pTree->GetChildPtr()[i] );
	}
}

void cOctree::Culling( cOctreeNode* pTree )
{
	if( pTree == NULL ) return ;

	//< 노드의 민맥스로 바운딩 스피어를 생성하고 바운딩 스피어가 프러스텀 안에 들어가는지 체크해서 랜더를 할지 결정함
	D3DXVECTOR3 vCenter;
	vCenter.x = ( pTree->GetMinPtr()->x + pTree->GetMaxPtr()->x )/2.0f;
	vCenter.y = ( pTree->GetMinPtr()->y + pTree->GetMaxPtr()->y )/2.0f;
	vCenter.z = ( pTree->GetMinPtr()->z + pTree->GetMaxPtr()->z )/2.0f;
	float radius = sqrtf( (pTree->GetMaxPtr()->x - vCenter.x)*(pTree->GetMaxPtr()->x - vCenter.x) 
		+ (pTree->GetMaxPtr()->y - vCenter.y)*(pTree->GetMaxPtr()->y - vCenter.y)
		+ (pTree->GetMaxPtr()->z - vCenter.z)*(pTree->GetMaxPtr()->z - vCenter.z));

	if( sgFrustum.CheckInFrustum( &vCenter, &radius ) == true ) 
	{
		pTree->SetRender( true );
		if( pTree->m_nDepth == this->m_nDepth )	//< 리프노드라면
		{
			//< 개별 컬링
			for( int i = 0 ; i < pTree->m_ObjectList.size(); i ++ )
			{
				vCenter.x = ( pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->x + pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x )/2.0f;
				vCenter.y = ( pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->y + pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y )/2.0f;
				vCenter.z = ( pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->z + pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z )/2.0f;
				radius = sqrtf( (pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x - vCenter.x)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x - vCenter.x) 
				+ (pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y - vCenter.y)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y - vCenter.y)
				+ (pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z - vCenter.z)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z - vCenter.z));
				
				//< 오브젝트 센터를 월드좌표 기준으로 변경
				D3DXMATRIXA16 matPos;
				D3DXMatrixTranslation( &matPos, pTree->m_ObjectList[i]->GetTransformInfo()->vPos.x,
					pTree->m_ObjectList[i]->GetTransformInfo()->vPos.y,
					pTree->m_ObjectList[i]->GetTransformInfo()->vPos.z );
				D3DXVec3TransformCoord( &vCenter, &vCenter, &matPos );

				if( sgFrustum.CheckInFrustum( &vCenter, &radius ))
				{
					pTree->m_ObjectList[i]->SetRender( true );
				}
				else
				{
					pTree->m_ObjectList[i]->SetRender( false );
				}
			}
		}
		else
		{
			for( size_t i = 0 ; i < PT_END ; ++ i )
			{
				Culling( pTree->GetChildPtr()[i] );
			}
		}
	}
	else //< 프러스텀 안에 없으면 제거
	{
		pTree->SetRender( false );
	}
}

void cOctree::Collision( cOctreeNode* pTree, cPlayerCharacter* pPlayer )
{
	if( pTree->m_nDepth == this->m_nDepth )
	{
		for( size_t i = 0 ; i < pTree->m_ObjectList.size() ; ++ i )
		{
			//< 랜더 대상인 오브젝트와 충돌처리
			if( pTree->m_ObjectList[i]->m_bRender == true ) 
			{
				//< 플레이어와 오브젝트의 거리를 구하고, 
				D3DXVECTOR3 vObject, vPlayer;
				vObject = pTree->m_ObjectList[i]->GetTransformInfo()->vPos;
				vPlayer = pPlayer->GetTransformInfo()->vPos;
				float fDist = sqrtf(( vObject.x - vPlayer.x )*( vObject.x - vPlayer.x ) + ( vObject.y - vPlayer.y )*( vObject.y - vPlayer.y ) + ( vObject.z - vPlayer.z )*( vObject.z - vPlayer.z ));
				
				//< 플레이어의 반지름과 오브젝트의 반지름 합보다 작으면
				D3DXVECTOR3 vCenter;
				vCenter.x = pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x - pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->x;
				vCenter.y = pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y - pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->y;
				vCenter.z = pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z - pTree->m_ObjectList[i]->GetMeshPtr()->GetMinPtr()->z;
				float fRadius_Object = sqrtf(
					(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x-vCenter.x)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->x-vCenter.x) 
					+ (pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y-vCenter.y)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->y-vCenter.y)
					+ (pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z-vCenter.z)*(pTree->m_ObjectList[i]->GetMeshPtr()->GetMaxPtr()->z-vCenter.z));
				float fRadius_Player = pPlayer->GetCharacterInfo()->dwRadius;

//				if( CheckIntersect( pTree->m_ObjectList[i], pPlayer ))
				if( fDist < fRadius_Object + fRadius_Player )
				{
					//< 플레이어가 진행하는 역방향으로 밀어내기
					D3DXVECTOR3 vMoveDrt = pPlayer->GetCharacterInfo()->m_vView;
					vMoveDrt.y = 0;
					pPlayer->GetTransformInfo()->vPos -= vMoveDrt*1.0f;
				}
			}
		}
	}
	else
	{
		for( size_t i = 0 ; i < PT_END ; ++ i )
		{
			Collision( pTree->GetChildPtr()[i], pPlayer );
		}
	}
}

bool cOctree::CheckIntersect( cTerrainObject* pObject, cPlayerCharacter* pPlayer )
{
	if( pObject == NULL ) return false;
	if( pPlayer == NULL ) return false;

	D3DXMATRIXA16 matPos;
	D3DXVECTOR3 vMaxPos, vMinPos;
	D3DXMatrixTranslation( &matPos, pObject->GetTransformInfo()->vPos.x, pObject->GetTransformInfo()->vPos.y, pObject->GetTransformInfo()->vPos.z );
	D3DXVec3TransformCoord( &vMaxPos, pObject->GetMeshPtr()->GetMaxPtr(), &matPos );
	D3DXVec3TransformCoord( &vMinPos, pObject->GetMeshPtr()->GetMinPtr(), &matPos );

	//< 위에서 봤을때 겹치고, 옆에서봤을때 겹치면 무조건 겹친다.

	D3DXVECTOR3 vMax( 1, 1, 1 );
	D3DXVECTOR3 vMin( -1, -1, -1 );
	D3DXMatrixTranslation( &matPos, pPlayer->GetTransformInfo()->vPos.x, pPlayer->GetTransformInfo()->vPos.y, pPlayer->GetTransformInfo()->vPos.z );
	D3DXVec3TransformCoord( &vMax, &vMax, &matPos );
	D3DXVec3TransformCoord( &vMin, &vMin, &matPos );

	RECT rcTemp;
	RECT rcPlayerTop = { vMin.x, vMin.z, vMax.x, vMax.z };
	RECT rcObjTop = { vMinPos.x, vMinPos.z, vMaxPos.x, vMaxPos.z }; 
	RECT rcPlayerSide = { vMin.x, vMin.y, vMax.x, vMax.y };
	RECT rcObjSide = { vMinPos.x, vMinPos.y, vMaxPos.x, vMaxPos.y }; 
	if( IntersectRect( &rcTemp, &rcPlayerTop, &rcObjTop ) && IntersectRect( &rcTemp, &rcPlayerSide, &rcObjSide ) )
	{
		return true;
	}

	return false;
}