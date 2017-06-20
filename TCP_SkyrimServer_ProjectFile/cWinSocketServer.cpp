#include "stdafx.h"
#include "cWinSocketServer.h"
#include "cAI.h"

//< ������ �Լ� : Ŭ���̾�Ʈ�� ������ ���
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
		m_pEnemy[i].MonsterType = MONSTER_DRAGON;
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
		m_Client[i]; //< 10���� ������ �̸� �Ҵ�
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
		assert( false && "���� �ʱ�ȭ ����" );
	}
}

void cWinSocketServer::Cleanup( void )
{
	WSACleanup();
}

void cWinSocketServer::CreateListenSocket( void )
{
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 ); //< TCP/IP �������� ���� ���� ����
	if( m_ListenSock == INVALID_SOCKET ) err_quit( "socket()" );
}

void cWinSocketServer::Bind( void )
{
	int retval = 0;

	SOCKADDR_IN serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	serveraddr.sin_family = AF_INET;	
	serveraddr.sin_port = htons( 9000 );	//< ���� ���� ��Ʈ��ȣ
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
	//< ������ ��ſ� ����ϴ� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;		//< ������ �ڵ�
	DWORD ThreadId;		//< ������ ID

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
		std::cout << "Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa( clientaddr.sin_addr ) << "��Ʈ ��ȣ = " << ntohs( clientaddr.sin_port );
		
		SendInitiateInfo( client_sock, &ThreadId ); //< �ʱ����� ����
		SendEnemyInitiateInfo( client_sock ); //< ���� �ʱ����� ����

		//< �� Ŭ���̾�Ʈ�� ����ϴ� ������ ����
		hThread = CreateThread( NULL, 0, ProcessReceive, (LPVOID)client_sock, 0, &ThreadId );
		if( hThread == NULL )
		{
			std::cout << "������ ���� ����";
		}
		else
		{
			CloseHandle( hThread );
		}
	}//< while
}

void cWinSocketServer::SendInitiateInfo( IN SOCKET sock, OUT DWORD* clientID )
{
	char szBuf[1024] = {0};

	//< ��Ŷ ���� Ŭ���̾�Ʈ�� ����
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
	*clientID = packet.ID; //< ������ ID ( ���� �ʿ���� )

	std::cout << "������ ID: " << packet.ID << std::endl;
	//< Ŭ���̾�Ʈ ���
	m_Client[packet.ID].bAlive = true;
	m_Client[packet.ID].nID = packet.ID;
	m_Client[packet.ID].sock = sock;
	memset( &m_Client[packet.ID].transformInfo, 0, sizeof(stTransformInfo) );
//	sgAI.SetPlayerInfo(&m_Client); //< AI �� �����̳� ������ ����

	//< Ŭ���̾�Ʈ�� ������ ������(send())
	int retval = send( sock, szBuf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}

	//< ��� Ŭ���̾�Ʈ���� ���ο� Ŭ���̾�Ʈ ������ �����϶�� ��Ŷ ������
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == *clientID ) continue; //<�ڱ��ڽ��� �н�

		if( m_bUsed[iter->first] == false ) continue; //< ���� ���� Ŭ���̾�Ʈ�� �н�

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

