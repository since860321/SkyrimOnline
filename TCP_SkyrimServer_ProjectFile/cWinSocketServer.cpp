#include "defines.h"
#include "cWinSocketServer.h"
#include "cAI.h"

//< 스레드 함수 : 클라이언트와 데이터 통신
DWORD WINAPI ProcessReceive( LPVOID arg );

cWinSocketServer::cWinSocketServer(void)
{
	Init();
	memset( m_bUsed, 0, sizeof(bool)*MAXCLIENT );
	m_dwSendTime = 0;

	m_pEnemy = new stEnemyInfo[MAXENEMY];
	for( size_t i = 0 ; i < MAXENEMY ; ++ i )
	{
		m_pEnemy[i].bAlive = true;
		m_pEnemy[i].dwAttackDamage = 4;
		m_pEnemy[i].dwHp =	m_pEnemy[i].dwHpMax= 50;
		m_pEnemy[i].dwHpReduce =0;
		m_pEnemy[i].vPos = D3DXVECTOR3(0,0,0);
		m_pEnemy[i].vStartPos = D3DXVECTOR3(0,0,0);
		m_pEnemy[i].pTarget = NULL;
		m_pEnemy[i].nAnimationIndex = 0;
		m_pEnemy[i].nEnemyIndex = i;
		m_pEnemy[i].fPerceptionLength = 1000;
		m_pEnemy[i].MonsterType = CT_MONSTER_DRAGON;
		m_pEnemy[i].vTargetPos = D3DXVECTOR3( 0, 0, 0 );
		m_pEnemy[i].fAttackLange = 400.0f;
		m_pEnemy[i].dwAttackCoolTime	=	3000;	
		m_pEnemy[i].dwAttackTime		=	0;	
		m_pEnemy[i].dwRecoveryCoolTime	=	4000;		 
		m_pEnemy[i].dwRecoveryTime		=	0;			 
		m_pEnemy[i].dwGenerationCoolTime	=	10000;		
		m_pEnemy[i].dwGenerationTime		=	0;			
		m_pEnemy[i].fMoveSpeed		=	100.0f;			
	}

	for( size_t i = 0 ; i < MAXCLIENT ; ++ i )
	{
		m_Client[i]; //< 10개의 공간을 미리 할당
		memset( &m_Client[i], 0, sizeof(m_Client[i]) );
		m_Client[i].bAlive = false;
	}

	sgAI.SetPlayerInfo(&m_Client);
}

cWinSocketServer::~cWinSocketServer(void)
{
	Cleanup();
	SAFE_DELETE_ARRAY( m_pEnemy );
}

cWinSocketServer& cWinSocketServer::GetInstance()
{
	static cWinSocketServer instance;
	return instance;
}

void cWinSocketServer::Init( void )
{
	if( WSAStartup( MAKEWORD( 2, 2), &m_Wsa ) != 0 )
	{
		assert( false && "윈속 초기화 실패" );
	}
}

void cWinSocketServer::Cleanup( void )
{
	WSACleanup();
}

void cWinSocketServer::CreateListenSocket( void )
{
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 ); //< TCP/IP 프로토콜 전용 소켓 생성
	if( m_ListenSock == INVALID_SOCKET ) err_quit( "socket()" );
}

void cWinSocketServer::Bind( void )
{
	int retval = 0;

	SOCKADDR_IN serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	serveraddr.sin_family = AF_INET;	
	serveraddr.sin_port = htons( 9000 );	//< 접속 전용 포트번호
	serveraddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	retval = bind( m_ListenSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr) ); 
	if( retval == SOCKET_ERROR ) err_quit( "bind()" );
}

void cWinSocketServer::Listen( void )
{
	int retval = 0;

	retval = listen( m_ListenSock, SOMAXCONN ); 
	if( retval == SOCKET_ERROR ) err_quit( "listen()" );
}

