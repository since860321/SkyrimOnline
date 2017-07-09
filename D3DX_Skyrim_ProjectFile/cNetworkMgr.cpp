#include "StdAfx.h"
#include "cPlayerCharacter.h"
#include "cEnemyCharacter.h"
#include "BaseSkinnedMesh.h"
#include "cObjFileMesh.h"
#include "cGameTime.h"
#include "cNetworkMgr.h"
#include "cCharacterDamage.h"
#include "cSoundMgr.h"

DWORD WINAPI ProcessReceive( LPVOID arg );

cNetworkMgr::cNetworkMgr(void)
{
	m_pPlayer = NULL;
	m_pEnemy = NULL;
//	m_pWeapon = NULL;
	Init();
	
	m_dwSendTime = 0;
}


cNetworkMgr::~cNetworkMgr(void)
{
	CloseSocket();	//< 소켓 닫기
	Cleanup();		//< 윈속 종료

	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	while( 0 != m_Client.size() )
	{
		iter->second.pModel->Release();
		iter = m_Client.erase(iter);
	}
}

void cNetworkMgr::Init( void )
{
	if( WSAStartup( MAKEWORD( 2, 2), &m_Wsa ) != 0 )
	{
		assert( false && "윈속 초기화 실패" );
	}

	for( size_t i = 0 ; i < MAXCLIENT ; ++ i )
	{
		m_Client[i]; //< 10개의 공간을 미리 할당
		memset( &m_Client[i], 0, sizeof(m_Client[i]) );
		m_Client[i].bAlive = false;
		m_Client[i].pModel = new BaseD3D::BaseSkinnedMesh();
		m_Client[i].pModel->Init( sgDevice, "./Resources/xfile/character_test01.X", NULL );
		m_Client[i].pModel->SetAnimationIndex(1, false); 
		m_Client[i].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex와 일치
		m_Client[i].useWeapon = WEAPON_TWOHAND; //<임시로 디폴트
	}
}

void cNetworkMgr::Cleanup( void )
{
	WSACleanup();
}

void cNetworkMgr::CreateSocket( void )
{
	m_Sock = socket( AF_INET, SOCK_STREAM, 0 );	 //< TCP 소켓 생성
	if( m_Sock == INVALID_SOCKET ) err_quit( "socket()" );	//< 에러 메시지 발생 후 프로그램 종료
}

LRESULT cNetworkMgr::ConnectServer( void )
{
	int retval;

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons( 9000 );	//< 연결하려는 서버의 포트번호
	serveraddr.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1");	//"192.168.11.53"//< 루프백 주소( 서버와 클라이언트가 동일한 컴퓨터 )
	retval = connect( m_Sock, (SOCKADDR*) &serveraddr, sizeof(serveraddr) );	//< 클라이언트의 IP주소와 포트번호를 내부적으로 소켓에 할당
	if( retval == SOCKET_ERROR ) 
	{
		MessageBox( g_hWnd, "서버 접속 실패", "실패", MB_OK );
		return E_FAIL;
	}

	//< 접속 성공시 고유 ID 넘겨주고 초기정보 받음
	ReceiveClientInformation();

	return S_OK;
}

void cNetworkMgr::ReceiveClientInformation( void )
{
	char buf[SENDBUFSIZE + 1];

	int received = recvn( m_Sock, buf, sizeof(stConnectPacket), 0 );
	if( received == SOCKET_ERROR )
	{
		err_display("초기정보받아오기실패");
		return ;
	}

	//< 서버로부터 초기정보 받아서 세팅
	stConnectPacket packet;
	memcpy( &packet, buf, sizeof(packet) );
	m_nClientID = packet.ID;
	//소켓 = packet.sock
	//초기위치 = packet.transformInfo;
}

int cNetworkMgr::recvn( SOCKET s, char* buf, int len, int flags )
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

void cNetworkMgr::SetReceiver( cPlayerCharacter* pPlayer, cEnemyCharacter* pEnemy )
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;
}

void cNetworkMgr::CreateNetworkThread( void )
{
	HANDLE hThread;		//< 스레드 핸들
	DWORD ThreadId;		//< 스레드 ID

	hThread = CreateThread( NULL, 0, ProcessReceive, (LPVOID)m_Sock, 9, &ThreadId );
	if( hThread == NULL )
	{
		MessageBox( g_hWnd, "스레드 생성 실패", "실패", MB_OK );
	}
	else
	{
		CloseHandle( hThread );
	}
}

