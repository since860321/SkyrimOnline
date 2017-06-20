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

//ä��ī��Ʈ ��ŭ�� ä���� ������ ���´�.
HRESULT cSoundMgr::Init( UINT ChannelCount )
{
	//���� System ����
	System_Create( &m_pSystem );

	//ä�μ� 
	m_pSystem->init( ChannelCount, FMOD_INIT_NORMAL, 0 );

	//ä�μ��� ���� ������ ���� �ִ� ���� ����
	//ä�μ� ��ŭ �޸� ���� �� ���� �����ϱ�
	m_pSound = new Sound*[ChannelCount];
	m_pChannel = new Channel*[ChannelCount];

	//�ʱ�ȭ
	memset( m_pSound, 0, sizeof( Sound* ) * ( ChannelCount ) );
	memset( m_pChannel, 0, sizeof( Channel* ) * ( ChannelCount ) );


	return S_OK;
}

//FMOD �ý��� ���� 
void cSoundMgr::Release(void)
{
	//���� ����
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

	//�޸� �����
	SAFE_DELETE_ARRAY( m_pChannel );
	SAFE_DELETE_ARRAY( m_pSound );


	//�ý��� �ݱ� 
	if( m_pSystem != NULL )
	{
		m_pSystem->release();
		m_pSystem->close();
	}
}


//���ϸ�, enum ���� ������ ���� �̸�, �ݺ� ��� ����
void cSoundMgr::SoundSetup( char* szFileName, int nSoundKind, bool bBackGround, bool bSoundLoop )
{
	//���� �Ǵ� ���� �ΰ�?
	if( bSoundLoop )
	{
		//����� �ΰ�?
		if( bBackGround )
		{
			//��� �����϶� ( createStream ���� ���� ) 
			//�ߺ� ������ �����ϰ� ��� �÷��� �ؾ� �ϱ� ������ ��Ʈ������.....
			m_pSystem->createStream( szFileName, FMOD_LOOP_NORMAL, 0, &m_pSound[nSoundKind] );
		}

		//������� �ƴѰ�?
		else
		{
			//�Ϲ� ���� �϶� 
			m_pSystem->createSound( szFileName, FMOD_LOOP_NORMAL, 0, &m_pSound[nSoundKind] );
		}
	}

	//���� �Ǵ� ���尡 �ƴѰ�?
	else
	{
		//�ݺ� ���� �� �ѹ� �÷��̴�.
		m_pSystem->createSound( szFileName, FMOD_DEFAULT, 0, &m_pSound[nSoundKind] );
	}

}

//����� ���� �̸� ( enum ������ ������ �� ), ���� ( 0.0 ~ 1.0 )
void cSoundMgr::SoundPlay( int nSoundKind, float fVolume )
{
	
	//���� �÷��� 
	m_pSystem->playSound( FMOD_CHANNEL_FREE, m_pSound[nSoundKind], false, &m_pChannel[nSoundKind] );

	//���� ����
	m_pChannel[ nSoundKind ]->setVolume( fVolume );
}
 
//���� ������� ���带 �Ͻ� ���� ��Ų��.
void cSoundMgr::SoundPause( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->setPaused( true );
}

//�Ͻ������� ���带 �ٽ� ����Ѵ�.
void cSoundMgr::SoundResume( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->setPaused( false );
}

//���� ������� ���带 ���� ��Ų��.
void cSoundMgr::SoundStop( int nSoundKind )
{
	m_pChannel[ nSoundKind ]->stop();
}

//�Ͻ������Ǿ����� Ȯ���Ѵ�
bool cSoundMgr::isPausedSound( int nSoundKind )
{
	bool isPause;
	m_pChannel[ nSoundKind ]->getPaused( &isPause );
	return isPause;
}

//���� ��������� Ȯ���Ѵ�.
bool cSoundMgr::isPlaySound( int nSoundKind )
{
	bool isPlay;
	m_pChannel[ nSoundKind ]->isPlaying( &isPlay );
	return isPlay;
}

//FMOD �ý����� �����Ѵ�.
void cSoundMgr::SoundUpdate(void)
{
	//���� System ��������� ������Ʈ
	m_pSystem->update();

	//������ �ٲ�ٰų�
	//����� ���� ���带 ä�ο��� ���µ��� �پ��� �۾��� �ڵ����� ���ش�!
}