void cWinSocketServer::AcceptClient( void )
{
	//< 데이터 통신에 사용하는 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;		//< 스레드 핸들
	DWORD ThreadId;		//< 스레드 ID

	while( true )
	{
		//< Accept
		addrlen = sizeof(clientaddr);
		client_sock = accept( m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			err_display( "accept()" );
			continue;
		}
		std::cout << "클라이언트 접속 : IP 주소 = " << inet_ntoa( clientaddr.sin_addr ) << "포트 번호 = " << ntohs( clientaddr.sin_port );
		
		SendInitiateInfo( client_sock, &ThreadId ); //< 초기정보 전달
		SendEnemyInitiateInfo( client_sock ); //< 몬스터 초기정보 전달

		//< 각 클라이언트와 통신하는 스레드 생성
		hThread = CreateThread( NULL, 0, ProcessReceive, (LPVOID)client_sock, 0, &ThreadId );
		if( hThread == NULL )
		{
			std::cout << "스레드 생성 실패";
		}
		else
		{
			CloseHandle( hThread );
		}
	}//< while
}

void cWinSocketServer::SendInitiateInfo(SOCKET sock, DWORD* clientID )
{
	char szBuf[1024] = {0};

	//< 패킷 만들어서 클라이언트로 보냄
	stConnectPacket packet;
	packet.header = sizeof(packet.ID);
	for( size_t index = 0 ; index < MAXCLIENT ; ++ index )
	{
		if( m_bUsed[index] == false )
		{
			packet.ID = index;
			m_bUsed[index] = true;
			break;
		}
	}
	memcpy( szBuf, &packet, sizeof(packet) );
	*clientID = packet.ID; //< 쓰레드 ID ( 현재 필요없음 )

	std::cout << "접속자 ID: " << packet.ID << std::endl;
	//< 클라이언트 등록
	m_Client[packet.ID].bAlive = true;
	m_Client[packet.ID].nID = packet.ID;
	m_Client[packet.ID].sock = sock;
	memset( &m_Client[packet.ID].transformInfo, 0, sizeof(stTransformInfo) );
//	sgAI.SetPlayerInfo(&m_Client); //< AI 맵 컨테이너 포인터 갱신

	//< 클라이언트로 데이터 보내기(send())
	int retval = send( sock, szBuf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}

	//< 모든 클라이언트에게 새로운 클라이언트 데이터 생성하라고 패킷 보내기
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == *clientID ) continue; //<자기자신은 패스

		if( m_bUsed[iter->first] == false ) continue; //< 접속 안한 클라이언트는 패스

		stGeneratePacket newClient;
		newClient.header = sizeof(newClient.Length);
		newClient.Length.Length = 0;
		newClient.Length.Option = PO_GENERATE;
		newClient.Length.Sender = SND_MANAGER;
		newClient.Length.clientId = packet.ID;
		
		memcpy( szBuf, &newClient, sizeof(newClient) );

		retval = send( iter->second.sock, szBuf, sizeof(newClient), 0 );
		if( retval == SOCKET_ERROR )
		{
			err_display( "send()" ); 
			return;
		}
	}
}

void cWinSocketServer::SendEnemyInitiateInfo(SOCKET sock )
{
	char szBuf[1024] = {0};

	for( size_t i = 0 ; i < MAXENEMY ; ++ i )
	{
		//< 패킷 만들어서 클라이언트로 보냄
		stEnemyTransportInfoPacket packet;
		packet.header = sizeof(packet.Length);
		packet.Length.Length = sizeof(packet.EnemyInfo);
		packet.Length.Option = PO_TRANSFORM;
		packet.Length.Sender = SND_ENEMY;
		packet.Length.clientId = i;	//< 적의 인덱스
		packet.EnemyInfo.bAlive			=	m_pEnemy[i].bAlive;
		packet.EnemyInfo.nAnimationIndex = m_pEnemy[i].nAnimationIndex;
		packet.EnemyInfo.nEnemyIndex = m_pEnemy[i].nEnemyIndex;
		packet.EnemyInfo.nHpReduce = m_pEnemy[i].dwHpReduce;
		packet.EnemyInfo.vPos = m_pEnemy[i].vPos;
		packet.EnemyInfo.vTarget = m_pEnemy[i].vTargetPos;

		memcpy( szBuf, &packet, sizeof(packet) );

		//< 클라이언트로 데이터 보내기(send())
		int retval = send( sock, szBuf, sizeof(packet), 0 );
		if( retval == SOCKET_ERROR )
		{
			err_display( "send()" ); 
			return;
		}
	}
}

int cWinSocketServer::recvn( SOCKET s, char* buf, int len, int flags )
{
	int received;
	char* ptr = buf;
	int left = len;

	while( left > 0 )
	{
		received = recv( s, ptr, left, flags );
		if( received == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}
		else if( received == 0 ) break;

		left -= received;
		ptr += received;
	}
	
	return (len - left);
}

