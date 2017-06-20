#include "StdAfx.h"
#include "cEnemyCharacter.h"
#include "BaseSkinnedMesh.h"
#include "cNetworkMgr.h"
#include "cSoundMgr.h"

cEnemyCharacter::cEnemyCharacter(void)
{
}


cEnemyCharacter::~cEnemyCharacter(void)
{
}


void cEnemyCharacter::Control()
{

}

void cEnemyCharacter::Update()
{
	//이것이 바로 애니메이션 속도 조절해주는 함수입니다
	m_pSkinnedMesh->SetPlaySpeed(0.5f);

	if((DRAGON_ANIMATION)this->m_stEnemyData.nAniIndex != this->m_stEnemyData.NowAni)
	{
		// 모션에 따른 사운드 체인지
		switch(this->m_stEnemyData.NowAni) // 이전모션 사운드끄기
		{
		case D_mtidle_hover:
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_D_WING);
			}
			break;
		case D_mtfastforward_flap:
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_D_WING);
			}
			break;
		case D_mtforwardground:
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_D_WALK);
			}
			break;
		case D_ground_injured:
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_D_CRY);
			}
			break;
		case D_ground_bite:
			{
				cSoundMgr::GetInstance()->SoundStop(SOUND_D_CRY);
			}
			break;
		case D_ground_bleedoutidle:
			{
				if(!cSoundMgr::GetInstance()->isPlaySound(SOUND_BATTLE))
				{
					cSoundMgr::GetInstance()->SoundPlay(SOUND_BATTLE);
				}
				if(cSoundMgr::GetInstance()->isPlaySound(SOUND_NORMAL))
				{
					cSoundMgr::GetInstance()->SoundStop(SOUND_NORMAL);
				}
			}
			break;
		}

		
		
		switch(this->m_stEnemyData.nAniIndex) // 다음모션 켜기
		{
		case D_mtidle_hover:
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_D_WING);
			}
			break;
		case D_mtfastforward_flap:
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_D_WING);
			}
			break;
		case D_mtforwardground:
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_D_WALK);
			}
			break;
		case D_ground_injured:
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_D_CRY,0.3f);
			}
			break;
		case D_ground_bite:
			{
				cSoundMgr::GetInstance()->SoundPlay(SOUND_D_CRY,0.3f);
			}
			break;
		case D_ground_bleedoutidle:
			{
				if(cSoundMgr::GetInstance()->isPlaySound(SOUND_BATTLE))
				{
					cSoundMgr::GetInstance()->SoundStop(SOUND_BATTLE);
				}
				if(!cSoundMgr::GetInstance()->isPlaySound(SOUND_NORMAL))
				{
					cSoundMgr::GetInstance()->SoundPlay(SOUND_NORMAL);
				}
			}
			break;
		}
		this->m_stEnemyData.NowAni = (DRAGON_ANIMATION)this->m_stEnemyData.nAniIndex;
		this->m_pSkinnedMesh->SetAnimationIndex(this->m_stEnemyData.nAniIndex);
		
	
	}
	
	if(this->m_stCharacterInfo.bAlive == true)
	{
	
		D3DXVECTOR3 pos = this->m_stEnemyData.vTargetPos - this->m_TransformInfo.vPos;
		D3DXVec3Normalize(&pos,&pos);

		if( this->m_stEnemyData.nAniIndex	==(int)D_mtforwardground	||
			this->m_stEnemyData.nAniIndex	==(int)D_mtidle_hover	||
			this->m_stEnemyData.nAniIndex	==(int)D_mtfastforward_flap)
		{
		float dwTimeAssist =  (GetTickCount() - this->m_stEnemyData.dwUpdateTime) / (float)1000.0f; 
		this->m_TransformInfo.vPos += (pos*5)*dwTimeAssist;

		}

	D3DXVECTOR3			vPos2(1,0,0);
		
	float fRot = acosf(D3DXVec3Dot(&pos,&vPos2));

	if(pos.z > 0) fRot = (D3DX_PI*2) - fRot;

	fRot += (D3DX_PI/(float)2);

	float fchange = fRot - this->m_TransformInfo.vRot.y;
	
	// 목표 각도와의 차이가 1바퀴 이상일경우 1바퀴돌려서 보정한다

	while(fchange > D3DX_PI*2)
	{
		this->m_TransformInfo.vRot.y += D3DX_PI*2;
		fchange = fRot - this->m_TransformInfo.vRot.y;
	}

	while(fchange < -(D3DX_PI*2))
	{
		this->m_TransformInfo.vRot.y -= D3DX_PI*2;
		fchange = fRot - this->m_TransformInfo.vRot.y;
	}




	if(fchange * fchange > 0.0026) // 회전보정 좌우중 가까운곳으로 돈다.
	{
		 
		if(fRot>D3DX_PI)
		{
			if((this->m_TransformInfo.vRot.y < fRot)&&
				(this->m_TransformInfo.vRot.y > fRot - D3DX_PI))
			{
				this->m_TransformInfo.vRot.y+=this->m_stCharacterInfo.fRotSpeed;
			}
			else
			{
				this->m_TransformInfo.vRot.y-=this->m_stCharacterInfo.fRotSpeed;
			}
		}
		else
		{
			if((this->m_TransformInfo.vRot.y > fRot)&&
				(this->m_TransformInfo.vRot.y < fRot + D3DX_PI))
			{
				this->m_TransformInfo.vRot.y-=this->m_stCharacterInfo.fRotSpeed;
			}
			else
			{
				this->m_TransformInfo.vRot.y+=this->m_stCharacterInfo.fRotSpeed;
			}
		}
	}
	
	}
	else
	{
		if(this->m_TransformInfo.vPos.y  < 50)
		{
			cSoundMgr::GetInstance()->SoundPlay(SOUND_D_DEATH);
		}

		if(cSoundMgr::GetInstance()->isPlaySound(SOUND_BATTLE))
		{
			cSoundMgr::GetInstance()->SoundStop(SOUND_BATTLE);
		}
		if(!cSoundMgr::GetInstance()->isPlaySound(SOUND_NORMAL))
		{
			cSoundMgr::GetInstance()->SoundPlay(SOUND_NORMAL);
		}

		this->m_TransformInfo.vPos.y	+= 10;
		
	}

	Control();
}

void cEnemyCharacter::HPDamage(int nDamage)
{
	sgNetwork.SendToServer(this->m_stEnemyData.bMyIndex,nDamage);
}