DWORD WINAPI ProcessReceive( LPVOID arg ) //< 리시브 전용 스레드
{
	SOCKET sock = (SOCKET) arg;	//< 스레드 인자로 클라이언트 소켓을 전달받음
	std::queue<char> QueueBuf; //< 큐 버퍼
	PART part = PART_HEADER; //< 헤더부터 받아야함
	int nNeedSize = 1;	//< 헤더부터 받아야함
	int retval;

	//< 서버와 데이터 통신
	while( true )
	{
		//< 데이터 받기
		retval = NetworkMgr.PacketToken( sock, &QueueBuf, RECVBUFSIZE, 0, part, nNeedSize );
		if( retval == SOCKET_ERROR )
		{
			NetworkMgr.err_display( "recv()" );
			break;
		}
		else if( retval == 0 )
		{
			break;
		}
	}

	//< 클라이언트와 통신이 끝나면 클라이언트 소켓 닫기
	closesocket( sock );

	return 0;
}


void cNetworkMgr::CloseSocket( void )
{
	closesocket(m_Sock);
}

cNetworkMgr& cNetworkMgr::GetInstance()
{
	static cNetworkMgr instance;
	return instance;
}

void cNetworkMgr::err_quit( char* msg )
{
	LPVOID szMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&szMsgBuf, 0, NULL );
	assert( false && szMsgBuf );
	LocalFree( szMsgBuf );
}

void cNetworkMgr::err_display( char* msg )
{
	LPVOID szMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&szMsgBuf, 0, NULL );
	OutputDebugString( msg );
	OutputDebugString( ":" );
	OutputDebugString( (LPSTR)szMsgBuf ); 
	LocalFree( szMsgBuf );
}

int cNetworkMgr::PacketToken( SOCKET s, std::queue<char>* QueueBuf, int len, int flags, PART& part, int& nNeedSize  )
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
		QueueBuf->push( szReceive[i] ); //< 받은만큼 큐에 삽입
	}

	while( !QueueBuf->empty() ) //< 큐가 빌때까지 데이터 처리
	{
		switch( part )
		{
		case PART_HEADER :
			{
				nNeedSize = QueueBuf->front();
				QueueBuf->pop();
				part = PART_LENGTH;
			}
		case PART_LENGTH :
			{
				nLeft = nNeedSize; //< 이전에 남은 양부터
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf->front();
					QueueBuf->pop(); 
					nLeft = nLeft - 1; 
					if( QueueBuf->empty() ) //< 모자라면 남은 양 저장하고 더 받으러 리턴
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
					szPacketToken[nLast++] = QueueBuf->front();
					QueueBuf->pop(); 
					nLeft = nLeft - 1; 
					if( nLeft == 0 ) break; //< 더이상 받아야할게 없다면
					if( QueueBuf->empty() ) //< 모자라면 남은 양 저장하고 더 받으러 리턴	
					{
						nNeedSize = nLeft;	
						if( nLeft != 0 ) return received; //< 모자라면 리턴, 딱맞으면 패스
					}
				}

				//< 받은 DATA 에 대한 처리 	ProcessData( Sender, PacketOption )
				ProcessData( Sender, PacketOption, szPacketToken, clientId );

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


void cNetworkMgr::SendToServer( stCharacterInfo chInfo, stTransformInfo trInfo, D3DXVECTOR3 vDrt, float fMoveSpeed, PLAYER_ANIMATION CurrentAnimation )
{
	//< 1초에 1번씩 보냄
	if( m_dwSendTime >= GetTickCount() ) return;
	m_dwSendTime = GetTickCount() + 500;//1000;

	char buf[SENDBUFSIZE + 1];
	int retval;
	
	//< 패킷 만들어서 서버로 보냄
	stClientInfoPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.clientInfo);
	packet.Length.Option = PO_TRANSFORM;
	packet.Length.Sender = SND_PLAYER;
	packet.Length.clientId = m_nClientID;
	packet.clientInfo.transformInfo = trInfo;
	packet.clientInfo.vDrt = vDrt;
	packet.clientInfo.nHP	=	(int)(chInfo.dwHP);
	packet.clientInfo.fSpeed = fMoveSpeed;
	packet.clientInfo.CurrentAnimation = CurrentAnimation;
	packet.clientInfo.nID = m_nClientID;
	packet.clientInfo.useWeapon = m_pPlayer->GetCharacterInfo()->m_UseWeapon;
	packet.clientInfo.sock = m_Sock;
	memcpy( buf, &packet, sizeof(stClientInfoPacket) );

	//< 서버로 데이터 보내기(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
	char szDbgMsg[1024];
	wsprintf( szDbgMsg, "%d", &retval );
	strcat( szDbgMsg, "바이트 서버로 전송" ); 
	OutputDebugString( szDbgMsg );
}

void cNetworkMgr::SendToServer( cEnemyCharacter* pCharacter, stTransformInfo Info )
{
	char buf[SENDBUFSIZE + 1];
	int retval;

	//< 패킷 만들어서 서버로 보냄
	stTransformPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.Info);
	packet.Length.Option = PO_TRANSFORM;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = m_nClientID;
	packet.Info = Info;

	memcpy( buf, &packet, sizeof(stTransformPacket) ); 

	//< 서버로 데이터 보내기(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
	char szDbgMsg[1024];
	wsprintf( szDbgMsg, "%d", &retval );
	strcat( szDbgMsg, "바이트 서버로 전송" ); 
	OutputDebugString( szDbgMsg );
}