void cWinSocketServer::SendEnemyInitiateInfo( IN SOCKET sock )
{
	char szBuf[1024] = {0};

	for( size_t i = 0 ; i < MAXENEMY ; ++ i )
	{
		//< ��Ŷ ���� Ŭ���̾�Ʈ�� ����
		stEnemyTransportInfoPacket packet;
		packet.header = sizeof(packet.Length);
		packet.Length.Length = sizeof(packet.EnemyInfo);
		packet.Length.Option = PO_TRANSFORM;
		packet.Length.Sender = SND_ENEMY;
		packet.Length.clientId = i;	//< ���� �ε���
		packet.EnemyInfo.bAlive			=	m_pEnemy[i].bAlive;
		packet.EnemyInfo.nAnimationIndex = m_pEnemy[i].nAnimationIndex;
		packet.EnemyInfo.nEnemyIndex = m_pEnemy[i].nEnemyIndex;
		packet.EnemyInfo.nHpReduce = m_pEnemy[i].dwHpReduce;
		packet.EnemyInfo.vPos = m_pEnemy[i].vPos;
		packet.EnemyInfo.vTarget = m_pEnemy[i].vTargetPos;

		memcpy( szBuf, &packet, sizeof(packet) );

		//< Ŭ���̾�Ʈ�� ������ ������(send())
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
	static int nLast = 0;	//< ��Ŷ ������ ���ڿ��� ������ �ε���
	static int clientId;

	char szReceive[1024] = {0};
	int received = recv( s, szReceive, len, flags ); 

	for( int i = 0; i < received ; i ++ )
	{
		QueueBuf.push( szReceive[i] ); //< ������ŭ ť�� ����
	}

	while( !QueueBuf.empty() ) //< ť�� �������� ������ ó��
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
				nLeft = nNeedSize; //< ������ ���� �����
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf.front();
					QueueBuf.pop(); 
					nLeft = nLeft - 1; 
					if( QueueBuf.empty() ) //< ���ڶ�� ���� �� �����ϰ� �� ������ ����
					{
						nNeedSize = nLeft;
						if( nLeft != 0 ) return received; //< ���ڶ�� ����, �������� �н�
					}
				}

				//< ���� LENGTH �� ���� ó��
				memcpy( &nNeedSize, &szPacketToken[0], sizeof(WORD) );	//< ������ ����
				WORD temp;
				memcpy( &temp, &szPacketToken[2], sizeof(WORD) ); //< ������ ����
				PacketOption = PACKET_OPTION(temp);
				memcpy( &temp, &szPacketToken[4], sizeof(WORD) ); //< �߽� ��ü
				Sender = SENDER(temp);
				memcpy( &clientId, &szPacketToken[6], sizeof(int) ); //< Ŭ���̾�Ʈ ID

				//< ó�� �Ŀ��� szMemory ���
				memset( szPacketToken, 0, 256 );
				nLast = 0;
				part = PART_DATA;
			}
		case PART_DATA :
			{
				nLeft = nNeedSize; //< ������ ���� �����
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf.front();
					QueueBuf.pop(); 
					nLeft = nLeft - 1; 
					if( nLeft == 0 ) break; //< ���̻� �޾ƾ��Ұ� ���ٸ�
					if( QueueBuf.empty() ) //< ���ڶ�� ���� �� �����ϰ� �� ������ ����	
					{
						nNeedSize = nLeft;	
						if( nLeft != 0 ) return received; //< ���ڶ�� ����, �������� �н�
					}
				}
			
				//< ���� DATA �� ���� ó��
				ProcessData( s, Sender, PacketOption, szPacketToken, clientId );

				//< ó�� �Ŀ��� szMemory ���
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
	//< �߽� ��ü�� ��Ŷ�� ������ ���� � ���μ����� �ѱ������ ������
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
	//std::cout << "�÷��̾� ��ġ : " << Info.vPos.x << " " << Info.vPos.y << " " << Info.vPos.z << std::endl;
	//std::cout << "�÷��̾� ũ�� : " << Info.vScale.x << " " << Info.vScale.y << " " << Info.vScale.z << std::endl;
	//std::cout << "�÷��̾� ȸ�� : " << Info.vRot.x << " " << Info.vRot.y << " " << Info.vRot.z << std::endl;

	//< ������ �����̳ʿ� ����
	m_Client[nClientID].transformInfo = Info.transformInfo;
	m_Client[nClientID].nHP	=	Info.nHP;
	if( nClientID >= static_cast<int>(m_Client.size()) )
	{
		std::cout << "�̻��� ID ����." << std::endl;
		system("pause");
	}

	//< ��� Ŭ���̾�Ʈ���� �̵��� Ŭ���̾�Ʈ ������ ����
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
	client.clientInfo.transformInfo = Info.transformInfo; //< ���۹��� ����
	client.clientInfo.vDrt = Info.vDrt;
	client.clientInfo.fSpeed = Info.fSpeed;
	client.clientInfo.CurrentAnimation = Info.CurrentAnimation;
	client.clientInfo.useWeapon = Info.useWeapon;
	memcpy( szBuf, &client, sizeof(client) );

	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == nClientID ) continue; //< �ڱ��ڽſ� ���� ������ �ȹ���

		if( m_bUsed[iter->first] == false ) continue; //< ���� ���� Ŭ���̾�Ʈ�� �н�

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
	std::cout << "���� ��ġ : " << Info.vPos.x << " " << Info.vPos.y << " " << Info.vPos.z << std::endl;
	std::cout << "���� ũ�� : " << Info.vScale.x << " " << Info.vScale.y << " " << Info.vScale.z << std::endl;
	std::cout << "���� ȸ�� : " << Info.vRot.x << " " << Info.vRot.y << " " << Info.vRot.z << std::endl;
}

void cWinSocketServer::ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID )
{
	stHpReduceInfo Info;
	memcpy( &Info, Buf, sizeof(stHpReduceInfo) );

	/* nClientID == HP ������ 
	m_Client[nClientID]*/
	
	if(	m_pEnemy[Info.nMonsterIndex].pTarget	==	NULL)
	{
		m_pEnemy[Info.nMonsterIndex].pTarget = &m_Client[nClientID];
	}

	m_pEnemy[Info.nMonsterIndex].dwHpReduce += Info.nHpReduce;
}

