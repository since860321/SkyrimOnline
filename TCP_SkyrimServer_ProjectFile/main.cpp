// TCP_SkyrimServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "defines.h"
#include "cWinSocketServer.h"
#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
int _tmain(int argc, _TCHAR* argv[])
{
	sgNetwork.CreateListenSocket();
	sgNetwork.Bind();
	sgNetwork.Listen();
	sgNetwork.AcceptClient();
	sgNetwork.CloseSocket();

	return 0;
}
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#include "cAI.h"

#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
CRITICAL_SECTION cs;
#endif _S_LINUX_EPOLL_

void CreateAIThread( void );
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
DWORD WINAPI ProcessAI( LPVOID arg ); //< AI 쓰레드
#endif //_S_LINUX_EPOLL_
#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else 
int _tmain(int argc, char* argv[])
#endif
{
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
	InitializeCriticalSection(&cs);	
	sgNetwork.CreateListenSocket();
	sgNetwork.Bind();
	sgNetwork.Listen();

	CreateAIThread(); //< AI 쓰레드 생성

	sgNetwork.AcceptClient();
	sgNetwork.CloseSocket();

	DeleteCriticalSection(&cs);
#endif //_S_LINUX_EPOLL_

	return 0;
}

void CreateAIThread( void )
{
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
	HANDLE hThread;		//< 스레드 핸들
	DWORD ThreadId;		//< 스레드 ID

	//< 각 클라이언트와 통신하는 스레드 생성
	hThread = CreateThread( NULL, 0, ProcessAI, NULL, 0, &ThreadId );
	if( hThread == NULL )
	{
		std::cout << "스레드 생성 실패";
	}
	else
	{
		CloseHandle( hThread );
	}
#endif //_S_LINUX_EPOLL_
}
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
DWORD WINAPI ProcessAI( LPVOID arg )
{
	DWORD m_dwSendTime[MAXENEMY];
	memset(m_dwSendTime, 0,sizeof(m_dwSendTime));
	DWORD	dwTime = 0;
	while( true )
	{
		for( size_t i = 0 ; i < MAXENEMY ; ++ i )
		{		
			dwTime = GetTickCount();
			if( m_dwSendTime[i] >= dwTime ) break;
			m_dwSendTime[i] = dwTime + 1000;
		
			sgAI.AI( &sgNetwork.m_pEnemy[i] );
			if(sgNetwork.m_pEnemy[i].bAlive == false) break;

	

			stEnemyTransportInfo Info;

			Info.bAlive			=	sgNetwork.m_pEnemy[i].bAlive;
			Info.nAnimationIndex = sgNetwork.m_pEnemy[i].nAnimationIndex;
			Info.nEnemyIndex = sgNetwork.m_pEnemy[i].nEnemyIndex;
			Info.nHpReduce = sgNetwork.m_pEnemy[i].dwHpReduce;
			Info.vPos = sgNetwork.m_pEnemy[i].vPos;
			Info.vTarget = sgNetwork.m_pEnemy[i].vTargetPos;

			sgNetwork.SendToServer( Info );
		}
	}

	std::cout << "AI 쓰레드 종료";

	return 0;
}
#endif //_S_LINUX_EPOLL_
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_