void cNetworkMgr::SendToServer( IN int nMonsterIndex, IN int nHpReduce )
{
	char buf[SENDBUFSIZE + 1];
	int retval;

	//< 패킷 만들어서 서버로 보냄
	stHpReduceInfoPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.ReduceInfo);
	packet.Length.Option = PO_HPREDUCE;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = m_nClientID; //< 언놈이 깎았나
	packet.ReduceInfo.nMonsterIndex = nMonsterIndex;
	packet.ReduceInfo.nHpReduce = nHpReduce;
	memcpy( buf, &packet, sizeof(stHpReduceInfoPacket) ); 

	//< 서버로 데이터 보내기(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
}

void cNetworkMgr::ProcessData( SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID )
{
	//< 발신 주체와 패킷의 종류에 따라 어떤 프로세스로 넘길것인지 정해짐
	switch( PacketOption )
	{
	case PO_GENERATE:
		{
			if( Sender == SND_MANAGER ) { GenerateClient( nClientID ); }
		}
		break;
	case PO_DESTROY:
		{
			if( Sender == SND_MANAGER ) { DeleteClient( nClientID ); }
		}
		break;
	case PO_TRANSFORM:
		{	
			if( Sender == SND_PLAYER ) { ProcessOtherClientTransform( Buf ); }
			else if( Sender == SND_ENEMY ) { ProcessEnemy( Buf, nClientID ); }
		}
		break;
	case PO_HPREDUCE:
		{
			if( Sender == SND_ENEMY ) { ProcessHpReduce( nClientID ); }
		}
		break;
	}
}

void cNetworkMgr::GenerateClient( int nClientID )
{
	if( m_nClientID == nClientID ) return;

	m_Client[nClientID].bAlive = true;
	m_Client[nClientID].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex와 일치
	m_Client[nClientID].useWeapon = WEAPON_TWOHAND; //<임시로 디폴트
}

void cNetworkMgr::DeleteClient( int nClientID )
{
	if( m_nClientID == nClientID ) return;

	//< 맵에서 해당 클라이언트 제거
	int nDeleteClientIndex;
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();
	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->second.nID == nClientID )
		{
			nDeleteClientIndex = iter->first;
			m_Client[nDeleteClientIndex].bAlive = false;
			break;
		}
	}
}

void cNetworkMgr::ProcessOtherClientTransform( char* Buf )
{
	stClientInfo Info;
	memcpy( &Info, Buf, sizeof(stClientInfo) );

	m_Client[Info.nID].nID = Info.nID;
	m_Client[Info.nID].nHP =Info.nHP;
	m_Client[Info.nID].sock = Info.sock;
	m_Client[Info.nID].transformInfo = Info.transformInfo;
	m_Client[Info.nID].vDrt = Info.vDrt;
	m_Client[Info.nID].fSpeed = Info.fSpeed;
	m_Client[Info.nID].useWeapon = Info.useWeapon;
	if( m_Client[Info.nID].bAlive == false )
	{
		m_Client[Info.nID].bAlive = true;
		m_Client[Info.nID].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex와 일치
		m_Client[Info.nID].useWeapon = WEAPON_TWOHAND; //<임시로 디폴트
	}

	//뒷 모션과 비교 , 같은 모션이였으면 넘어간다.
	if( m_Client[Info.nID].CurrentAnimation != Info.CurrentAnimation )
	{
		m_Client[Info.nID].CurrentAnimation = Info.CurrentAnimation;
//		m_Client[Info.nID].pModel->SetAnimationIndexBelnd( m_Client[Info.nID].CurrentAnimation, true, 0.4f );
		m_Client[Info.nID].pModel->SetAnimationIndexBelnd( m_Client[Info.nID].CurrentAnimation, false );
		m_Client[Info.nID].pModel->SetAnimationIndex( m_Client[Info.nID].CurrentAnimation, false );
	}
}

