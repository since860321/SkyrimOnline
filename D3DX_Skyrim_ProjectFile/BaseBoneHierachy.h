#pragma once
#include "defines.h"
#include <vector>
#include "BaseSkinnedMesh.h"

namespace BaseD3D
{
	//
	// D3DXFRAME �� ��ӹ��� BONE ����ü
	//
	struct BONE : public D3DXFRAME
	{
		D3DXMATRIX	CombinedTransformationMatrix;			//��� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
	};

	//
	// D3DXMESHCONTAINER �� ��ӹ��� BONEMESH ����ü
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
	// ID3DXAllocateHierarchy Ŭ����(�������̽�)�� ��ӹ޴� BONE_HIERARCHY Ŭ����
	// ID3DXAllocateHierarchy �� �������� ��� �Լ��� �Ǿ��־�
	// �ش� ��� �Լ����� �۵��ϴ� �Ϳ� ���� �����Ǹ� ��ǥ�� �Ѵ�.
	//
	class BaseSkinnedMesh;
	class BONE_HIERARCHY : public ID3DXAllocateHierarchy
	{
	private:
		// �ؽ����� ��δ� X������ ��ο� ���ƾ� �Ѵ�.
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