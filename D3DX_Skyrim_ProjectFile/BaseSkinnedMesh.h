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
	struct BONE; //< 추가됨(형석)

	// 멀티애니메이션 메쉬 컨테이너
	typedef struct _ST_SKINNED_MESH
	{
		LPD3DXFRAME					pRootBone;		//최상위 본 프레임 ( 실제 "cBoneHierarchy.h" 정의한 BONE 구조체를 가리키게 된다 )
		LPD3DXANIMATIONCONTROLLER	pAnimControl;	//애니메이션 컨트롤
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
		SYNTHESIZE(LPD3DXFRAME, m_pRootBone, RootBone);		//최상위 본 프레임 ( 실제 "cBoneHierarchy.h" 정의한 BONE 구조체를 가리키게 된다 )
		LPD3DXANIMATIONCONTROLLER	m_pAnimControl;				//애니메이션 컨트롤
		SYNTHESIZE(LPD3DXEFFECT, m_pEffect, Effect);
		DWORD						m_dwWorkingPaletteSize;
		D3DXMATRIX *				m_amxWorkingPalette;

		SYNTHESIZE(DOUBLE, m_CurrentAnimPeriod, CurrentAnimPeriod);//현재 셋팅된 Animation 의 길이
		//float						m_CurrentAnimPeriod;		
		D3DXMATRIX					m_mat;

		SYNTHESIZE(DOUBLE, m_dPassedTime, PassedTime);		//애니메이션 지난 타임
		SYNTHESIZE(bool, m_bPassed, Passed);					//애니메이션 완료 여부
		DOUBLE						m_dAniTime;					//애니메이션 월드 타임
		DOUBLE						m_dBlendingTime;			//애니메이션 전체 블렌딩 타임
		DOUBLE						m_dLeftChange;				//애니메이션 체인지 시 남은 블렌딩 타임
		DOUBLE						m_dLeftBlendingWeight;		//남은 블렌딩 가중치


		//DWORD						m_dwCurrentTrack;

		SYNTHESIZE(UINT, m_AniIndex, AniIndex);


	public:
		BaseSkinnedMesh(void);
		~BaseSkinnedMesh(void);

		//메쉬 로드 함수
		HRESULT		Init( LPDIRECT3DDEVICE9 pd3dDevice, const std::string& name, const D3DXMATRIX* pmat = NULL );

		//해제 함수
		void Release();

		//랜더 함수
		void			Render( D3DXMATRIXA16& matWorld, DOUBLE deltaFrameTime );

		//에니메이션 셋팅 함수
		void			SetAnimationName(const std::string & actionName, bool bSetBegin = true );
		void			SetAnimationIndex(int index, bool bSetBegin = true );
		void			SetAnimationIndexBelnd(int index, bool bSetBegin = true, DOUBLE blendTime = 1.0f );

		//에니메이션 프레임셋팅 함수
		void			SetFrame( DOUBLE frameTime );

		//프레임 진행함수
		void			PlayFrame( DOUBLE deltaFrameTime );

		//프레임 진행속도 셋팅
		void			SetPlaySpeed( DOUBLE speed );

		// 매트릭스 셋팅
		void			SetWorldMatrix( D3DXMATRIXA16& matWorld );

		//Map에 있는 메쉬들 다 날려 제낀다.
		static void		SkinnenMeshesRelease(void);

		// 노드를 찾는다.
		LPD3DXFRAME GetFindBone( LPD3DXFRAME Node , char* NodeName );
		LPD3DXFRAME GetBone(){return m_pRootBone;}

	private:

		//본의 메트릭스 포인터 생성
		//void		SetupBoneMatrixPointer( BONE* pBone );
		void		SetupBonePtrs( BONE * pBone );

		//매트릭스 업데이트 함수
		void		UpdateMatrices( BONE* pBone, const D3DXMATRIX* parentMatrix );

		//스키닝 업데이트 함수
		void		UpdateSkinnedMesh( BONE* pBone );


	};

}