void cWinSocketServer::CloseSocket( void )
{
	closesocket( m_ListenSock );
}

void cWinSocketServer::err_quit( char* msg )
{
	LPVOID szMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&szMsgBuf, 0, NULL );
	assert( false && szMsgBuf );
	LocalFree( szMsgBuf );
}

void cWinSocketServer::err_display( char* msg )
{
	LPVOID szMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&szMsgBuf, 0, NULL );
	std::cout << msg << ":" << szMsgBuf;
	LocalFree( szMsgBuf );
}

int cWinSocketServer::PacketToken( SOCKET s, std::queue<char>& QueueBuf, int len, int flags, PART& part, int& nNeedSize  )
{
	static char szPacketToken[256] = {0};
	static PACKET_OPTION PacketOption;
	static SENDER	Sender;
	static int nLeft = nNeedSize;
	static int nLast = 0;	//< 패킷 데이터 문자열의 마지막 인덱스
	static int clientId;

	char szReceive[1024] = {0};
	int received = recv( s, szReceive, len, flags ); 

	for( int i = 0; i < received ; i ++ )
	{
		QueueBuf.push( szReceive[i] ); //< 받은만큼 큐에 삽입
	}

	while( !QueueBuf.empty() ) //< 큐가 빌때까지 데이터 처리
	{
		switch( part )
		{
		case PART_HEADER :
			{
				nNeedSize = QueueBuf.front();
				QueueBuf.pop();
				part = PART_LENGTH;
			}
		case PART_LENGTH :
			{
				nLeft = nNeedSize; //< 이전에 남은 양부터
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf.front();
					QueueBuf.pop(); 
					nLeft = nLeft - 1; 
					if( QueueBuf.empty() ) //< 모자라면 남은 양 저장하고 더 받으러 리턴
					{
						nNeedSize = nLeft;
						if( nLeft != 0 ) return received; //< 모자라면 리턴, 딱맞으면 패스
					}
				}

				//< 받은 LENGTH 에 대한 처리
				memcpy( &nNeedSize, &szPacketToken[0], sizeof(WORD) );	//< 데이터 길이
				WORD temp;
				memcpy( &temp, &szPacketToken[2], sizeof(WORD) ); //< 데이터 종류
				PacketOption = PACKET_OPTION(temp);
				memcpy( &temp, &szPacketToken[4], sizeof(WORD) ); //< 발신 객체
				Sender = SENDER(temp);
				memcpy( &clientId, &szPacketToken[6], sizeof(int) ); //< 클라이언트 ID

				//< 처리 후에는 szMemory 비움
				memset( szPacketToken, 0, 256 );
				nLast = 0;
				part = PART_DATA;
			}
		case PART_DATA :
			{
				nLeft = nNeedSize; //< 이전에 남은 양부터
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf.front();
					QueueBuf.pop(); 
					nLeft = nLeft - 1; 
					if( nLeft == 0 ) break; //< 더이상 받아야할게 없다면
					if( QueueBuf.empty() ) //< 모자라면 남은 양 저장하고 더 받으러 리턴	
					{
						nNeedSize = nLeft;	
						if( nLeft != 0 ) return received; //< 모자라면 리턴, 딱맞으면 패스
					}
				}
			
				//< 받은 DATA 에 대한 처리
				ProcessData( s, Sender, PacketOption, szPacketToken, clientId );

				//< 처리 후에는 szMemory 비움
				memset( szPacketToken, 0, 256 );
				nLast = 0;
				nNeedSize = 1;
				part = PART_HEADER;
			}
		}//< switch
	}//< while

	return received;
}

void cWinSocketServer::ProcessData( SOCKET s, SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID )
{
	//< 발신 주체와 패킷의 종류에 따라 어떤 프로세스로 넘길것인지 정해짐
	switch( PacketOption )
	{
	case PO_TRANSFORM:
		{
			if( Sender == SND_PLAYER ) { ProcessPlayerTransform( s, Buf, nClientID ); }
			else if( Sender == SND_ENEMY ) { ProcessEnemyTransform( s, Buf, nClientID ); }
		}
		break;
	case PO_HPREDUCE:
		{
			if( Sender == SND_ENEMY ) { ProcessEnemyHpReduce( s, Buf, nClientID ); }
		}
		break;
	}
}

