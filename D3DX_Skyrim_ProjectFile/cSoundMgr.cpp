#include "stdafx.h"
#include "cSoundMgr.h"



cSoundMgr::cSoundMgr(void)
	: m_pSystem( NULL ), m_pChannel( NULL ), m_pSound( NULL )
{
}


cSoundMgr::~cSoundMgr(void)
{
}

cSoundMgr*	cSoundMgr::GetInstance()
{
	static cSoundMgr sound;
	return &sound;
}

//채널카운트 만큼의 채널을 생성해 놓는다.
HRESULT cSoundMgr::Init( UINT ChannelCount )
{
	//사운드 System 생성
	System_Create( &m_pSystem );

	//채널수 
	m_pSystem->init( ChannelCount, FMOD_INIT_NORMAL, 0 );

	//채널수와 사운드 수량은 맞춰 주는 것이 좋다
	//채널수 만큼 메모리 버퍼 및 사운드 생성하기
	m_pSound = new Sound*[ChannelCount];
	m_pChannel = new Channel*[ChannelCount];

	//초기화
	memset( m_pSound, 0, sizeof( Sound* ) * ( ChannelCount ) );
	memset( m_pChannel, 0, sizeof( Channel* ) * ( ChannelCount ) );


	return S_OK;
}

//FMOD 시스템 해제 
void cSoundMgr::Release(void)
{
	//사운드 삭제
	if( m_pChannel != NULL || m_pSound != NULL )
	{
		for( int i = 0 ; i < TOTAL_SOUND_CHANNEL ; i++ )
		{
			if( m_pChannel != NULL )
			{
				if( m_pChannel[i] )
					m_pChannel[i]->stop();
			}

			if( m_pSound != NULL )
			{
				if( m_pSound != NULL )
					m_pSound[i]->release();
			}
		}
	}

	//메모리 지우기
	SAFE_DELETE_ARRAY( m_pChannel );
	SAFE_DELETE_ARRAY( m_pSound );


	//시스템 닫기 
	if( m_pSystem != NULL )
	{
		m_pSystem->release();
		m_pSystem->close();
	}
}


//파일명, enum 에서 정해준 사운드 이름, 반복 재생 여부
void cSoundMgr::SoundSetup( char* szFileName, int nSoundKind, bool bBackGround, bool bSoundLoop )
{
	//루프 되는 사운드 인가?
	if( bSoundLoop )
	{
		//배경음 인가?
		if( bBackGround )
		{
			//배경 음악일때 ( createStream 으로 생성 ) 
			//중복 방지도 가능하고 계속 플레이 해야 하기 때문에 스트림으로.....
			m_pSystem->createStream( szFileName, FMOD_LOOP_NORMAL, 0, &m_pSound[nSoundKind] );
		}

		//배경음이 아닌가?
		else
		{
			//일반 사운드 일때 
			m_pSystem->createSound( szFileName, FMOD_LOOP_NORMAL, 0, &m_pSound[nSoundKind] );
		}
	}

	//루프 되는 사운드가 아닌가?
	else
	{
		//반복 없는 단 한번 플레이다.
		m_pSystem->createSound( szFileName, FMOD_DEFAULT, 0, &m_pSound[nSoundKind] );
	}

}

//재생할 사운드 이름 ( enum 문에서 설정한 값 ), 볼륨 ( 0.0 ~ 1.0 )
void cSoundMgr::SoundPlay( int nSoundKind, float fVolume )
{
	
	//사운드 플레이 
	m_pSystem->playSound( FMOD_CHANNEL_FREE, m_pSound[nSoundKind], false, &m_pChannel[nSoundKind] );

	//볼륨 설정
	m_pChannel[ nSoundKind ]->setVolume( fVolume );
}
 
//현재 재생중인 사운드를 일시 정지 시킨다.
void cSoundMgr::SoundPause( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->setPaused( true );
}

//일시정지된 사운드를 다시 재생한다.
void cSoundMgr::SoundResume( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->setPaused( false );
}

//현재 재생중인 사운드를 정지 시킨다.
void cSoundMgr::SoundStop( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->stop();
}

//일시정지되었는지 확인한다
bool cSoundMgr::isPausedSound( int nSoundKind )
{
	bool isPause;
	m_pChannel[ nSoundKind ]->getPaused( &isPause );
	return isPause;
}

//현재 재생중인지 확인한다.
bool cSoundMgr::isPlaySound( int nSoundKind )
{
	bool isPlay;
	m_pChannel[ nSoundKind ]->isPlaying( &isPlay );
	return isPlay;
}

//FMOD 시스템을 갱신한다.
void cSoundMgr::SoundUpdate(void)
{
	//사운드 System 계속적으로 업데이트
	m_pSystem->update();

	//볼륨이 바뀐다거나
	//재생이 끝난 사운드를 채널에서 빼는등의 다양한 작업을 자동으로 해준다!
}

