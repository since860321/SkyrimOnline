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
			//�̰� ���� �¿�Ű�� ���� �ִϸ��̼� �ε��� ��ȣ �˾Ƴ��� �ֶ�
			static int nIndex = 0;
			if(wParam == VK_RIGHT)
			{
				if( nIndex < WEAPON_END-1)
				{
					nIndex++;
				}

				m_stCharacterInfo.m_UseWeapon = WEAPON_TYPE(nIndex);
				 				
				 				//char szBuff[128];
				 				//wsprintf(szBuff,"���� �ε��� %d",nIndex);
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
				 				//wsprintf(szBuff,"���� �ε��� %d",nIndex);
				 				//MessageBox(g_hWnd , szBuff ,0,0);
				 				//this->m_pSkinnedMesh->SetAnimationIndexBelnd(nIndex);
			}
			if(wParam == VK_UP)
			{
				m_pHand = m_pSkinnedMesh->GetFindBone(m_pSkinnedMesh->GetBone() , "WEAPON" );
				if( m_pHand == NULL)
				{
					MessageBox(g_hWnd , "������" ,  0 , 0 );
				}
			}

			return true;
		}

	}

	return false;
}

void cPlayerCharacter::KeyInput()
{

	/////->>>>>>>>>>>>>> ������ <<<<<<<<<<<<<<<<<- //////
	if(m_stCharacterInfo.bAlive == true)
	{
		Battle_Move();
	}
	

}


void cPlayerCharacter::Battle_Move()
{

	//���� ���� ���� ����, �� Y = 0;
	D3DXVECTOR3 vMoveDrt = m_stCharacterInfo.m_vView;
	vMoveDrt.y = 0;

	//�¿츦 �����ϴ� ũ�ν� ����
	D3DXVECTOR3 vCross = D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&vCross ,&vCross , &vMoveDrt);
	D3DXVec3Normalize(&vCross , &vCross);

	////////////////////////////////////////////////////////////////////////////////////
	static bool bAtk = false;
	static bool bAtkSound	=	false;
	static DWORD dwAtkTime = 0;
	PLAYER_ANIMATION CurrentAnimation;

	D3DXVECTOR3 vDrt = vMoveDrt;
	float fMoveSpeed = 0.0f; //< ������ �̵��ӵ��� �Ѱ��ֱ����� �������� ����( ���� )


	//���� ���°� �ƴҶ��� ����� ��ȭ �� �������� �ش�.
	if( bAtk == false )
	{
		bAtkSound = true;
		//������
		if( (GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (vMoveDrt - vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*1.2f);
			CurrentAnimation = weapon_runforwardleft;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*1.2f); //< (����)
		}
		//������
		else if( (GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (vMoveDrt + vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt *(m_stCharacterInfo.fMoveSpeed*1.2f);
			CurrentAnimation = weapon_runforwardright;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*1.2f); //< (����)
		}
		//���Ĺ�
		else if( (GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (-vMoveDrt - vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*0.8f);
			CurrentAnimation = weapon_runbackwardleft;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.8f); //< (����)
		}
		//���Ĺ�
		else if( (GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000) && (GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000)  )
		{
			D3DXVECTOR3 vDrt = (-vMoveDrt + vCross) *0.5f;
			m_TransformInfo.vPos +=  vDrt * (m_stCharacterInfo.fMoveSpeed*0.8f);
			CurrentAnimation = weapon_runbackwardright;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.8f); //< (����)
		}
		//��
		else if( GetAsyncKeyState(m_dwKey[KEY_FORWARD]) & 0x8000  )
		{
			m_TransformInfo.vPos +=  vMoveDrt  * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runforward;
			vDrt =  vMoveDrt;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (����)
		}
		//��
		else if( GetAsyncKeyState(m_dwKey[KEY_BACKWARD]) & 0x8000  )
		{
			m_TransformInfo.vPos -=  vMoveDrt  * (m_stCharacterInfo.fMoveSpeed*0.6f);
			CurrentAnimation = weapon_runbackward;	
			vDrt =  -vMoveDrt;

			fMoveSpeed = (m_stCharacterInfo.fMoveSpeed*0.6f); //< (����)
		}
		//��
		else if( GetAsyncKeyState(m_dwKey[KEY_LEFT]) & 0x8000  )
		{
			m_TransformInfo.vPos -= vCross * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runleft;
			vDrt =  -vCross;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (����)
		}
		//��
		else if( GetAsyncKeyState(m_dwKey[KEY_RIGHT]) & 0x8000  )
		{
			m_TransformInfo.vPos += vCross * m_stCharacterInfo.fMoveSpeed;
			CurrentAnimation = weapon_runright;
			vDrt =  vCross;

			fMoveSpeed = m_stCharacterInfo.fMoveSpeed; //< (����)
		}
		else
		{
			// �ƹ� �Էµ� ���� �ʾ�����
			CurrentAnimation  = weapon_idle;

			fMoveSpeed = 0.0f; //< (����)
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


		//�� ��ǰ� �� , ���� ����̿����� �Ѿ��.
		if( CurrentAnimation != this->m_stCharacterInfo.m_Animation )
		{
			//��������� �ٴ¸���̰� ������ �ٴ°Ծƴϸ� �ٴ»��带����

			if((this->m_stCharacterInfo.m_Animation >= weapon_runforwardright && this->m_stCharacterInfo.m_Animation <= weapon_runbackward)
				&&(CurrentAnimation < weapon_runforwardright || CurrentAnimation > weapon_runbackward))
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_C_RUN);
			}

			//������ �ٴ°� �ƴϰ� ��������� �ٴ¸���̸� �ٴ»��带 �Ҵ�
			if((CurrentAnimation >= weapon_runforwardright && CurrentAnimation <= weapon_runbackward)&&
				(this->m_stCharacterInfo.m_Animation < weapon_runforwardright || this->m_stCharacterInfo.m_Animation > weapon_runbackward))	
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_C_RUN);
			}

			this->m_stCharacterInfo.m_Animation = CurrentAnimation; 
			this->m_pSkinnedMesh->SetAnimationIndexBelnd(this->m_stCharacterInfo.m_Animation,true,0.4f);
			//this->m_pSkinnedMesh->SetAnimationIndexBelnd(this->m_stCharacterInfo.m_Animation,false); //< ���ιٲ�(����)
			//this->m_pSkinnedMesh->SetAnimationIndex(this->m_stCharacterInfo.m_Animation,false);
		}
	}
	else
	{
		//���� ����϶�, ���� ��� �ð��� ������ �ٽ� ���� ���¸� ���ش�.
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


		fMoveSpeed = 0.0f; //< (����)
	}

		
	

	sgNetwork.SendToServer( m_stCharacterInfo, m_TransformInfo, vDrt, fMoveSpeed, m_stCharacterInfo.m_Animation );  
}


