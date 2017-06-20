#include "StdAfx.h"
#include "cNetworkMgr.h"
#include "cPlayerCharacter.h"
#include "BaseSkinnedMesh.h"
#include "cCamera.h"
#include "cObjFileMesh.h"
#include "cGameTime.h"
#include "BaseBoneHierachy.h"
#include "cEnemyCharacter.h"
#include "cSoundMgr.h"

cPlayerCharacter::cPlayerCharacter(void)
{
	D3DXMatrixIdentity(&m_matWeaponTM);
	m_pHand = NULL;

	// 	m_dwKey[KEY_FORWARD]	 = 'T';
	// 	m_dwKey[KEY_BACKWARD]	 = 'G';
	// 	m_dwKey[KEY_LEFT]		 = 'F';
	// 	m_dwKey[KEY_RIGHT]		 = 'H';

	m_dwKey[KEY_FORWARD]	 = 'W';
	m_dwKey[KEY_BACKWARD]	 = 'S';
	m_dwKey[KEY_LEFT]		 = 'A';
	m_dwKey[KEY_RIGHT]		 = 'D';

}


cPlayerCharacter::~cPlayerCharacter(void)
{
}

void cPlayerCharacter::Update()
{

	Update_CameraVector();

	if((int)m_stCharacterInfo.dwHP <= 0 &&  m_stCharacterInfo.dwDeadTime == 0 )
	{
	m_stCharacterInfo.dwHP= 0;
	m_stCharacterInfo.dwDeadTime = GetTickCount();
	}

	if(!m_stCharacterInfo.m_bMouseMove)
	{
		KeyInput();
	}

	if( Atk_CollisionCheck() )
	{
		m_stCharacterInfo.m_bAtk= true;
	}
	else
	{
		m_stCharacterInfo.m_bAtk = false;
	}



	if(m_stCharacterInfo.dwDeadTime != 0&& m_stCharacterInfo.dwDeadTime + m_pSkinnedMesh->GetCurrentAnimPeriod() * 1000 <= GetTickCount())
	{
		m_stCharacterInfo.bAlive= false;
		this->m_pSkinnedMesh->SetAnimationIndexBelnd(1,true,0.4f);
		cSoundMgr::GetInstance()->SoundPlay(SOUND_C_DEATH);
	}

}

bool cPlayerCharacter::WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{

	switch(iMessage)
	{
	case WM_KEYDOWN:
		{
			//이거 쓰면 좌우키로 현제 애니메이션 인덱스 번호 알아낼수 있뜸
			static int nIndex = 0;
			if(wParam == VK_RIGHT)
			{
				if( nIndex < WEAPON_END-1)
				{
					nIndex++;
				}

				m_stCharacterInfo.m_UseWeapon = WEAPON_TYPE(nIndex);
				 				
				 				//char szBuff[128];
				 				//wsprintf(szBuff,"현제 인덱스 %d",nIndex);
				 				//MessageBox(g_hWnd , szBuff ,0,0);
				 				//this->m_pSkinnedMesh->SetAnimationIndexBelnd(nIndex);
			}
			if(wParam == VK_LEFT)
			{
				if( nIndex > 0)
				{
					nIndex--;
				}
				m_stCharacterInfo.m_UseWeapon = WEAPON_TYPE(nIndex);

				 				//
				 				//char szBuff[128];
				 				//wsprintf(szBuff,"현제 인덱스 %d",nIndex);
				 				//MessageBox(g_hWnd , szBuff ,0,0);
				 				//this->m_pSkinnedMesh->SetAnimationIndexBelnd(nIndex);
			}
			if(wParam == VK_UP)
			{
				m_pHand = m_pSkinnedMesh->GetFindBone(m_pSkinnedMesh->GetBone() , "WEAPON" );
				if( m_pHand == NULL)
				{
					MessageBox(g_hWnd , "못차즘" ,  0 , 0 );
				}
			}

			return true;
		}

	}

	return false;
}

void cPlayerCharacter::KeyInput()
{

	/////->>>>>>>>>>>>>> 움직임 <<<<<<<<<<<<<<<<<- //////
	if(m_stCharacterInfo.bAlive == true)
	{
		Battle_Move();
	}
	

}


