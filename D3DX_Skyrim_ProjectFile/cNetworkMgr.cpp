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
	CloseSocket();	//< ���� �ݱ�
	Cleanup();		//< ���� ����

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
		assert( false && "���� �ʱ�ȭ ����" );
	}

	for( size_t i = 0 ; i < MAXCLIENT ; ++ i )
	{
		m_Client[i]; //< 10���� ������ �̸� �Ҵ�
		memset( &m_Client[i], 0, sizeof(m_Client[i]) );
		m_Client[i].bAlive = false;
		m_Client[i].pModel = new BaseD3D::BaseSkinnedMesh();
		m_Client[i].pModel->Init( sgDevice, "./Resources/xfile/character_test01.X", NULL );
		m_Client[i].pModel->SetAnimationIndex(1, false); 
		m_Client[i].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex�� ��ġ
		m_Client[i].useWeapon = WEAPON_TWOHAND; //<�ӽ÷� ����Ʈ
	}
}

void cNetworkMgr::Cleanup( void )
{
	WSACleanup();
}

void cNetworkMgr::CreateSocket( void )
{
	m_Sock = socket( AF_INET, SOCK_STREAM, 0 );	 //< TCP ���� ����
	if( m_Sock == INVALID_SOCKET ) err_quit( "socket()" );	//< ���� �޽��� �߻� �� ���α׷� ����
}

LRESULT cNetworkMgr::ConnectServer( void )
{
	int retval;

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons( 9000 );	//< �����Ϸ��� ������ ��Ʈ��ȣ
	serveraddr.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1");	//"192.168.11.53"//< ������ �ּ�( ������ Ŭ���̾�Ʈ�� ������ ��ǻ�� )
	retval = connect( m_Sock, (SOCKADDR*) &serveraddr, sizeof(serveraddr) );	//< Ŭ���̾�Ʈ�� IP�ּҿ� ��Ʈ��ȣ�� ���������� ���Ͽ� �Ҵ�
	if( retval == SOCKET_ERROR ) 
	{
		MessageBox( g_hWnd, "���� ���� ����", "����", MB_OK );
		return E_FAIL;
	}

	//< ���� ������ ���� ID �Ѱ��ְ� �ʱ����� ����
	ReceiveClientInformation();

	return S_OK;
}

void cNetworkMgr::ReceiveClientInformation( void )
{
	char buf[SENDBUFSIZE + 1];

	int received = recvn( m_Sock, buf, sizeof(stConnectPacket), 0 );
	if( received == SOCKET_ERROR )
	{
		err_display("�ʱ������޾ƿ������");
		return ;
	}

	//< �����κ��� �ʱ����� �޾Ƽ� ����
	stConnectPacket packet;
	memcpy( &packet, buf, sizeof(packet) );
	m_nClientID = packet.ID;
	//���� = packet.sock
	//�ʱ���ġ = packet.transformInfo;
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
	HANDLE hThread;		//< ������ �ڵ�
	DWORD ThreadId;		//< ������ ID

	hThread = CreateThread( NULL, 0, ProcessReceive, (LPVOID)m_Sock, 9, &ThreadId );
	if( hThread == NULL )
	{
		MessageBox( g_hWnd, "������ ���� ����", "����", MB_OK );
	}
	else
	{
		CloseHandle( hThread );
	}
}

