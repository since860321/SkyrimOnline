#pragma once
#include "stdafx.h"


//fmod.hpp 추가
#include "lib/fmod.hpp"

//Lib 링크
#pragma comment (lib, "lib/fmodex_vc.lib" )

//추가 여분의 채널 갯수
#define EXTRA_SOUND_CHANNEL		5

// 총사운드 갯수 : 버퍼가작아서 씹히는 문제를 해결하기 위해 여분의 사운드 채널을 생성한다.
#define TOTAL_SOUND_CHANNEL SOUND_END + EXTRA_SOUND_CHANNEL

//FMOD 네임 스페이스 사용
using namespace FMOD;

/*---------------------------------------------------------------------------------------
	사운드 메니져 클레스
---------------------------------------------------------------------------------------*/
class cSoundMgr
{
private:
	cSoundMgr(void);
	~cSoundMgr(void);

public:
	//사운드 관리
	System*				m_pSystem;
	Sound**				m_pSound;
	Channel**			m_pChannel;

public:

	static	cSoundMgr*	GetInstance();
	
	//채널카운트 만큼의 채널을 생성해 놓는다.
	HRESULT		Init( UINT ChannelCount );

	//해제 
	void		Release(void);

	//파일명, enum 에서 정해준 사운드 이름, 반복 재생 여부
	void		SoundSetup( char* szFileName, int nSoundKind, bool bBackGround = false, bool bSoundLoop = false );

	//재생할 사운드 이름 ( enum 문에서 설정한 값 ), 볼륨 ( 0.0 ~ 1.0 )
	void		SoundPlay( int nSoundKind, float fVolume = 1.0f );

	//현재 재생중인 사운드를 일시 정지 시킨다.
	void		SoundPause( int nSoundKind );

	//일시정지된 사운드를 다시 재생한다.
	void		SoundResume( int nSoundKind );

	//일시정지되었는지 확인한다
	bool		isPausedSound( int nSoundKind );

	//현재 재생중인 사운드를 정지 시킨다.
	void		SoundStop( int nSoundKind );

	//현재 재생중인지 확인한다.
	bool		isPlaySound( int nSoundKind );

	//FMOD 시스템을 갱신한다.
	void		SoundUpdate(void);

	


};



/*===============================================================================================
	사운드 관련

	다운로드 사이트
	FMOD 사용법 ( 사운드 넣기 )

	http://www.fmod.org/fmod-downloads.html
	다운로드 주소

	링커 해주어야 할것
	fmodex_vc.lib

	포함헤더
	fmod.hpp


	Init() 부분 
	System_Create(&pSystem); 
	pSystem->init( 채널 수 , FMOD_INIT_NORMAL , 0); 
	pSystem->createSound("파일의 이름.mp3" , FMOD_HARDWARE | FMOD_LOOP_NORMAL , 0 , &pSound) 
	2번째 인자값 : FMOD_LOOP_NORMAL  // 노래 반복 여부 
	pSystem->playSound(FMOD_CHANNEL_FREE , pSound , 0 , &pChannel ); 
	3번째 인자값 : 0(flase) or 1(true) // 1 로 하면 소리가 안나옴 (왜 1로 쓰는지 모르겠다. system에는 일시정지 함수가 있기 때문에 무조건 0으로 해주자.) 

	update()부분 

	노래 나오는 부분에서 
	pChannel->update(); 
	중지 하고 싶은 부분에서. 
	pChannel->stop(); 

	//update 에 stop 을 넣어버리면 계속 중지되니까 노래가 안나옴니당. 

	관련 함수 
	pChannel->update(); 시작 
	pChannel->stop(); 정지 
	pChannel->setPaused(true); //일시정지 
	pChannel->setVolume(0.5); //사운드 볼륨 조절 0 ~ 1; 

	//소리 속도 바꾸기 
	float temp; 
	pChannel->getFrequency(&temp); 
	pChannel->setFrequency(temp * 0.5); temp 에 곱한 값에 따라 속도가 바뀐다 

	효과음 넣는 방법 

	Sound* 형으로 하나 더 만든다 ex) pEfSound; 
	Channel* 형으로 채널을 하나 더 만든다 ex)pEfChannel; 

	Init 부분에서 pSystem->createSound("이펙트 소리의 이름.map" , FMOD_HARDWARE  , 0 , &pEfSound); 

	소리를 사용하는 부분에서(충돌체크 라던가) 
	pSystem->playSound(FMOD_CHANNEL_FREE , pEfSound , false , &pEfChannel); 
	2번째 인자값 : pEfSound =  효과의 사운드 
	4번째 인자값 : &pEfChannel = 효과가 들어있는 채널의 주소값 


===============================================================================================*/