void cPlayerCharacter::Battle_Move()
{

	//현제 보는 방향 벡터, 단 Y = 0;
	D3DXVECTOR3 vMoveDrt = m_stCharacterInfo.m_vView;
	vMoveDrt.y = 0;

	//좌우를 결정하는 크로스 벡터
	D3DXVECTOR3 vCross = D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&vCross ,&vCross , &vMoveDrt);
	D3DXVec3Normalize(&vCross , &vCross);

	////////////////////////////////////////////////////////////////////////////////////
	static bool bAtk = false;
	static bool bAtkSound	=	false;
	static DWORD dwAtkTime = 0;
	PLAYER_ANIMATION CurrentAnimation;

	D3DXVECTOR3 vDrt = vMoveDrt;
	float fMoveSpeed = 0.0f; //< 서버에 이동속도를 넘겨주기위한 지역변수 선언( 형석 )


	//공격 상태가 아닐때만 모션의 변화 및 움직임을 준다.
	if( bAtk == false )
	{
		bAtkSound = true;
		//좌전방
		if( (GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (vMoveDrt - vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*1.2f);
			CurrentAnimation = weapon_runforwardleft;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*1.2f); //< (형석)
		}
		//우전방
		else if( (GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (vMoveDrt + vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt *(m_stCharacterInfo.fMoveSpeed*1.2f);
			CurrentAnimation = weapon_runforwardright;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*1.2f); //< (형석)
		}
		//좌후방
		else if( (GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (-vMoveDrt - vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*0.8f);
			CurrentAnimation = weapon_runbackwardleft;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.8f); //< (형석)
		}
		//우후방
		else if( (GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (-vMoveDrt + vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*0.8f);
			CurrentAnimation = weapon_runbackwardright;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.8f); //< (형석)
		}
		//전
		else if( GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000  )
		{
			m_TransformInfo.vPos +=  vMoveDrt  * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runforward;
			vDrt =  vMoveDrt;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (형석)
		}
		//후
		else if( GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000  )
		{
			m_TransformInfo.vPos -=  vMoveDrt  * (m_stCharacterInfo.fMoveSpeed*0.6f);
			CurrentAnimation = weapon_runbackward;	
			vDrt =  -vMoveDrt;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.6f); //< (형석)
		}
		//좌
		else if( GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000  )
		{
			m_TransformInfo.vPos -= vCross * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runleft;
			vDrt =  -vCross;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (형석)
		}
		//우
		else if( GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000  )
		{
			m_TransformInfo.vPos += vCross * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runright;
			vDrt =  vCross;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (형석)
		}
		else
		{
			// 아무 입력도 오지 않았을때
			CurrentAnimation  = weapon_idle;

			fMoveSpeed = 0.0f; //< (형석)
		}


		if( (GetAsyncKeyState( MK_LBUTTON ) & 0x8000) && (int)m_stCharacterInfo.dwHP >0)
		{
						bAtk = true;
					
					

			dwAtkTime = GetTickCount();

			if(CurrentAnimation == weapon_idle )				CurrentAnimation = weapon_attackright;
			if(CurrentAnimation == weapon_runforward )			CurrentAnimation = weapon_attackpower;
			if(CurrentAnimation == weapon_runbackward )			CurrentAnimation = weapon_attackpowerbwd;
			if(CurrentAnimation == weapon_runforwardleft )		CurrentAnimation = weapon_attackleft;
			if(CurrentAnimation == weapon_runforwardright )		CurrentAnimation = weapon_attackright;
			if(CurrentAnimation == weapon_runleft )				CurrentAnimation = weapon_attackpowerleft;
			if(CurrentAnimation == weapon_runright )			CurrentAnimation = weapon_attackpowerright;
			if(CurrentAnimation == weapon_runbackwardleft )		CurrentAnimation = weapon_attackleft;
			if(CurrentAnimation == weapon_runbackwardright )	CurrentAnimation = weapon_attackright;


			if( Atk_CollisionCheck() )
			{
				((cEnemyCharacter*)m_stCharacterInfo.m_pFocus)->HPDamage(m_stCharacterInfo.dwAttackDamage);


			}

		}
			if((int)m_stCharacterInfo.dwHP <= 0) CurrentAnimation = weapon_Dead;


		//뒷 모션과 비교 , 같은 모션이였으면 넘어간다.
		if( CurrentAnimation != this->m_stCharacterInfo.m_Animation )
		{
			//이전모션이 뛰는모션이고 다음이 뛰는게아니면 뛰는사운드를끈다

			if((this->m_stCharacterInfo.m_Animation >= weapon_runforwardright && this->m_stCharacterInfo.m_Animation <= weapon_runbackward)
				&&(CurrentAnimation < weapon_runforwardright || CurrentAnimation > weapon_runbackward))
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_C_RUN);
			}

			//이전이 뛰는게 아니고 다음모션이 뛰는모션이면 뛰는사운드를 켠다
			if((CurrentAnimation >= weapon_runforwardright && CurrentAnimation <= weapon_runbackward)&&
				(this->m_stCharacterInfo.m_Animation < weapon_runforwardright || this->m_stCharacterInfo.m_Animation > weapon_runbackward))	
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_C_RUN);
			}

			this->m_stCharacterInfo.m_Animation = CurrentAnimation; 
			this->m_pSkinnedMesh->SetAnimationIndexBelnd(this->m_stCharacterInfo.m_Animation,true,0.4f);
			//this->m_pSkinnedMesh->SetAnimationIndexBelnd(this->m_stCharacterInfo.m_Animation,false); //< 새로바뀜(형석)
			//this->m_pSkinnedMesh->SetAnimationIndex(this->m_stCharacterInfo.m_Animation,false);
		}
	}
	else
	{
		//공격 모션일땐, 현제 모션 시간이 지나면 다시 공격 상태를 꺼준다.
		if( GetTickCount() >= dwAtkTime + (m_pSkinnedMesh->GetCurrentAnimPeriod() * 1000 ) )
		{
			bAtk = false;

		}
		if( GetTickCount() >= dwAtkTime + (m_pSkinnedMesh->GetCurrentAnimPeriod() * 300 ) )
		{
			if(bAtkSound)
			{
				if( Atk_CollisionCheck() )
				{
							switch(m_stCharacterInfo.m_UseWeapon)
				{
				case WEAPON_TWOHAND_STEEL:
				case WEAPON_TWOHAND:
				case WEAPON_BATTLE_AXE:
				case WEAPON_EX_AXE:
					{
						cSoundMgr::GetInstance()->SoundPlay(SOUND_C_S_ATTACK);
					}
					break;
	
				case WEAPON_HAMMER:
					{
						cSoundMgr::GetInstance()->SoundPlay(SOUND_C_C_ATTACK);
					}
					break;
				}
				}

				switch(m_stCharacterInfo.m_UseWeapon)
				{
				case WEAPON_TWOHAND:
				case WEAPON_TWOHAND_STEEL:
				{
					cSoundMgr::GetInstance()->SoundPlay(SOUND_C_S_SWING);
				}
				break;
				
				case WEAPON_BATTLE_AXE:					
				case WEAPON_EX_AXE:
				case WEAPON_HAMMER:
				{
					cSoundMgr::GetInstance()->SoundPlay(SOUND_C_C_SWING);
				}
				break;
			}



				bAtkSound = false;
			}

		}


		fMoveSpeed = 0.0f; //< (형석)
	}

		
	

	sgNetwork.SendToServer( m_stCharacterInfo, m_TransformInfo, vDrt, fMoveSpeed, m_stCharacterInfo.m_Animation );  
}