DWORD WINAPI ProcessReceive( LPVOID arg ) //< ���ú� ���� ������
{
	SOCKET sock = (SOCKET) arg;	//< ������ ���ڷ� Ŭ���̾�Ʈ ������ ���޹���
	std::queue<char> QueueBuf; //< ť ����
	PART part = PART_HEADER; //< ������� �޾ƾ���
	int nNeedSize = 1;	//< ������� �޾ƾ���
	int retval;

	//< ������ ������ ���
	while( true )
	{
		//< ������ �ޱ�
		retval = sgNetwork.PacketToken( sock, &QueueBuf, RECVBUFSIZE, 0, part, nNeedSize );
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

	//< Ŭ���̾�Ʈ�� ����� ������ Ŭ���̾�Ʈ ���� �ݱ�
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
	static int nLast = 0;	//< ��Ŷ ������ ���ڿ��� ������ �ε���
	static int clientId;

	char szReceive[1024] = {0};
	int received = recv( s, szReceive, len, flags ); 
	
	for( int i = 0; i < received ; i ++ )
	{
		QueueBuf->push( szReceive[i] ); //< ������ŭ ť�� ����
	}

	while( !QueueBuf->empty() ) //< ť�� �������� ������ ó��
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
				nLeft = nNeedSize; //< ������ ���� �����
				for( int i = 0 ; i < nNeedSize ; i ++ )
				{
					szPacketToken[nLast++] = QueueBuf->front();
					QueueBuf->pop(); 
					nLeft = nLeft - 1; 
					if( QueueBuf->empty() ) //< ���ڶ�� ���� �� �����ϰ� �� ������ ����
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
					szPacketToken[nLast++] = QueueBuf->front();
					QueueBuf->pop(); 
					nLeft = nLeft - 1; 
					if( nLeft == 0 ) break; //< ���̻� �޾ƾ��Ұ� ���ٸ�
					if( QueueBuf->empty() ) //< ���ڶ�� ���� �� �����ϰ� �� ������ ����	
					{
						nNeedSize = nLeft;	
						if( nLeft != 0 ) return received; //< ���ڶ�� ����, �������� �н�
					}
				}

				//< ���� DATA �� ���� ó�� 	ProcessData( Sender, PacketOption )
				ProcessData( Sender, PacketOption, szPacketToken, clientId );

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


void cNetworkMgr::SendToServer( stCharacterInfo chInfo, stTransformInfo trInfo, D3DXVECTOR3 vDrt, float fMoveSpeed, PLAYER_ANIMATION CurrentAnimation )
{
	//< 1�ʿ� 1���� ����
	if( m_dwSendTime >= GetTickCount() ) return;
	m_dwSendTime = GetTickCount() + 500;//1000;

	char buf[SENDBUFSIZE + 1];
	int retval;
	
	//< ��Ŷ ���� ������ ����
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

	//< ������ ������ ������(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
	char szDbgMsg[1024];
	wsprintf( szDbgMsg, "%d", &retval );
	strcat( szDbgMsg, "����Ʈ ������ ����" ); 
	OutputDebugString( szDbgMsg );
}

void cNetworkMgr::SendToServer( cEnemyCharacter* pCharacter, stTransformInfo Info )
{
	char buf[SENDBUFSIZE + 1];
	int retval;

	//< ��Ŷ ���� ������ ����
	stTransformPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.Info);
	packet.Length.Option = PO_TRANSFORM;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = m_nClientID;
	packet.Info = Info;

	memcpy( buf, &packet, sizeof(stTransformPacket) ); 

	//< ������ ������ ������(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
	char szDbgMsg[1024];
	wsprintf( szDbgMsg, "%d", &retval );
	strcat( szDbgMsg, "����Ʈ ������ ����" ); 
	OutputDebugString( szDbgMsg );
}

void cNetworkMgr::SendToServer( IN int nMonsterIndex, IN int nHpReduce )
{
	char buf[SENDBUFSIZE + 1];
	int retval;

	//< ��Ŷ ���� ������ ����
	stHpReduceInfoPacket packet;
	packet.header = sizeof(packet.Length);
	packet.Length.Length = sizeof(packet.ReduceInfo);
	packet.Length.Option = PO_HPREDUCE;
	packet.Length.Sender = SND_ENEMY;
	packet.Length.clientId = m_nClientID; //< ����� ��ҳ�
	packet.ReduceInfo.nMonsterIndex = nMonsterIndex;
	packet.ReduceInfo.nHpReduce = nHpReduce;
	memcpy( buf, &packet, sizeof(stHpReduceInfoPacket) ); 

	//< ������ ������ ������(send())
	retval = send( m_Sock, buf, sizeof(packet), 0 );
	if( retval == SOCKET_ERROR )
	{
		err_display( "send()" ); 
		return;
	}
}

void cNetworkMgr::ProcessData( SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID )
{
	//< �߽� ��ü�� ��Ŷ�� ������ ���� � ���μ����� �ѱ������ ������
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
	m_Client[nClientID].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex�� ��ġ
	m_Client[nClientID].useWeapon = WEAPON_TWOHAND; //<�ӽ÷� ����Ʈ
}

void cNetworkMgr::DeleteClient( int nClientID )
{
	if( m_nClientID == nClientID ) return;

	//< �ʿ��� �ش� Ŭ���̾�Ʈ ����
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
		m_Client[Info.nID].CurrentAnimation = PLAYER_ANIMATION(1); //< SetAnimationIndex�� ��ġ
		m_Client[Info.nID].useWeapon = WEAPON_TWOHAND; //<�ӽ÷� ����Ʈ
	}

	//�� ��ǰ� �� , ���� ����̿����� �Ѿ��.
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
		if( iter->first == this->m_nClientID ) continue;	 //< �ڱ��ڽ��� �н�

		if( iter->second.bAlive == false ) continue; 

		iter->second.transformInfo.vPos += iter->second.vDrt * iter->second.fSpeed;
		iter->second.pModel->SetPlaySpeed(0.64f); 
//		iter->second.pModel->PlayFrame(cGameTime::Instance().Get_ElapsedTime() / 2000.0f); //< ���ֺ�
	}
}

void cNetworkMgr::RenderOtherClient( void )
{
	std::map<int, stClientInfo>::iterator iter = m_Client.begin();

	for( iter = m_Client.begin() ; iter != m_Client.end(); iter ++ )
	{
		if( iter->first == this->m_nClientID ) continue;	 //< �ڱ��ڽ��� �н�

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

		//< �տ� ��� �ִ� ���� �׸���
		if( iter->second.useWeapon < 0 || iter->second.useWeapon > WEAPON_END ) continue;

		D3DXMATRIXA16 matWeaponTM;
		LPD3DXFRAME pHand = iter->second.pModel->GetFindBone(iter->second.pModel->GetBone(), "WEAPON" );
		if( pHand == NULL)
		{
			MessageBox(g_hWnd , "�� �� ��ã��" ,  0 , 0 );
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
	/* HP�� ���, HP�� �����Ű��*/
	m_pPlayer->GetCharacterInfo()->dwHP -= nHp;

	cCharacterDamage::GetInstance().SetDamage((DWORD)nHp);

	cSoundMgr::GetInstance()->SoundPlay(SOUND_D_ATTACK);
}