void cWinSocketServer::ProcessPlayerTransform( SOCKET s, char* Buf, int nClientID )
{
	stClientInfo Info;
	memcpy( &Info, Buf, sizeof(stClientInfo) );
	//std::cout << "플레이어 위치 : " << Info.vPos.x << " " << Info.vPos.y << " " << Info.vPos.z << std::endl;
	//std::cout << "플레이어 크기 : " << Info.vScale.x << " " << Info.vScale.y << " " << Info.vScale.z << std::endl;
	//std::cout << "플레이어 회전 : " << Info.vRot.x << " " << Info.vRot.y << " " << Info.vRot.z << std::endl;

	//< 정보를 컨테이너에 저장
	m_Client[nClientID].transformInfo = Info.transformInfo;
	m_Client[nClientID].nHP	=	Info.nHP;
	if( nClientID >= static_cast<int>(m_Client.size()) )
	{
		std::cout << "이상한 ID 들어옴." << std::endl;
		system("pause");
	}

	//< 모든 클라이언트에게 이동한 클라이언트 정보를 전송
	char szBuf[1024];
	stClientInfoPacket client;
	client.header = sizeof(client.Length);
	client.Length.Length = sizeof(client.clientInfo);
	client.Length.Option = PO_TRANSFORM;
	client.Length.Sender = SND_PLAYER;
	client.Length.clientId = nClientID;
	client.clientInfo.bAlive = Info.bAlive;
	client.clientInfo.nHP	=	Info.nHP;
	client.clientInfo.nID = nClientID;
	client.clientInfo.sock = s;
	client.clientInfo.transformInfo = Info.transformInfo; //< 전송받은 정보
	client.clientInfo.vDrt = Info.vDrt;
	client.clientInfo.fSpeed = Info.fSpeed;
	client.clientInfo.CurrentAnimation = Info.CurrentAnimation;
	client.clientInfo.useWeapon = Info.useWeapon;
	memcpy( szBuf, &client, sizeof(client) );

	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == nClientID ) continue; //< 자기자신에 대한 정보는 안받음

		if( m_bUsed[iter->first] == false ) continue; //< 접속 안한 클라이언트는 패스

		int retval = send( iter->second.sock, szBuf, sizeof(client), 0 );
		if( retval == SOCKET_ERROR )
		{
			err_display( "send()" ); 
			return;
		}
	}

}

void cWinSocketServer::ProcessEnemyTransform( SOCKET s, char* Buf, int nClientID )
{
	stTransformInfo Info;
	memcpy( &Info, Buf, sizeof(stTransformInfo) );
	std::cout << "적군 위치 : " << Info.vPos.x << " " << Info.vPos.y << " " << Info.vPos.z << std::endl;
	std::cout << "적군 크기 : " << Info.vScale.x << " " << Info.vScale.y << " " << Info.vScale.z << std::endl;
	std::cout << "적군 회전 : " << Info.vRot.x << " " << Info.vRot.y << " " << Info.vRot.z << std::endl;
}

void cWinSocketServer::ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID )
{
	stHpReduceInfo Info;
	memcpy( &Info, Buf, sizeof(stHpReduceInfo) );

	/* nClientID == HP 깎은놈 
	m_Client[nClientID]*/
	
	if(	m_pEnemy[Info.nMonsterIndex].pTarget	==	NULL)
	{
		m_pEnemy[Info.nMonsterIndex].pTarget = &m_Client[nClientID];
	}

	m_pEnemy[Info.nMonsterIndex].dwHpReduce += Info.nHpReduce;
}

DWORD WINAPI ProcessReceive( LPVOID arg )
{
	SOCKET client_sock = (SOCKET) arg;	//< 스레드 인자로 클라이언트 소켓을 전달받음
	std::queue<char> QueueBuf; //< 큐 버퍼
	PART part = PART_HEADER; //< 헤더부터 받아야함
	int nNeedSize = 1;	//< 헤더부터 받아야함
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	//< 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername( client_sock, (SOCKADDR*)&clientaddr, &addrlen ); //< 클라이언트 주소와 포트번호

	while( true )
	{
		//< 데이터 받기
		retval = sgNetwork.PacketToken( client_sock, QueueBuf, BUFSIZE, 0, part, nNeedSize );

		if( retval == SOCKET_ERROR )
		{
			sgNetwork.err_display( "recv()" );
			break;
		}
		else if( retval == 0 )
		{
			break;
		}
	}

	//< 네트워크 매니저에게 해당 클라이언트 데이터 삭제하라고 명령
	sgNetwork.DeleteClientProcess( client_sock );

	closesocket( client_sock );
	std::cout << "클라이언트 접속 종료";

	return 0;
}