void cNetworkMgr::ProcessEnemy( char* Buf, int nClientID )
{
	stEnemyTransportInfo Info;
	memcpy( &Info, Buf, sizeof(Info) );


	m_pEnemy[Info.nEnemyIndex].GetCharacterInfo()->bAlive	=	Info.bAlive;
	m_pEnemy[Info.nEnemyIndex].GetMonsterInfo()->nAniIndex = Info.nAnimationIndex;
	m_pEnemy[Info.nEnemyIndex].GetCharacterInfo()->dwHP = m_pEnemy[Info.nEnemyIndex].GetCharacterInfo()->dwMaxHP - Info.nHpReduce;
	m_pEnemy[Info.nEnemyIndex].GetTransformInfo()->vPos = Info.vPos;
	m_pEnemy[Info.nEnemyIndex].GetMonsterInfo()->vTargetPos = Info.vTarget;
	m_pEnemy[Info.nEnemyIndex].GetMonsterInfo()->dwUpdateTime = GetTickCount();
}

void cNetworkMgr::MoveOtherClient( void )
{
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == this->m_nClientID ) continue;	 //< 자기자신은 패스

		if( iter->second.bAlive == false ) continue; 

		iter->second.transformInfo.vPos += iter->second.vDrt * iter->second.fSpeed;
		iter->second.pModel->SetPlaySpeed(0.64f); 
//		iter->second.pModel->PlayFrame(cGameTime::Instance().Get_ElapsedTime() / 2000.0f); //< 없애봄
	}
}

void cNetworkMgr::RenderOtherClient( void )
{
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == this->m_nClientID ) continue;	 //< 자기자신은 패스

		if( iter->second.bAlive == false ) continue; 

		D3DXMATRIXA16 matWorld, matPos, matScale, matRotX, matRotY, matRotZ ;

		D3DXMatrixScaling(&matScale,iter->second.transformInfo.vScale.x, 
									iter->second.transformInfo.vScale.y,
									iter->second.transformInfo.vScale.z);

		D3DXMatrixTranslation(&matPos,iter->second.transformInfo.vPos.x, 
									  iter->second.transformInfo.vPos.y, 
									  iter->second.transformInfo.vPos.z);

		D3DXMatrixRotationX(&matRotX, iter->second.transformInfo.vRot.x );
		D3DXMatrixRotationY(&matRotY, iter->second.transformInfo.vRot.y );
		D3DXMatrixRotationZ(&matRotZ, iter->second.transformInfo.vRot.z );

		matWorld = matScale  * (matRotX * matRotY * matRotZ) * matPos;

		sgDevice->SetTransform(D3DTS_WORLD , &matWorld);
		iter->second.pModel->Render( matWorld, cGameTime::Instance().Get_ElapsedTime()/1000.0f );
//		m_pSkinnedMesh->Render( sgDevice, 0 );

		//< 손에 쥐고 있는 무기 그리기
		if( iter->second.useWeapon < 0 || iter->second.useWeapon > WEAPON_END ) continue;

		D3DXMATRIXA16 matWeaponTM;
		LPD3DXFRAME pHand = iter->second.pModel->GetFindBone(iter->second.pModel->GetBone(), "WEAPON" );
		if( pHand == NULL)
		{
			MessageBox(g_hWnd , "손 뼈 못찾음" ,  0 , 0 );
		}

		BaseD3D::BONE* b = (BaseD3D::BONE*)pHand;

		matWeaponTM =  b->TransformationMatrix;

		sgDevice->SetTransform(D3DTS_WORLD , &matWeaponTM);
		sgDevice->SetTexture( 0, m_Weapon[iter->second.useWeapon]->m_pTex );
		sgDevice->SetMaterial( &m_Weapon[iter->second.useWeapon]->m_Mtrl );
		sgDevice->SetStreamSource( 0, m_Weapon[iter->second.useWeapon]->m_pVB, 0, sizeof(stObjVertex) );
		sgDevice->SetFVF( OBJ_D3DFVF );
		sgDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_Weapon[iter->second.useWeapon]->m_Vertices.size()/3 );
	}
}

void cNetworkMgr::GetWeaponMeshPtr( std::vector<cObjFileMesh*>& pMesh )
{
	m_Weapon = pMesh;
}

void cNetworkMgr::ProcessHpReduce( int nHp )
{
	/* HP를 깎든, HP를 변경시키든*/
	m_pPlayer->GetCharacterInfo()->dwHP -= nHp;

	cCharacterDamage::GetInstance().SetDamage((DWORD)nHp);

	cSoundMgr::GetInstance()->SoundPlay(SOUND_D_ATTACK);
}
