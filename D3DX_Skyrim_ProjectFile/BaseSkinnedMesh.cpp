#include "stdafx.h"
#include "BaseSkinnedMesh.h"


namespace BaseD3D
{
	BaseSkinnedMesh::BaseSkinnedMesh(void)
		: m_pRootBone( NULL ), 
		m_pAnimControl( NULL ), 
		m_CurrentAnimPeriod( 0.0 ), 
		m_dLeftChange( 0.0 ), 
		m_dLeftBlendingWeight( 0.0 ), 
		m_dBlendingTime( 0.0 ), 
		m_AniIndex( 0 ), 
		m_dPassedTime( 0 ),
		m_pEffect( NULL ),
		m_dwWorkingPaletteSize( 0 ),
		m_amxWorkingPalette( NULL )
	{
	}

	BaseSkinnedMesh::~BaseSkinnedMesh(void)
	{
		Release();
		SkinnenMeshesRelease(); //< 형석추가
	}

	//메쉬 로드 함수
	HRESULT		BaseSkinnedMesh::Init( LPDIRECT3DDEVICE9 pd3dDevice, const std::string& name, const D3DXMATRIX* pmat )
	{
		m_pDevice = pd3dDevice;

		MAP_SKINNED_MESH_ITER find = s_SkinnedMeshes.find( name );

		//기존에 로딩이 된적이 있다면..
		if( !s_SkinnedMeshes.empty() && find != s_SkinnedMeshes.end() )
		{

			//루트 본 대입
			m_pRootBone = find->second->pRootBone;

			//애니메이션 컨트롤 복사
			find->second->pAnimControl->CloneAnimationController(
				find->second->pAnimControl->GetMaxNumAnimationOutputs(),
				find->second->pAnimControl->GetMaxNumAnimationSets(),
				find->second->pAnimControl->GetMaxNumTracks(),
				find->second->pAnimControl->GetMaxNumEvents(),
				&m_pAnimControl );

			// 이펙트 복사
			m_pEffect = find->second->pEffect;

			// 팔레트 복사
			m_dwWorkingPaletteSize = find->second->dwWorkingPaletteSize;
			m_amxWorkingPalette = find->second->amxWorkingPalette;

		}

		//기존에 로딩이 된적이 없다면..
		else
		{
			D3DXMACRO mac[2] =
			{
				{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
				{ NULL,                          NULL }
			};

			D3DCAPS9 caps;
			D3DXMACRO *pmac = NULL;
			m_pDevice->GetDeviceCaps( & caps );
			if( caps.VertexShaderVersion > D3DVS_VERSION( 1, 1 ) )
				pmac = mac;

			DWORD dwShaderFlags = 0;

			#if defined( DEBUG ) || defined( _DEBUG )
				// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
				// Setting this flag improves the shader debugging experience, but still allows 
				// the shaders to be optimized and to run exactly the way they will run in 
				// the release configuration of this program.
				dwShaderFlags |= D3DXSHADER_DEBUG;
			#endif

			#ifdef DEBUG_VS
				dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
			#endif
			#ifdef DEBUG_PS
				dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
			#endif

			ID3DXBuffer* pBuffer = NULL;
			if(FAILED(D3DXCreateEffectFromFile( m_pDevice,
				"MultiAnimation.fx",
				pmac,
				NULL,
				dwShaderFlags,
				NULL,
				&m_pEffect,
				NULL )))
			{
					// if creation fails, and debug information has been returned, output debug info
					if (pBuffer)
					{
						OutputDebugStringA((char*)pBuffer->GetBufferPointer());
						SAFE_RELEASE(pBuffer);
					}

					MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", "ERROR", 0);
					return E_FAIL;
				
			}

			char Modelloading_dir[MAX_PATH];
			char drv[10];
			char dir[MAX_PATH];
			char file_name[100];
			char ext[10];
			_splitpath_s(name.c_str(),drv,dir,file_name,ext);

			strcpy_s(Modelloading_dir, drv); 
			strcat_s(Modelloading_dir, dir); 

			//ID3DXAllocateHierarchy  상속받아 메쉬생성이나 해제방법등을 
			//재구성한 클래스의 객체 ( 여기서는 생성 방법을 위해 필요하다 )
			BONE_HIERARCHY boneHirearchy;

			// X파일 경로 파싱
			boneHirearchy.DirCopy(Modelloading_dir);

			//생성
			ST_LPSKINNED_MESH newMesh = new ST_SKINNED_MESH;
			ZeroMemory( newMesh, sizeof( ST_SKINNED_MESH ) );

			boneHirearchy.SetBSM(this);

			//파일로 부터 
			D3DXLoadMeshHierarchyFromX(
				name.c_str(),				//파일 경로 명
				0,			//메쉬 옵션 ( 메쉬데이터는 관리메모리풀내에 보관 됨 )
				pd3dDevice,					//디바이스
				&boneHirearchy,				//메쉬 로딩 방법을 지정한 ID3DXAllocateHierarchy 상속받은 객체
				NULL,						//Effect
				(LPD3DXFRAME *) &newMesh->pRootBone,			//얻어올 RootFrame( 실제 "cBoneHierarchy.h" 정의한 BONE 구조체를 가리키게 된다 )
				&newMesh->pAnimControl		//얻어올 에니메이션 컨트롤
				);


			//루트 본 대입
			if(newMesh->pRootBone != NULL)
			{
				m_pRootBone = newMesh->pRootBone;
			}
			else
			{
				assert(false);
			}

			SetupBonePtrs( (BONE*) newMesh->pRootBone );

			// 이펙트 복사
			newMesh->pEffect = m_pEffect;

			// 팔레트 복사
			newMesh->dwWorkingPaletteSize = m_dwWorkingPaletteSize;
			newMesh->amxWorkingPalette = m_amxWorkingPalette;

			//푸쉬
			s_SkinnedMeshes.insert( std::make_pair( name, newMesh ) );

			//애니메이션 컨트롤 복사
			newMesh->pAnimControl->CloneAnimationController(
				newMesh->pAnimControl->GetMaxNumAnimationOutputs(),
				newMesh->pAnimControl->GetMaxNumAnimationSets(),
				newMesh->pAnimControl->GetMaxNumTracks(),
				newMesh->pAnimControl->GetMaxNumEvents(),
				&m_pAnimControl );

			//본 매트릭스 포인터 생성
			//SetupBoneMatrixPointer( (BONE*)m_pRootBone );
		}


		if( pmat == NULL )
			D3DXMatrixIdentity( &m_mat );

		else
			m_mat = *pmat;

		return S_OK;
	}

	//해제 함수
	void BaseSkinnedMesh::Release()
	{
		//ID3DXAllocateHierarchy  상속받아 메쉬생성이나 해제방법등을 
		//재구성한 클래스의 객체 ( 여기서는 해제 방법을 위해 필요하다 )
		//BONE_HIERARCHY boneHirearchy;
		//D3DXFrameDestroy( m_pRootBone, &boneHirearchy );

		//애니메이션 컨트롤러
		SAFE_RELEASE( m_pAnimControl );
	}

	//에니메이션 셋팅 함수
	void			BaseSkinnedMesh::SetAnimationName(const std::string & actionName, bool bSetBegin )
	{
		//해당 이름의 애니메이션 셋을 얻는다
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSetByName( actionName.c_str(), &pAnim );

		if( pAnim == NULL ) return;

		//애니메이션을 정상적으로 얻어왓다면...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//셋팅되는 애니메이션의 길이를 얻는다.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//에니메이션을 0 번 트랙에 셋팅한다,

		//프레임 위치 초기화
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );

		pAnim->Release();		//셋팅에 성공했으면 얻어온 애니메이션 셋 해제


	}

