#pragma once
#include "stdafx.h"


//fmod.hpp �߰�
#include "lib/fmod.hpp"

//Lib ��ũ
#pragma comment (lib, "lib/fmodex_vc.lib" )

//�߰� ������ ä�� ����
#define EXTRA_SOUND_CHANNEL		5

// �ѻ��� ���� : ���۰��۾Ƽ� ������ ������ �ذ��ϱ� ���� ������ ���� ä���� �����Ѵ�.
#define TOTAL_SOUND_CHANNEL SOUND_END + EXTRA_SOUND_CHANNEL

//FMOD ���� �����̽� ���
using namespace FMOD;

/*---------------------------------------------------------------------------------------
	���� �޴��� Ŭ����
---------------------------------------------------------------------------------------*/
class cSoundMgr
{
private:
	cSoundMgr(void);
	~cSoundMgr(void);

public:
	//���� ����
	System*				m_pSystem;
	Sound**				m_pSound;
	Channel**			m_pChannel;

public:

	static	cSoundMgr*	GetInstance();
	
	//ä��ī��Ʈ ��ŭ�� ä���� ������ ���´�.
	HRESULT		Init( UINT ChannelCount );

	//���� 
	void		Release(void);

	//���ϸ�, enum ���� ������ ���� �̸�, �ݺ� ��� ����
	void		SoundSetup( char* szFileName, int nSoundKind, bool bBackGround = false, bool bSoundLoop = false );

	//����� ���� �̸� ( enum ������ ������ �� ), ���� ( 0.0 ~ 1.0 )
	void		SoundPlay( int nSoundKind, float fVolume = 1.0f );

	//���� ������� ���带 �Ͻ� ���� ��Ų��.
	void		SoundPause( int nSoundKind );

	//�Ͻ������� ���带 �ٽ� ����Ѵ�.
	void		SoundResume( int nSoundKind );

	//�Ͻ������Ǿ����� Ȯ���Ѵ�
	bool		isPausedSound( int nSoundKind );

	//���� ������� ���带 ���� ��Ų��.
	void		SoundStop( int nSoundKind );

	//���� ��������� Ȯ���Ѵ�.
	bool		isPlaySound( int nSoundKind );

	//FMOD �ý����� �����Ѵ�.
	void		SoundUpdate(void);

	


};



/*===============================================================================================
	���� ����

	�ٿ�ε� ����Ʈ
	FMOD ���� ( ���� �ֱ� )

	http://www.fmod.org/fmod-downloads.html
	�ٿ�ε� �ּ�

	��Ŀ ���־�� �Ұ�
	fmodex_vc.lib

	�������
	fmod.hpp


	Init() �κ� 
	System_Create(&pSystem); 
	pSystem->init( ä�� �� , FMOD_INIT_NORMAL , 0); 
	pSystem->createSound("������ �̸�.mp3" , FMOD_HARDWARE | FMOD_LOOP_NORMAL , 0 , &pSound) 
	2��° ���ڰ� : FMOD_LOOP_NORMAL  // �뷡 �ݺ� ���� 
	pSystem->playSound(FMOD_CHANNEL_FREE , pSound , 0 , &pChannel ); 
	3��° ���ڰ� : 0(flase) or 1(true) // 1 �� �ϸ� �Ҹ��� �ȳ��� (�� 1�� ������ �𸣰ڴ�. system���� �Ͻ����� �Լ��� �ֱ� ������ ������ 0���� ������.) 

	update()�κ� 

	�뷡 ������ �κп��� 
	pChannel->update(); 
	���� �ϰ� ���� �κп���. 
	pChannel->stop(); 

	//update �� stop �� �־������ ��� �����Ǵϱ� �뷡�� �ȳ��ȴϴ�. 

	���� �Լ� 
	pChannel->update(); ���� 
	pChannel->stop(); ���� 
	pChannel->setPaused(true); //�Ͻ����� 
	pChannel->setVolume(0.5); //���� ���� ���� 0 ~ 1; 

	//�Ҹ� �ӵ� �ٲٱ� 
	float temp; 
	pChannel->getFrequency(&temp); 
	pChannel->setFrequency(temp * 0.5); temp �� ���� ���� ���� �ӵ��� �ٲ�� 

	ȿ���� �ִ� ��� 

	Sound* ������ �ϳ� �� ����� ex) pEfSound; 
	Channel* ������ ä���� �ϳ� �� ����� ex)pEfChannel; 

	Init �κп��� pSystem->createSound("����Ʈ �Ҹ��� �̸�.map" , FMOD_HARDWARE  , 0 , &pEfSound); 

	�Ҹ��� ����ϴ� �κп���(�浹üũ �����) 
	pSystem->playSound(FMOD_CHANNEL_FREE , pEfSound , false , &pEfChannel); 
	2��° ���ڰ� : pEfSound =  ȿ���� ���� 
	4��° ���ڰ� : &pEfChannel = ȿ���� ����ִ� ä���� �ּҰ� 


===============================================================================================*/