void cPlayerCharacter::Update_CameraVector()
{
	D3DXMATRIXA16 matRot;

	GetMouseMoveValue();

	// �� ���� 
	D3DXVECTOR3 vStart = D3DXVECTOR3(0,0,1);

	//ĳ���� ȸ�� + ī�޶� Y�� ȸ��
	D3DXMatrixRotationY(&matRot, m_TransformInfo.vRot.y );
	D3DXVec3TransformCoord(&m_stCharacterInfo.m_vView, &vStart, &matRot);

	m_stCharacterInfo.m_vView.y = m_stCharacterInfo.m_fCamere_UpDownValue;

	// ���� ����
	m_stCharacterInfo.m_vEye = m_TransformInfo.vPos;

	// �� ����
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
	//�̰��� �ٷ� �ִϸ��̼� �ӵ� �������ִ� �Լ��Դϴ�
	m_pSkinnedMesh->SetPlaySpeed(0.45f);
	

	if(!m_stCharacterInfo.m_bMouseMove)
	{
		m_pHand = m_pSkinnedMesh->GetFindBone(m_pSkinnedMesh->GetBone() , "WEAPON" );
		if( m_pHand == NULL)
		{
			MessageBox(g_hWnd , "������" ,  0 , 0 );
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
		cDevice::GetInstance().SetWireFrameTexture( WF_RED ); //< ���̾�������
		sgDevice->SetMaterial( &(*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_Mtrl );
		sgDevice->SetStreamSource( 0, (*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_pVB, 0, sizeof(stObjVertex) );
		sgDevice->SetFVF( OBJ_D3DFVF );
		sgDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, (*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_Vertices.size()/3 );
	}

	//< �ٿ���ڽ� ���( ������ )	////////////////////////////////////////////////
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
	 			char szBuf[128] = " ���� Ÿ�� :  �巡�� ";
	 			RECT rc3 = { 300 , 50 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuf, strlen(szBuf), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	 
	 			int nHP = m_stCharacterInfo.m_pFocus->GetCharacterInfo()->dwHP;
	 
	 			///ü�� 
	 			char szBuff[128];
	 			wsprintf(szBuff,"ü�� %d",nHP);
	 			RECT rc2 = { 700 , 50 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuff, strlen(szBuff), &rc2, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	 		}


		}

	 			char szBuff[128];
				wsprintf(szBuff,"ü�� %d",m_stCharacterInfo.dwHP);
	 			RECT rc3 = { 700 , 150 , 0 ,0};
	 			cDevice::GetInstance().GetFont()->DrawText
	 				(NULL, szBuff, strlen(szBuff), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);*/

}

bool cPlayerCharacter::Atk_CollisionCheck()
{
	if( m_stCharacterInfo.m_pFocus != NULL )
	{
		//�� ���� Ÿ���� ���� �浹�߳�
		float rad2 = m_stCharacterInfo.dwRadius;
		float rad1 = m_stCharacterInfo.m_pFocus->GetCharacterInfo()->dwRadius;
		D3DXVECTOR3 vDistance = ( m_TransformInfo.vPos  - m_stCharacterInfo.m_pFocus->GetTransformInfo()->vPos  );

		//���� ���� �Ÿ�
		float fDistance = D3DXVec3Length(&vDistance);

		// ���� ���� �Ÿ��� �������� �պ��� �۾������ �浹 
		if( fDistance < rad1 + rad2 )
		{
			return true;
		}


	}

	return false;
}

