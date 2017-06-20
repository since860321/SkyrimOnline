#pragma once
#include "defines.h"
#include <vector>
#include "BaseSkinnedMesh.h"

namespace BaseD3D
{
	//
	// D3DXFRAME 을 상속받은 BONE 구조체
	//
	struct BONE : public D3DXFRAME
	{
		D3DXMATRIX	CombinedTransformationMatrix;			//모든 행렬이 적용되어 변환이 완료된 행렬
	};

	//
	// D3DXMESHCONTAINER 를 상속받은 BONEMESH 구조체
	//
	struct BONEMESH : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9 *m_apTextures;
		LPD3DXMESH          m_pWorkingMesh;
		D3DXMATRIX *        m_amxBoneOffsets;		// Bone offset matrices retrieved from pSkinInfo
		D3DXMATRIX **       m_apmxBonePointers;		// Provides index to bone matrix lookup

		DWORD               m_dwNumPaletteEntries;
		DWORD               m_dwMaxNumFaceInfls;
		DWORD               m_dwNumAttrGroups;
		LPD3DXBUFFER        m_pBufBoneCombos;

		HRESULT SetupBonePtrs( D3DXFRAME * pFrameRoot );
	};



	//
	// ID3DXAllocateHierarchy 클래스(인터페이스)를 상속받는 BONE_HIERARCHY 클래스
	// ID3DXAllocateHierarchy 는 순수가상 멤버 함수로 되어있어
	// 해당 멤버 함수들이 작동하는 것에 대한 재정의를 목표로 한다.
	//
	class BaseSkinnedMesh;
	class BONE_HIERARCHY : public ID3DXAllocateHierarchy
	{
	private:
		// 텍스쳐의 경로는 X파일의 경로와 같아야 한다.
		char				m_Modelloading_dir[MAX_PATH];
		BaseSkinnedMesh*	m_pBSM;

	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
			LPD3DXFRAME *ppNewFrame) override;

		STDMETHOD(CreateMeshContainer)(THIS_ 
			LPCSTR Name, 
			CONST D3DXMESHDATA *pMeshData, 
			CONST D3DXMATERIAL *pMaterials, 
			CONST D3DXEFFECTINSTANCE *pEffectInstances, 
			DWORD NumMaterials, 
			CONST DWORD *pAdjacency, 
			LPD3DXSKININFO pSkinInfo, 
			LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override; 

		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override; 

		void DirCopy(char* pModelloading_dir);

		BONE_HIERARCHY(void);

		void SetBSM(BaseSkinnedMesh* pBSM);
	};



}