void cPlayerCharacter::Update_CameraVector()
{
	D3DXMATRIXA16 matRot;

	GetMouseMoveValue();

	// 뷰 벡터 
	D3DXVECTOR3 vStart = D3DXVECTOR3(0,0,1);

	//캐릭터 회전 + 카메라 Y축 회전
	D3DXMatrixRotationY(&matRot, m_TransformInfo.vRot.y );
	D3DXVec3TransformCoord(&m_stCharacterInfo.m_vView, &vStart, &matRot);

	m_stCharacterInfo.m_vView.y = m_stCharacterInfo.m_fCamere_UpDownValue;

	// 아이 벡터
	m_stCharacterInfo.m_vEye = m_TransformInfo.vPos;

	// 룩 벡터
	m_stCharacterInfo.m_vLook  = m_stCharacterInfo.m_vEye + m_stCharacterInfo.m_vView;	
}

void cPlayerCharacter::GetMouseMoveValue()
{
	if(!m_stCharacterInfo.m_bMouseMove)
	{
		POINT pt,m_Pt;

		m_Pt.x = WINSIZEX /2;
		m_Pt.y = WINSIZEY /2;
		ClientToScreen( g_hWnd, &m_Pt );

		GetCursorPos(&pt);

		float fX = -(m_Pt.x - pt.x) * CAMERA_SPEED_ROTATION; 
		float fY = -(m_Pt.y - pt.y) * CAMERA_SPEED_ROTATION; 

		m_TransformInfo.vRot.y += fX;
		m_stCharacterInfo.m_fCamere_UpDownValue  -= fY;


		if( m_stCharacterInfo.m_fCamere_UpDownValue < 3 )
		{
			m_stCharacterInfo.m_fCamere_UpDownValue = 3;
		}
		if( m_stCharacterInfo.m_fCamere_UpDownValue > 5.1f )
		{
			m_stCharacterInfo.m_fCamere_UpDownValue = 5.1f;
		}

		SetCursorPos(m_Pt.x, m_Pt.y);
	}

}

