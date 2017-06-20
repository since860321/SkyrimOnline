#pragma once
#include "defines.h"
//#include <d3dx9.h>
#include "BaseBoneHierachy.h"
#include "cObject.h"
#include <map>
#include <string>

class cCamera;
namespace BaseD3D
{
	struct BONE; //< �߰���(����)

	// ��Ƽ�ִϸ��̼� �޽� �����̳�
	typedef struct _ST_SKINNED_MESH
	{
		LPD3DXFRAME					pRootBone;		//�ֻ��� �� ������ ( ���� "cBoneHierarchy.h" ������ BONE ����ü�� ����Ű�� �ȴ� )
		LPD3DXANIMATIONCONTROLLER	pAnimControl;	//�ִϸ��̼� ��Ʈ��
		LPD3DXEFFECT				pEffect;
		DWORD						dwWorkingPaletteSize;
		D3DXMATRIX *				amxWorkingPalette;
	}ST_SKINNED_MESH, *ST_LPSKINNED_MESH;

	typedef std::map<std::string, ST_LPSKINNED_MESH>					MAP_SKINNED_MESH;
	typedef std::map<std::string, ST_LPSKINNED_MESH>::iterator		MAP_SKINNED_MESH_ITER;

	static MAP_SKINNED_MESH		s_SkinnedMeshes;

	class BaseSkinnedMesh : public cObject
	{
		friend class BONE_HIERARCHY;
		friend struct BONE;
		friend struct BONEMESH;

	private:
		LPDIRECT3DDEVICE9			m_pDevice;
		SYNTHESIZE(LPD3DXFRAME, m_pRootBone, RootBone);		//�ֻ��� �� ������ ( ���� "cBoneHierarchy.h" ������ BONE ����ü�� ����Ű�� �ȴ� )
		LPD3DXANIMATIONCONTROLLER	m_pAnimControl;				//�ִϸ��̼� ��Ʈ��
		SYNTHESIZE(LPD3DXEFFECT, m_pEffect, Effect);
		DWORD						m_dwWorkingPaletteSize;
		D3DXMATRIX *				m_amxWorkingPalette;

		SYNTHESIZE(DOUBLE, m_CurrentAnimPeriod, CurrentAnimPeriod);//���� ���õ� Animation �� ����
		//float						m_CurrentAnimPeriod;		
		D3DXMATRIX					m_mat;

		SYNTHESIZE(DOUBLE, m_dPassedTime, PassedTime);		//�ִϸ��̼� ���� Ÿ��
		SYNTHESIZE(bool, m_bPassed, Passed);					//�ִϸ��̼� �Ϸ� ����
		DOUBLE						m_dAniTime;					//�ִϸ��̼� ���� Ÿ��
		DOUBLE						m_dBlendingTime;			//�ִϸ��̼� ��ü ���� Ÿ��
		DOUBLE						m_dLeftChange;				//�ִϸ��̼� ü���� �� ���� ���� Ÿ��
		DOUBLE						m_dLeftBlendingWeight;		//���� ���� ����ġ


		//DWORD						m_dwCurrentTrack;

		SYNTHESIZE(UINT, m_AniIndex, AniIndex);


	public:
		BaseSkinnedMesh(void);
		~BaseSkinnedMesh(void);

		//�޽� �ε� �Լ�
		HRESULT		Init( LPDIRECT3DDEVICE9 pd3dDevice, const std::string& name, const D3DXMATRIX* pmat = NULL );

		//���� �Լ�
		void Release();

		//���� �Լ�
		void			Render( D3DXMATRIXA16& matWorld, DOUBLE deltaFrameTime );

		//���ϸ��̼� ���� �Լ�
		void			SetAnimationName(const std::string & actionName, bool bSetBegin = true );
		void			SetAnimationIndex(int index, bool bSetBegin = true );
		void			SetAnimationIndexBelnd(int index, bool bSetBegin = true, DOUBLE blendTime = 1.0f );

		//���ϸ��̼� �����Ӽ��� �Լ�
		void			SetFrame( DOUBLE frameTime );

		//������ �����Լ�
		void			PlayFrame( DOUBLE deltaFrameTime );

		//������ ����ӵ� ����
		void			SetPlaySpeed( DOUBLE speed );

		// ��Ʈ���� ����
		void			SetWorldMatrix( D3DXMATRIXA16& matWorld );

		//Map�� �ִ� �޽��� �� ���� ������.
		static void		SkinnenMeshesRelease(void);

		// ��带 ã�´�.
		LPD3DXFRAME GetFindBone( LPD3DXFRAME Node , char* NodeName );
		LPD3DXFRAME GetBone(){return m_pRootBone;}

	private:

		//���� ��Ʈ���� ������ ����
		//void		SetupBoneMatrixPointer( BONE* pBone );
		void		SetupBonePtrs( BONE * pBone );

		//��Ʈ���� ������Ʈ �Լ�
		void		UpdateMatrices( BONE* pBone, const D3DXMATRIX* parentMatrix );

		//��Ű�� ������Ʈ �Լ�
		void		UpdateSkinnedMesh( BONE* pBone );


	};

}