	void		BaseSkinnedMesh::SetAnimationIndex(int index, bool bSetBegin )
	{
		//해당 인덱스의 에니메이션 셋을 얻는다.
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSet( index, &pAnim );

		if( pAnim == NULL ) return;

		//애니메이션을 정상적으로 얻어왓다면...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//셋팅되는 애니메이션의 길이를 얻는다.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//에니메이션을 0 번 트랙에 셋팅한다,

		//프레임 위치 초기화
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );

		pAnim->Release();		//셋팅에 성공했으면 얻어온 애니메이션 셋 해제

		// 앞으로 관리할 애니메이션 인덱스
		m_AniIndex = index;

		// 지난 시간 체크
		m_dPassedTime = 0.0;
		m_bPassed = false;
	}

	void		BaseSkinnedMesh::SetAnimationIndexBelnd(int index, bool bSetBegin, DOUBLE blendTime )
	{
		//해당 인덱스의 에니메이션 셋을 얻는다.
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSet( index, &pAnim );

		//애니메이션 얻기 실패시 리턴
		if( pAnim == NULL ) return;

		//기존의 애니메이션 셋을 얻는다
		LPD3DXANIMATIONSET		pPrev = NULL;
		m_pAnimControl->GetTrackAnimationSet( 0, &pPrev );

		if( pPrev != NULL )
		{
			//기존 키프레임으로 블렌딩
			D3DXTRACK_DESC Desc;
			m_pAnimControl->GetTrackDesc(0, &Desc);

			//블렌딩 타임 대입
			m_dBlendingTime = blendTime;
			m_dLeftChange = blendTime;
			m_dLeftBlendingWeight = 1.0f;

			//기존의 애니메이션을 1번 트렉으로 셋팅한다
			m_pAnimControl->SetTrackAnimationSet( 1, pPrev );

			//1번 트렉 활성화
			m_pAnimControl->SetTrackEnable( 1, TRUE );
			m_pAnimControl->SetTrackWeight( 1, 1.0f);
			m_pAnimControl->SetTrackPosition( 1, Desc.Position);
			m_pAnimControl->SetTrackSpeed( 1, 0.0f );

			//얻어온 기존 애니메이션 해제 
			pPrev->Release();
		}

		//애니메이션을 정상적으로 얻어왓다면...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//셋팅되는 애니메이션의 길이를 얻는다.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//에니메이션을 0 번 트랙에 셋팅한다,
		m_pAnimControl->SetTrackWeight( 0, 0.0 );


		//프레임 위치 초기화
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );


		pAnim->Release();		//셋팅에 성공했으면 얻어온 애니메이션 셋 해제
	}

	//에니메이션 프레임셋팅 함수
	void		BaseSkinnedMesh::SetFrame( DOUBLE frameTime )
	{
		m_pAnimControl->SetTrackPosition( 0, frameTime );		//애니메이션 시간을 셋팅한다.
		m_pAnimControl->AdvanceTime(0.0, NULL );					//애니메이션을 진행시킨다.
	}

	//프레임 진행함수
	void		BaseSkinnedMesh::PlayFrame( DOUBLE deltaFrameTime )
	{
		//블렌딩 가중치가 있다면...
		if( m_dLeftChange >= 0.0 )
		{
			m_dLeftChange -= deltaFrameTime;

			//블렌딩이 끝났다면...
			if( m_dLeftChange <= 0.0f )
			{
				//1 번트랙 해제
				m_pAnimControl->SetTrackEnable( 1, FALSE );
				m_pAnimControl->SetTrackWeight( 0, 1.0f );
			}

			//블렌딩이 끝나지 않았다면..
			else
			{
				m_dLeftBlendingWeight =  m_dLeftChange / m_dBlendingTime;
				m_pAnimControl->SetTrackWeight( 1, FLOAT(m_dLeftBlendingWeight) );
				m_pAnimControl->SetTrackWeight( 0, 1 - FLOAT(m_dLeftBlendingWeight) );
			}
		}

		m_dPassedTime += deltaFrameTime; // Testing...
		if(m_CurrentAnimPeriod < m_dPassedTime + deltaFrameTime * 2.0)
		{
			m_bPassed = true;
		}

		m_pAnimControl->AdvanceTime( deltaFrameTime, NULL);					//애니메이션을 진행시킨다.
	}

	//랜더 함수
	void		BaseSkinnedMesh::Render( D3DXMATRIXA16& matWorld, DOUBLE deltaFrameTime )
	{
		m_pRootBone->TransformationMatrix = matWorld;

		PlayFrame(deltaFrameTime);

		m_pAnimControl->AdvanceTime( 0.0, NULL );

		//메트릭스 업데이트
		UpdateMatrices( (BONE*)m_pRootBone, &m_mat );

		//스킨 메쉬 업데이트
		UpdateSkinnedMesh( (BONE*)m_pRootBone );

	}

	//프레임 진행속도 셋팅
	void			BaseSkinnedMesh::SetPlaySpeed( DOUBLE speed )
	{
		m_pAnimControl->SetTrackSpeed( 0, FLOAT(speed) );
	}


	//Map에 있는 메쉬들 다 날려 제낀다.
	void BaseSkinnedMesh::SkinnenMeshesRelease(void)
	{
		BONE_HIERARCHY boneHirearchy;

		MAP_SKINNED_MESH_ITER iter;

		for( iter = s_SkinnedMeshes.begin() ; iter != s_SkinnedMeshes.end() ; ++iter )
		{
			D3DXFrameDestroy( iter->second->pRootBone, &boneHirearchy );
			SAFE_RELEASE( iter->second->pAnimControl );

			SAFE_DELETE( iter->second );
		}

		s_SkinnedMeshes.clear();

	}

	//매트릭스 업데이트 함수
	void		BaseSkinnedMesh::UpdateMatrices( BONE* pBone, const D3DXMATRIX* parentMatrix )
	{
		if( pBone == NULL ) return;

		D3DXMatrixMultiply( 
			&pBone->TransformationMatrix,
			&pBone->TransformationMatrix, 
			parentMatrix );

		//이웃 본이 존재한다면....
		if( pBone->pFrameSibling ) 
			UpdateMatrices( (BONE*)pBone->pFrameSibling, parentMatrix );

		//자식 본이 존재한다면...
		if( pBone->pFrameFirstChild )
			UpdateMatrices( (BONE*)pBone->pFrameFirstChild, &pBone->TransformationMatrix );

	}

	//스키닝 업데이트 함수
	void		BaseSkinnedMesh::UpdateSkinnedMesh( BONE* pBone )
	{
		//최초 NULL 이 들어오면 부모부터 실행된다.
		//if( pBone == NULL )
		//	pBone = (BONE*)m_pRootBone;


		//그릴 본컨테이너가 존재한다면....
		if( pBone->pMeshContainer )
		{
			BONEMESH * boneMesh = (BONEMESH *) pBone->pMeshContainer;

			// get bone combinations
			LPD3DXBONECOMBINATION pBC = ( LPD3DXBONECOMBINATION )( boneMesh->m_pBufBoneCombos->GetBufferPointer() );
			DWORD dwAttrib, dwPalEntry;

			// for each palette
			for( dwAttrib = 0; dwAttrib < boneMesh->m_dwNumAttrGroups; ++ dwAttrib )
			{
				// set each transform into the palette
				for( dwPalEntry = 0; dwPalEntry < boneMesh->m_dwNumPaletteEntries; ++ dwPalEntry )
				{
					DWORD dwMatrixIndex = pBC[ dwAttrib ].BoneId[ dwPalEntry ];
					if( dwMatrixIndex != UINT_MAX )
					{
						D3DXMatrixMultiply( &m_amxWorkingPalette[ dwPalEntry ],
						&( boneMesh->m_amxBoneOffsets[ dwMatrixIndex ] ),
						boneMesh->m_apmxBonePointers[ dwMatrixIndex ] );
					}
				}

				// set the matrix palette into the effect
				m_pEffect->SetMatrixArray( "amPalette",
					m_amxWorkingPalette,
					boneMesh->m_dwNumPaletteEntries );

				D3DXMATRIX mx;
				D3DXMATRIXA16 matView, matProj;
				m_pDevice->GetTransform(D3DTS_VIEW, &matView);
				m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

				D3DXMatrixMultiply( & mx, &matView, &matProj );

				m_pEffect->SetMatrix("g_mViewProj", &mx);

				D3DXVECTOR3		vDrt(1, -1, 0);
				D3DXVec3Normalize(&vDrt, &vDrt);
				m_pEffect->SetVector("lhtDir", &D3DXVECTOR4( vDrt, 1.0f ) );
				m_pEffect->SetVector("lightDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f  ) );

				m_pEffect->SetVector("MaterialAmbient", &D3DXVECTOR4( 0.53f, 0.53f, 0.53f, 0.53f  ) );
				m_pEffect->SetVector("MaterialDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f  ) );
 

				// we're pretty much ignoring the materials we got from the x-file; just set
				// the texture here
				m_pEffect->SetTexture( "g_txScene", boneMesh->m_apTextures[ pBC[ dwAttrib ].AttribId ] );

				// set the current number of bones; this tells the effect which shader to use
				m_pEffect->SetInt( "CurNumBones", boneMesh->m_dwMaxNumFaceInfls - 1 );

				// set the technique we use to draw
				m_pEffect->SetTechnique( "Skinning20" );

				UINT uiPasses, uiPass;

				// run through each pass and draw
				m_pEffect->Begin( & uiPasses, 0 );
				for( uiPass = 0; uiPass < uiPasses; ++ uiPass )
				{
					m_pEffect->BeginPass( uiPass );
					cDevice::GetInstance().SetWireFrameTexture( WF_WHITE ); //< 와이어프레임
					boneMesh->m_pWorkingMesh->DrawSubset( dwAttrib );
					m_pEffect->EndPass();
				}
				m_pEffect->End();
			}
			
			
		}


		if( pBone->pFrameSibling != NULL ) UpdateSkinnedMesh( (BONE*)pBone->pFrameSibling );
		if( pBone->pFrameFirstChild != NULL ) UpdateSkinnedMesh( (BONE*)pBone->pFrameFirstChild );
	}

	void		BaseSkinnedMesh::SetupBonePtrs( BONE * pBone )
	{
		assert( pBone != NULL );

		if( pBone->pMeshContainer )
		{
			// call setup routine
			((BONEMESH*) pBone->pMeshContainer )->SetupBonePtrs( m_pRootBone );
		}

		if( pBone->pFrameSibling )
		{
			// recursive call
			SetupBonePtrs( (BONE*) pBone->pFrameSibling );
		}

		if( pBone->pFrameFirstChild )
		{
			// recursive call
			SetupBonePtrs( (BONE*) pBone->pFrameFirstChild );
		}

	}

	void BaseSkinnedMesh::SetWorldMatrix( D3DXMATRIXA16& matWorld )
	{
		m_mat = matWorld;
	}

	//해당 노드를 찾는다.
	LPD3DXFRAME BaseSkinnedMesh::GetFindBone( LPD3DXFRAME Node , char* NodeName )
	{
		LPD3DXFRAME m_pFindNode = NULL;

		//나를 확인
		if(Node->Name != NULL)
		{
			if( strcmp(Node->Name , NodeName) == 0 )
			{
				return Node;
			}
		}
		

		//형제 노드를 확인 
		if( Node->pFrameSibling != NULL )
		{
			m_pFindNode = GetFindBone(Node->pFrameSibling,	NodeName );

			if( m_pFindNode != NULL)
			{
				return m_pFindNode;
			}
		}

		//자식 노드를 확인
		if( Node->pFrameFirstChild != NULL )
		{
			m_pFindNode = GetFindBone(Node->pFrameFirstChild, NodeName);

			if( m_pFindNode != NULL)
			{
				return m_pFindNode;
			}

		}

		return m_pFindNode;
	}

}