void cPlayerCharacter::Render( void )
{
	//이것이 바로 애니메이션 속도 조절해주는 함수입니다
	m_pSkinnedMesh->SetPlaySpeed(0.45f);
	

	if(!m_stCharacterInfo.m_bMouseMove)
	{
		m_pHand = m_pSkinnedMesh->GetFindBone(m_pSkinnedMesh->GetBone() , "WEAPON" );
		if( m_pHand == NULL)
		{
			MessageBox(g_hWnd , "못차즘" ,  0 , 0 );
		}

		BaseD3D::BONE* b = (BaseD3D::BONE*)m_pHand;

		//////////////////////////////////////////////////////////////////////////////////////
		this->SetWorldTM();
		assert( m_pSkinnedMesh != NULL && "Message : Not Model"  );
		sgDevice->SetTransform(D3DTS_WORLD , &m_WolrdTM);
		m_pSkinnedMesh->Render( m_WolrdTM, cGameTime::Instance().Get_ElapsedTime()/ 1000.0f);
		//(m_pSkinnedMesh->GetCurrentAnimPeriod()*1000)
		m_matWeaponTM =   b->TransformationMatrix ;

		sgDevice->SetTransform(D3DTS_WORLD , &m_matWeaponTM);
		sgDevice->SetTexture( 0, (*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_pTex);
		cDevice::GetInstance().SetWireFrameTexture( WF_RED ); //< 와이어프레임
		sgDevice->SetMaterial( &(*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_Mtrl );
		sgDevice->SetStreamSource( 0, (*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_pVB, 0, sizeof(stObjVertex) );
		sgDevice->SetFVF( OBJ_D3DFVF );
		sgDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, (*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_Vertices.size()/3 );
	}

	//< 바운딩박스 출력( 디버깅용 )	////////////////////////////////////////////////
	//  	LPD3DXMESH			m_BoundingBoxMesh;
	//  	sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//  	
	//  	D3DXCreateSphere(sgDevice , m_stCharacterInfo.dwRadius , 30,30, &m_BoundingBoxMesh, NULL );
	// 	
	// 	D3DXMATRIXA16 matTemp;
	// 	D3DXMatrixTranslation(&matTemp,m_TransformInfo.vPos.x , m_TransformInfo.vPos.y , m_TransformInfo.vPos.z);
	// 
	//  	sgDevice->SetTransform(D3DTS_WORLD , &matTemp);
	//  
	//  	m_BoundingBoxMesh->DrawSubset( 0 );
	//  	sgDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	/////////////////////////////////////////////////////////////////////////////////////

	/* 	if( m_stCharacterInfo.m_pFocus != NULL)
	 	{
	 		if( m_stCharacterInfo.m_pFocus->GetCharacterInfo()->CharacterType == CT_MONSTER_DRAGON)
	 		{
	 			char szBuf[128] = " 현제 타겟 :  드래곤 ";
	 			RECT rc3 = { 300 , 50 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuf, strlen(szBuf), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	 
	 			int nHP = m_stCharacterInfo.m_pFocus->GetCharacterInfo()->dwHP;
	 
	 			///체력 
	 			char szBuff[128];
	 			wsprintf(szBuff,"체력 %d",nHP);
	 			RECT rc2 = { 700 , 50 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuff, strlen(szBuff), &rc2, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	 		}


		}

	 			char szBuff[128];
				wsprintf(szBuff,"체력 %d",m_stCharacterInfo.dwHP);
	 			RECT rc3 = { 700 , 150 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuff, strlen(szBuff), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);*/

}

bool cPlayerCharacter::Atk_CollisionCheck()
{
	if( m_stCharacterInfo.m_pFocus != NULL )
	{
		//내 원과 타겟의 원이 충돌했냐
		float rad2 = m_stCharacterInfo.dwRadius;
		float rad1 = m_stCharacterInfo.m_pFocus->GetCharacterInfo()->dwRadius;
		D3DXVECTOR3 vDistance = ( m_TransformInfo.vPos  - m_stCharacterInfo.m_pFocus->GetTransformInfo()->vPos  );

		//두점 사이 거리
		float fDistance = D3DXVec3Length(&vDistance);

		// 두점 사이 거리가 반지름의 합보다 작아질경우 충돌 
		if( fDistance < rad1 + rad2 )
		{
			return true;
		}


	}

	return false;
}

