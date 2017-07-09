// TCP_SkyrimServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

CRITICAL_SECTION cs;

void CreateAIThread( void );
DWORD WINAPI ProcessAI( LPVOID arg ); //< AI ������

int _tmain(int argc, _TCHAR* argv[])
{
	InitializeCriticalSection(&cs);	

	sgNetwork.CreateListenSocket();
	sgNetwork.Bind();
	sgNetwork.Listen();

	CreateAIThread(); //< AI ������ ����

	sgNetwork.AcceptClient();
	sgNetwork.CloseSocket();

	DeleteCriticalSection(&cs);

	return 0;
}

void CreateAIThread( void )
{
	HANDLE hThread;		//< ������ �ڵ�
	DWORD ThreadId;		//< ������ ID

	//< �� Ŭ���̾�Ʈ�� ����ϴ� ������ ����
	hThread = CreateThread( NULL, 0, ProcessAI, NULL, 0, &ThreadId );
	if( hThread == NULL )
	{
		std::cout << "������ ���� ����";
	}
	else
	{
		CloseHandle( hThread );
	}
}

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

	std::cout << "AI ������ ����";

	return 0;
}
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_