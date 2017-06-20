#include "stdafx.h"
#include "BaseBoneHierachy.h"

namespace BaseD3D
{

	//
	// 본 생성시 실행되는 함수
	//
	HRESULT BONE_HIERARCHY::CreateFrame(LPCSTR Name, 
		LPD3DXFRAME *ppNewFrame)
	{
		//ppNewFrame 로 넘겨줄 새로운 BONE 생성
		BONE*		newBone = new BONE;
		ZeroMemory( newBone, sizeof( BONE ) );

		//본 이름 복사
		if( Name != NULL )
		{
			int len = strlen(Name);
			newBone->Name = new char[len+1];
			strcpy_s( newBone->Name, len + 1, Name );
		}

		//매트릭스 정보 초기화
		D3DXMatrixIdentity( &newBone->TransformationMatrix );
		D3DXMatrixIdentity( &newBone->CombinedTransformationMatrix );

		//리턴값 대입
		*ppNewFrame = newBone;

		return S_OK;
	}


	HRESULT BONE_HIERARCHY::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer)
	{
		//ppNewMeshContainer 로 넘겨줄 새로운 BONEMESH 생성
		BONEMESH* boneMesh = new BONEMESH;
		ZeroMemory( boneMesh, sizeof( BONEMESH ) );

		//매개변수로 받은 메쉬데이터의 Device 를 얻는다.
		LPDIRECT3DDEVICE9		d3dDevice = NULL;
		pMeshData->pMesh->GetDevice( &d3dDevice );

		//이름 복사
		if( Name != NULL )
		{
			int len = strlen( Name );
			boneMesh->Name = new char[ len + 1 ];
			strcpy_s( boneMesh->Name, len + 1, Name );
		}

		boneMesh->MeshData.Type = pMeshData->Type;
		boneMesh->MeshData.pMesh = pMeshData->pMesh;
		boneMesh->MeshData.pMesh->AddRef();

		// copy adjacency over
		{
			DWORD dwNumFaces = boneMesh->MeshData.pMesh->GetNumFaces();
			boneMesh->pAdjacency = new DWORD[ 3 * dwNumFaces ];
			if( boneMesh->pAdjacency == NULL )
			{
				assert(false);
			}

			CopyMemory( boneMesh->pAdjacency, pAdjacency, 3 * sizeof( DWORD ) * dwNumFaces );
		}

		// Effects 인스턴스 무시
		boneMesh->pEffects = NULL;

		//재질 정보 
		boneMesh->NumMaterials = max( 1, NumMaterials );
		boneMesh->pMaterials = new D3DXMATERIAL           [ boneMesh->NumMaterials ];
		boneMesh->m_apTextures = new LPDIRECT3DTEXTURE9   [ boneMesh->NumMaterials ];
		if( boneMesh->pMaterials == NULL || boneMesh->m_apTextures == NULL )
		{
			assert(false);
		}

		if( NumMaterials > 0 )
		{
			CopyMemory( boneMesh->pMaterials, pMaterials, NumMaterials * sizeof( D3DXMATERIAL ) );
			for( DWORD i = 0; i < NumMaterials; ++ i )
			{
				if( boneMesh->pMaterials[ i ].pTextureFilename )
				{
					//절대경로 연결
					char szTemp[MAX_PATH] = {};
					strcpy_s(szTemp, m_Modelloading_dir);
					strcat_s(szTemp, boneMesh->pMaterials[ i ].pTextureFilename);



					// create the D3D texture
					if( FAILED( D3DXCreateTextureFromFile( m_pBSM->m_pDevice,
						szTemp,
						&boneMesh->m_apTextures[ i ] ) ) )
						boneMesh->m_apTextures[ i ] = NULL;
				}
				else
					boneMesh->m_apTextures[ i ] = NULL;
			}
		}
		else    // mock up a default material and set it
		{
			ZeroMemory( & boneMesh->pMaterials[ 0 ].MatD3D, sizeof( D3DMATERIAL9 ) );
			boneMesh->pMaterials[ 0 ].MatD3D.Diffuse.r = 0.5f;
			boneMesh->pMaterials[ 0 ].MatD3D.Diffuse.g = 0.5f;
			boneMesh->pMaterials[ 0 ].MatD3D.Diffuse.b = 0.5f;
			boneMesh->pMaterials[ 0 ].MatD3D.Specular = boneMesh->pMaterials[ 0 ].MatD3D.Diffuse;
			boneMesh->pMaterials[ 0 ].pTextureFilename = NULL;
		}

		//스킨 정보가 있다면...
		if( pSkinInfo != NULL )
		{
			boneMesh->pSkinInfo = pSkinInfo;

			//스킨 정보 레퍼런스 카운트 증가
			pSkinInfo->AddRef();

			boneMesh->m_amxBoneOffsets = new D3DXMATRIX[ pSkinInfo->GetNumBones() ];
			if( boneMesh->m_amxBoneOffsets == NULL )
			{
				assert(false);
			}

			{
				for( DWORD i = 0; i < pSkinInfo->GetNumBones(); ++ i )
					boneMesh->m_amxBoneOffsets[ i ] = * (D3DXMATRIX *) pSkinInfo->GetBoneOffsetMatrix( i );
			}

			//
			// Determine the palette size we need to work with, then call ConvertToIndexedBlendedMesh
			// to set up a new mesh that is compatible with the palette size.
			//
			{
				UINT iPaletteSize = 0;
				m_pBSM->m_pEffect->GetInt( "MATRIX_PALETTE_SIZE", (INT *) & iPaletteSize );
				boneMesh->m_dwNumPaletteEntries = min( iPaletteSize, boneMesh->pSkinInfo->GetNumBones() );
			}

			// generate the skinned mesh - creates a mesh with blend weights and indices
			boneMesh->pSkinInfo->ConvertToIndexedBlendedMesh( boneMesh->MeshData.pMesh,
				D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
				boneMesh->m_dwNumPaletteEntries,
				boneMesh->pAdjacency,
				NULL,
				NULL,
				NULL,
				&boneMesh->m_dwMaxNumFaceInfls,
				&boneMesh->m_dwNumAttrGroups,
				&boneMesh->m_pBufBoneCombos,
				&boneMesh->m_pWorkingMesh );

			if( m_pBSM->m_dwWorkingPaletteSize < boneMesh->m_dwNumPaletteEntries )
			{
				if( m_pBSM->m_amxWorkingPalette )
					delete [] m_pBSM->m_amxWorkingPalette;

				m_pBSM->m_dwWorkingPaletteSize = boneMesh->m_dwNumPaletteEntries;
				m_pBSM->m_amxWorkingPalette = new D3DXMATRIX[ m_pBSM->m_dwWorkingPaletteSize ];
				if( m_pBSM->m_amxWorkingPalette == NULL )
				{
					m_pBSM->m_dwWorkingPaletteSize = 0;
				}
			}

			DWORD dwOldFVF = boneMesh->m_pWorkingMesh->GetFVF();
			DWORD dwNewFVF = ( dwOldFVF & D3DFVF_POSITION_MASK ) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
			if( dwNewFVF != dwOldFVF )
			{
				LPD3DXMESH pMesh;

				boneMesh->m_pWorkingMesh->CloneMeshFVF( boneMesh->m_pWorkingMesh->GetOptions(),
					dwNewFVF,
					m_pBSM->m_pDevice,
					&pMesh );

				boneMesh->m_pWorkingMesh->Release();
				boneMesh->m_pWorkingMesh = pMesh;

				// if the loaded mesh didn't contain normals, compute them here
				if( ! ( dwOldFVF & D3DFVF_NORMAL ) )
				{
					D3DXComputeNormals( boneMesh->m_pWorkingMesh, NULL );
				}
			}

			// ensure the proper vertex format for the mesh
			{
				DWORD dwOldFVF = boneMesh->m_pWorkingMesh->GetFVF();
				DWORD dwNewFVF = ( dwOldFVF & D3DFVF_POSITION_MASK ) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
				if( dwNewFVF != dwOldFVF )
				{
					LPD3DXMESH pMesh;
					boneMesh->m_pWorkingMesh->CloneMeshFVF( boneMesh->m_pWorkingMesh->GetOptions(),
						dwNewFVF,
						m_pBSM->m_pDevice,
						&pMesh );

					boneMesh->m_pWorkingMesh->Release();
					boneMesh->m_pWorkingMesh = pMesh;

					// if the loaded mesh didn't contain normals, compute them here
					if( ! ( dwOldFVF & D3DFVF_NORMAL ) )
					{
						D3DXComputeNormals( boneMesh->m_pWorkingMesh, NULL );
					}
				}
			}

			// Interpret the UBYTE4 as a D3DCOLOR.
			// The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
			// blend indices to a D3DCOLOR semantic, and later in the shader convert
			// it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
			// convert any data, just the declaration.
			D3DVERTEXELEMENT9 pDecl[ MAX_FVF_DECL_SIZE ];
			D3DVERTEXELEMENT9 * pDeclCur;
			boneMesh->m_pWorkingMesh->GetDeclaration( pDecl );

			pDeclCur = pDecl;
			while( pDeclCur->Stream != 0xff )
			{
				if( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( pDeclCur->UsageIndex == 0 ) )
					pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
				pDeclCur++;
			}

			boneMesh->m_pWorkingMesh->UpdateSemantics( pDecl );
		}

		*ppNewMeshContainer = boneMesh;

		//얻어온 Device 해제 
		SAFE_RELEASE( d3dDevice );

		return S_OK;
	}


	HRESULT BONE_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
		if( pFrameToFree )
		{
			//이름 해제
			SAFE_DELETE_ARRAY( pFrameToFree->Name );

			SAFE_DELETE( pFrameToFree );
		}

		return S_OK;
	}


	HRESULT BONE_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		//boneMesh 로 형변환
		BONEMESH* boneMesh = (BONEMESH*)pMeshContainerBase;

		SAFE_DELETE_ARRAY( boneMesh->Name );

		SAFE_RELEASE( boneMesh->MeshData.pMesh );

		SAFE_DELETE_ARRAY( boneMesh->pAdjacency );

		SAFE_DELETE_ARRAY( boneMesh->pMaterials );

		for( DWORD i = 0; i < boneMesh->NumMaterials; ++ i )
		{
			SAFE_RELEASE( boneMesh->m_apTextures[ i ] );
		}

		SAFE_DELETE_ARRAY( boneMesh->m_apTextures );

		SAFE_RELEASE( boneMesh->pSkinInfo );

		SAFE_DELETE_ARRAY( boneMesh->m_amxBoneOffsets );

		SAFE_RELEASE( boneMesh->m_pWorkingMesh );

		boneMesh->m_dwNumPaletteEntries = 0;
		boneMesh->m_dwMaxNumFaceInfls = 0;
		boneMesh->m_dwNumAttrGroups = 0;

		SAFE_RELEASE( boneMesh->m_pBufBoneCombos );

		SAFE_DELETE( boneMesh->m_apmxBonePointers );

		SAFE_DELETE( boneMesh );


		return S_OK;
	}

	BONE_HIERARCHY::BONE_HIERARCHY(void)
	{
		memset(m_Modelloading_dir, 0, sizeof(m_Modelloading_dir));
	}


	void BONE_HIERARCHY::DirCopy(char* pModelloading_dir)
	{
		strcpy_s(m_Modelloading_dir, pModelloading_dir);
	}

	HRESULT BONEMESH::SetupBonePtrs( D3DXFRAME * pFrameRoot )
	{
		if( pSkinInfo )
		{
			if( m_apmxBonePointers )
				delete [] m_apmxBonePointers;

			DWORD dwNumBones = pSkinInfo->GetNumBones();

			m_apmxBonePointers = new D3DXMATRIX* [ dwNumBones ];
			if( m_apmxBonePointers == NULL )
				return E_OUTOFMEMORY;

			for( DWORD i = 0; i < dwNumBones; ++ i )
			{
				BONE *pBone = (BONE*) D3DXFrameFind( pFrameRoot, pSkinInfo->GetBoneName( i ) );
				if( pBone == NULL )
					return E_FAIL;

				m_apmxBonePointers[ i ] = &pBone->TransformationMatrix;
			}
		}

		return S_OK;
	}

	void BONE_HIERARCHY::SetBSM(BaseSkinnedMesh* pBSM)
	{
		m_pBSM = pBSM;
	}


}