void cWinSocketServer::DeleteClientProcess(SOCKET deleteClientSock )
{
	int addrlen;
	SOCKADDR_IN clientaddr;

	//< 맵에서 해당 클라이언트 제거
	int nDeleteClientIndex;
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->second.sock == deleteClientSock ) 
		{
			nDeleteClientIndex = iter->first;
			m_Client[nDeleteClientIndex].bAlive = false; //< 어쩌면 이건 천수형이 처리했을지도 모르므로.. bUsed만 false하는게 나을수도...
			m_Client[nDeleteClientIndex].nHP = 0;
			m_Client[nDeleteClientIndex].sock = 0;
			m_bUsed[nDeleteClientIndex] = false;
			break;
		}
	}

	char szBuf[1024] = {0};
	//< 모든 클라이언트에게 해당 클라이언트 데이터 삭제하라고 패킷 보내기
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( m_bUsed[iter->first] == false ) continue; //< 접속 안한 클라이언트는 패스

		stDeletePacket delClient;
		delClient.header = sizeof(delClient.Length);
		delClient.Length.Length = 0;
		delClient.Length.Option = PO_DESTROY;
		delClient.Length.Sender = SND_MANAGER;
		delClient.Length.clientId = nDeleteClientIndex;
		memcpy( szBuf, &delClient, sizeof(delClient) );

		//< 클라이언트 정보 얻기
		addrlen = sizeof(clientaddr);
		getpeername( iter->second.sock, (SOCKADDR*)&clientaddr, &addrlen ); //< 클라이언트 주소와 포트번호
		std::cout << "IP 주소 : " << inet_ntoa( clientaddr.sin_addr ) << ", 포트 번호 : " << ntohs( clientaddr.sin_port );

		int retval = send( iter->second.sock, szBuf, sizeof(delClient), 0 ); //< 다른클라이언트에게 쏴야하는데 그걸 못하는거 같음.
		if( retval == SOCKET_ERROR )
		{
			sgNetwork.err_display( "send()" ); 
			return ;
		}
	}
}

void cWinSocketServer::SendToServer( stEnemyTransportInfo Info )
{
	char buf[1024] = {0};
	
	//< 패킷 만들어서 서버로 보냄
	stEnemyTransportInfoPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.EnemyInfo);
	packet.Length.Option = PO_TRANSFORM;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = Info.nEnemyIndex;

	packet.EnemyInfo.bAlive			=Info.bAlive;
	packet.EnemyInfo.nAnimationIndex = Info.nAnimationIndex;
	packet.EnemyInfo.nEnemyIndex = Info.nEnemyIndex;
	packet.EnemyInfo.nHpReduce = Info.nHpReduce;
	packet.EnemyInfo.vPos = Info.vPos;
	packet.EnemyInfo.vTarget = Info.vTarget;

	memcpy( buf, &packet, sizeof(packet) );

	//< 1초에 1번씩 보냄

	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( m_bUsed[iter->first] == false ) continue; //< 접속 안한 클라이언트는 패스

		int retval = send( iter->second.sock, buf, sizeof(packet), 0 ); 
		if( retval == SOCKET_ERROR )
		{
			sgNetwork.err_display( "send()" ); 
			return ;
		}
	}
}

void cWinSocketServer::SendToServer(int nClientIndex, int nClientHp )
{
	/* 해당 인덱스의 클라이언트 HP를 감소시키는 패킷을 보내라 */
	if( m_bUsed[nClientIndex] == false ) return; //< 접속 안한 클라이언트는 패스

	if(( nClientIndex >= static_cast<int>(m_Client.size()) ) || ( nClientIndex < 0 ))
	{
		std::cout << "이상한 ID 들어옴.." << std::endl;
		system("pause");
	}

	char szBuf[1024] = {0};

	stClientHpReducePacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = 0;
	packet.Length.Option = PO_HPREDUCE;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = nClientHp; //< hp 값 전달.
	memcpy( szBuf, &packet, sizeof(packet) );

	int retval = send( m_Client[nClientIndex].sock, szBuf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
}


