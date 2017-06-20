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
		SkinnenMeshesRelease(); //< �����߰�
	}

	//�޽� �ε� �Լ�
	HRESULT		BaseSkinnedMesh::Init( LPDIRECT3DDEVICE9 pd3dDevice, const std::string& name, const D3DXMATRIX* pmat )
	{
		m_pDevice = pd3dDevice;

		MAP_SKINNED_MESH_ITER find = s_SkinnedMeshes.find( name );

		//������ �ε��� ������ �ִٸ�..
		if( !s_SkinnedMeshes.empty() && find != s_SkinnedMeshes.end() )
		{

			//��Ʈ �� ����
			m_pRootBone = find->second->pRootBone;

			//�ִϸ��̼� ��Ʈ�� ����
			find->second->pAnimControl->CloneAnimationController(
				find->second->pAnimControl->GetMaxNumAnimationOutputs(),
				find->second->pAnimControl->GetMaxNumAnimationSets(),
				find->second->pAnimControl->GetMaxNumTracks(),
				find->second->pAnimControl->GetMaxNumEvents(),
				&m_pAnimControl );

			// ����Ʈ ����
			m_pEffect = find->second->pEffect;

			// �ȷ�Ʈ ����
			m_dwWorkingPaletteSize = find->second->dwWorkingPaletteSize;
			m_amxWorkingPalette = find->second->amxWorkingPalette;

		}

		//������ �ε��� ������ ���ٸ�..
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

			//ID3DXAllocateHierarchy  ��ӹ޾� �޽������̳� ����������� 
			//�籸���� Ŭ������ ��ü ( ���⼭�� ���� ����� ���� �ʿ��ϴ� )
			BONE_HIERARCHY boneHirearchy;

			// X���� ��� �Ľ�
			boneHirearchy.DirCopy(Modelloading_dir);

			//����
			ST_LPSKINNED_MESH newMesh = new ST_SKINNED_MESH;
			ZeroMemory( newMesh, sizeof( ST_SKINNED_MESH ) );

			boneHirearchy.SetBSM(this);

			//���Ϸ� ���� 
			D3DXLoadMeshHierarchyFromX(
				name.c_str(),				//���� ��� ��
				0,			//�޽� �ɼ� ( �޽������ʹ� �����޸�Ǯ���� ���� �� )
				pd3dDevice,					//����̽�
				&boneHirearchy,				//�޽� �ε� ����� ������ ID3DXAllocateHierarchy ��ӹ��� ��ü
				NULL,						//Effect
				(LPD3DXFRAME *) &newMesh->pRootBone,			//���� RootFrame( ���� "cBoneHierarchy.h" ������ BONE ����ü�� ����Ű�� �ȴ� )
				&newMesh->pAnimControl		//���� ���ϸ��̼� ��Ʈ��
				);


			//��Ʈ �� ����
			if(newMesh->pRootBone != NULL)
			{
				m_pRootBone = newMesh->pRootBone;
			}
			else
			{
				assert(false);
			}

			SetupBonePtrs( (BONE*) newMesh->pRootBone );

			// ����Ʈ ����
			newMesh->pEffect = m_pEffect;

			// �ȷ�Ʈ ����
			newMesh->dwWorkingPaletteSize = m_dwWorkingPaletteSize;
			newMesh->amxWorkingPalette = m_amxWorkingPalette;

			//Ǫ��
			s_SkinnedMeshes.insert( std::make_pair( name, newMesh ) );

			//�ִϸ��̼� ��Ʈ�� ����
			newMesh->pAnimControl->CloneAnimationController(
				newMesh->pAnimControl->GetMaxNumAnimationOutputs(),
				newMesh->pAnimControl->GetMaxNumAnimationSets(),
				newMesh->pAnimControl->GetMaxNumTracks(),
				newMesh->pAnimControl->GetMaxNumEvents(),
				&m_pAnimControl );

			//�� ��Ʈ���� ������ ����
			//SetupBoneMatrixPointer( (BONE*)m_pRootBone );
		}


		if( pmat == NULL )
			D3DXMatrixIdentity( &m_mat );

		else
			m_mat = *pmat;

		return S_OK;
	}

	//���� �Լ�
	void BaseSkinnedMesh::Release()
	{
		//ID3DXAllocateHierarchy  ��ӹ޾� �޽������̳� ����������� 
		//�籸���� Ŭ������ ��ü ( ���⼭�� ���� ����� ���� �ʿ��ϴ� )
		//BONE_HIERARCHY boneHirearchy;
		//D3DXFrameDestroy( m_pRootBone, &boneHirearchy );

		//�ִϸ��̼� ��Ʈ�ѷ�
		SAFE_RELEASE( m_pAnimControl );
	}

	//���ϸ��̼� ���� �Լ�
	void			BaseSkinnedMesh::SetAnimationName(const std::string & actionName, bool bSetBegin )
	{
		//�ش� �̸��� �ִϸ��̼� ���� ��´�
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSetByName( actionName.c_str(), &pAnim );

		if( pAnim == NULL ) return;

		//�ִϸ��̼��� ���������� ���Ӵٸ�...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//���õǴ� �ִϸ��̼��� ���̸� ��´�.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//���ϸ��̼��� 0 �� Ʈ���� �����Ѵ�,

		//������ ��ġ �ʱ�ȭ
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );

		pAnim->Release();		//���ÿ� ���������� ���� �ִϸ��̼� �� ����


	}

	void		BaseSkinnedMesh::SetAnimationIndex(int index, bool bSetBegin )
	{
		//�ش� �ε����� ���ϸ��̼� ���� ��´�.
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSet( index, &pAnim );

		if( pAnim == NULL ) return;

		//�ִϸ��̼��� ���������� ���Ӵٸ�...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//���õǴ� �ִϸ��̼��� ���̸� ��´�.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//���ϸ��̼��� 0 �� Ʈ���� �����Ѵ�,

		//������ ��ġ �ʱ�ȭ
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );

		pAnim->Release();		//���ÿ� ���������� ���� �ִϸ��̼� �� ����

		// ������ ������ �ִϸ��̼� �ε���
		m_AniIndex = index;

		// ���� �ð� üũ
		m_dPassedTime = 0.0;
		m_bPassed = false;
	}

	void		BaseSkinnedMesh::SetAnimationIndexBelnd(int index, bool bSetBegin, DOUBLE blendTime )
	{
		//�ش� �ε����� ���ϸ��̼� ���� ��´�.
		LPD3DXANIMATIONSET		pAnim = NULL;
		m_pAnimControl->GetAnimationSet( index, &pAnim );

		//�ִϸ��̼� ��� ���н� ����
		if( pAnim == NULL ) return;

		//������ �ִϸ��̼� ���� ��´�
		LPD3DXANIMATIONSET		pPrev = NULL;
		m_pAnimControl->GetTrackAnimationSet( 0, &pPrev );

		if( pPrev != NULL )
		{
			//���� Ű���������� ����
			D3DXTRACK_DESC Desc;
			m_pAnimControl->GetTrackDesc(0, &Desc);

			//���� Ÿ�� ����
			m_dBlendingTime = blendTime;
			m_dLeftChange = blendTime;
			m_dLeftBlendingWeight = 1.0f;

			//������ �ִϸ��̼��� 1�� Ʈ������ �����Ѵ�
			m_pAnimControl->SetTrackAnimationSet( 1, pPrev );

			//1�� Ʈ�� Ȱ��ȭ
			m_pAnimControl->SetTrackEnable( 1, TRUE );
			m_pAnimControl->SetTrackWeight( 1, 1.0f);
			m_pAnimControl->SetTrackPosition( 1, Desc.Position);
			m_pAnimControl->SetTrackSpeed( 1, 0.0f );

			//���� ���� �ִϸ��̼� ���� 
			pPrev->Release();
		}

		//�ִϸ��̼��� ���������� ���Ӵٸ�...
		m_CurrentAnimPeriod = pAnim->GetPeriod();				//���õǴ� �ִϸ��̼��� ���̸� ��´�.
		m_pAnimControl->SetTrackAnimationSet( 0, pAnim );		//���ϸ��̼��� 0 �� Ʈ���� �����Ѵ�,
		m_pAnimControl->SetTrackWeight( 0, 0.0 );


		//������ ��ġ �ʱ�ȭ
		if( bSetBegin )
			m_pAnimControl->SetTrackPosition( 0, 0.0 );


		pAnim->Release();		//���ÿ� ���������� ���� �ִϸ��̼� �� ����
	}

	//���ϸ��̼� �����Ӽ��� �Լ�
	void		BaseSkinnedMesh::SetFrame( DOUBLE frameTime )
	{
		m_pAnimControl->SetTrackPosition( 0, frameTime );		//�ִϸ��̼� �ð��� �����Ѵ�.
		m_pAnimControl->AdvanceTime(0.0, NULL );					//�ִϸ��̼��� �����Ų��.
	}

	//������ �����Լ�
	void		BaseSkinnedMesh::PlayFrame( DOUBLE deltaFrameTime )
	{
		//���� ����ġ�� �ִٸ�...
		if( m_dLeftChange >= 0.0 )
		{
			m_dLeftChange -= deltaFrameTime;

			//������ �����ٸ�...
			if( m_dLeftChange <= 0.0f )
			{
				//1 ��Ʈ�� ����
				m_pAnimControl->SetTrackEnable( 1, FALSE );
				m_pAnimControl->SetTrackWeight( 0, 1.0f );
			}

			//������ ������ �ʾҴٸ�..
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

		m_pAnimControl->AdvanceTime( deltaFrameTime, NULL);					//�ִϸ��̼��� �����Ų��.
	}

	//���� �Լ�
	void		BaseSkinnedMesh::Render( D3DXMATRIXA16& matWorld, DOUBLE deltaFrameTime )
	{
		m_pRootBone->TransformationMatrix = matWorld;

		PlayFrame(deltaFrameTime);

		m_pAnimControl->AdvanceTime( 0.0, NULL );

		//��Ʈ���� ������Ʈ
		UpdateMatrices( (BONE*)m_pRootBone, &m_mat );

		//��Ų �޽� ������Ʈ
		UpdateSkinnedMesh( (BONE*)m_pRootBone );

	}

	//������ ����ӵ� ����
	void			BaseSkinnedMesh::SetPlaySpeed( DOUBLE speed )
	{
		m_pAnimControl->SetTrackSpeed( 0, FLOAT(speed) );
	}


	//Map�� �ִ� �޽��� �� ���� ������.
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

	//��Ʈ���� ������Ʈ �Լ�
	void		BaseSkinnedMesh::UpdateMatrices( BONE* pBone, const D3DXMATRIX* parentMatrix )
	{
		if( pBone == NULL ) return;

		D3DXMatrixMultiply( 
			&pBone->TransformationMatrix,
			&pBone->TransformationMatrix, 
			parentMatrix );

		//�̿� ���� �����Ѵٸ�....
		if( pBone->pFrameSibling ) 
			UpdateMatrices( (BONE*)pBone->pFrameSibling, parentMatrix );

		//�ڽ� ���� �����Ѵٸ�...
		if( pBone->pFrameFirstChild )
			UpdateMatrices( (BONE*)pBone->pFrameFirstChild, &pBone->TransformationMatrix );

	}

	//��Ű�� ������Ʈ �Լ�
	void		BaseSkinnedMesh::UpdateSkinnedMesh( BONE* pBone )
	{
		//���� NULL �� ������ �θ���� ����ȴ�.
		//if( pBone == NULL )
		//	pBone = (BONE*)m_pRootBone;


		//�׸� �������̳ʰ� �����Ѵٸ�....
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
					cDevice::GetInstance().SetWireFrameTexture( WF_WHITE ); //< ���̾�������
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

	//�ش� ��带 ã�´�.
	LPD3DXFRAME BaseSkinnedMesh::GetFindBone( LPD3DXFRAME Node , char* NodeName )
	{
		LPD3DXFRAME m_pFindNode = NULL;

		//���� Ȯ��
		if(Node->Name != NULL)
		{
			if( strcmp(Node->Name , NodeName) == 0 )
			{
				return Node;
			}
		}
		

		//���� ��带 Ȯ�� 
		if( Node->pFrameSibling != NULL )
		{
			m_pFindNode = GetFindBone(Node->pFrameSibling,	NodeName );

			if( m_pFindNode != NULL)
			{
				return m_pFindNode;
			}
		}

		//�ڽ� ��带 Ȯ��
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