DWORD WINAPI ProcessReceive( LPVOID arg )
{
	SOCKET client_sock = (SOCKET) arg;	//< ������ ���ڷ� Ŭ���̾�Ʈ ������ ���޹���
	std::queue<char> QueueBuf; //< ť ����
	PART part = PART_HEADER; //< ������� �޾ƾ���
	int nNeedSize = 1;	//< ������� �޾ƾ���
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	//< Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername( client_sock, (SOCKADDR*)&clientaddr, &addrlen ); //< Ŭ���̾�Ʈ �ּҿ� ��Ʈ��ȣ

	while( true )
	{
		//< ������ �ޱ�
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

	//< ��Ʈ��ũ �Ŵ������� �ش� Ŭ���̾�Ʈ ������ �����϶�� ���
	sgNetwork.DeleteClientProcess( client_sock );

	closesocket( client_sock );
	std::cout << "Ŭ���̾�Ʈ ���� ����";

	return 0;
}

void cWinSocketServer::DeleteClientProcess( IN SOCKET deleteClientSock )
{
	int addrlen;
	SOCKADDR_IN clientaddr;

	//< �ʿ��� �ش� Ŭ���̾�Ʈ ����
	int nDeleteClientIndex;
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->second.sock == deleteClientSock ) 
		{
			nDeleteClientIndex = iter->first;
			m_Client[nDeleteClientIndex].bAlive = false; //< ��¼�� �̰� õ������ ó���������� �𸣹Ƿ�.. bUsed�� false�ϴ°� ��������...
			m_Client[nDeleteClientIndex].nHP = 0;
			m_Client[nDeleteClientIndex].sock = 0;
			m_bUsed[nDeleteClientIndex] = false;
			break;
		}
	}

	char szBuf[1024] = {0};
	//< ��� Ŭ���̾�Ʈ���� �ش� Ŭ���̾�Ʈ ������ �����϶�� ��Ŷ ������
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( m_bUsed[iter->first] == false ) continue; //< ���� ���� Ŭ���̾�Ʈ�� �н�

		stDeletePacket delClient;
		delClient.header = sizeof(delClient.Length);
		delClient.Length.Length = 0;
		delClient.Length.Option = PO_DESTROY;
		delClient.Length.Sender = SND_MANAGER;
		delClient.Length.clientId = nDeleteClientIndex;
		memcpy( szBuf, &delClient, sizeof(delClient) );

		//< Ŭ���̾�Ʈ ���� ���
		addrlen = sizeof(clientaddr);
		getpeername( iter->second.sock, (SOCKADDR*)&clientaddr, &addrlen ); //< Ŭ���̾�Ʈ �ּҿ� ��Ʈ��ȣ
		std::cout << "IP �ּ� : " << inet_ntoa( clientaddr.sin_addr ) << ", ��Ʈ ��ȣ : " << ntohs( clientaddr.sin_port );

		int retval = send( iter->second.sock, szBuf, sizeof(delClient), 0 ); //< �ٸ�Ŭ���̾�Ʈ���� �����ϴµ� �װ� ���ϴ°� ����.
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
	
	//< ��Ŷ ���� ������ ����
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

	//< 1�ʿ� 1���� ����

	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( m_bUsed[iter->first] == false ) continue; //< ���� ���� Ŭ���̾�Ʈ�� �н�

		int retval = send( iter->second.sock, buf, sizeof(packet), 0 ); 
		if( retval == SOCKET_ERROR )
		{
			sgNetwork.err_display( "send()" ); 
			return ;
		}
	}
}

void cWinSocketServer::SendToServer( IN int nClientIndex, IN int nClientHp )
{
	/* �ش� �ε����� Ŭ���̾�Ʈ HP�� ���ҽ�Ű�� ��Ŷ�� ������ */
	if( m_bUsed[nClientIndex] == false ) return; //< ���� ���� Ŭ���̾�Ʈ�� �н�

	if(( nClientIndex >= static_cast<int>(m_Client.size()) ) || ( nClientIndex < 0 ))
	{
		std::cout << "�̻��� ID ����.." << std::endl;
		system("pause");
	}

	char szBuf[1024] = {0};

	stClientHpReducePacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = 0;
	packet.Length.Option = PO_HPREDUCE;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = nClientHp; //< hp �� ����.
	memcpy( szBuf, &packet, sizeof(packet) );

	int retval = send( m_Client[nClientIndex].sock, szBuf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
}


