#pragma once

//< Ŭ���̾�Ʈ�� ������ ��ſ� �ʿ��� �������� �����ϴ� �̱��� Ŭ�����Դϴ�. (����)

namespace BaseD3D
{
	class BaseSkinnedMesh;
};
class cPlayerCharacter;
class cEnemyCharacter;
class cNetworkMgr
{
	int					m_nClientID;
	std::map<int, stClientInfo> m_Client;

	WSADATA		m_Wsa;
	SOCKET			m_Sock;

	cPlayerCharacter* m_pPlayer;
	cEnemyCharacter* m_pEnemy;

	BaseD3D::BaseSkinnedMesh* m_pSkinnedMesh;
	std::vector<cObjFileMesh*>		m_Weapon;

	DWORD			m_dwSendTime;

	void Init( void );						//< ���� �ʱ�ȭ
	void Cleanup( void );				//< ���� ����
	void CloseSocket( void );			//< Ŭ���̾�Ʈ ���� �ݱ�

	cNetworkMgr(void);

public:
	virtual ~cNetworkMgr(void);
	static cNetworkMgr& GetInstance(); 

	void CreateSocket( void );				//< Socket ����
	LRESULT ConnectServer( void );			//< ���� �ּ� ����ü�� ���ε�
	void CreateNetworkThread( void ); //< ������ ��ſ� ��Ʈ��ũ ������ ����
	void SetReceiver( cPlayerCharacter* pPlayer, cEnemyCharacter* pEnemy );		//< ���۹޴� ��ü ����
	void ReceiveClientInformation( void ); //< Ŭ���̾�Ʈ ���� ���� ����
	int recvn( SOCKET s, char* buf, int len, int flags );
	
	void err_quit( char* msg );			//< �����޽��� ��� �� ���α׷� ������ ����
	void err_display( char* msg );		//< ���â�� �����޽��� ���
	int PacketToken( SOCKET s, std::queue<char>* QueueBuf, int len, int flags, PART& part, int& nNeedSize  );

	void ProcessData( SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< �����κ��� ���� ������ ó��
	void GenerateClient( int nClientID );					//< Ŭ���̾�Ʈ ������ ����
	void DeleteClient( int nClientID );						//< Ŭ���̾�Ʈ ������ ����
	void ProcessTotalClient( int nTotalClient );			//< ���� ������ Ŭ���̾�Ʈ �� �ľ�
	void ProcessOtherClientTransform( char* Buf );	//< �ٸ� Ŭ���̾�Ʈ�� ��ȯ�� ���� ó��
	void ProcessEnemy( char* Buf, int nClientID );						//< ���Ϳ� ���� ��Ŷ ó��
	void ProcessPlayerStatus( char* Buf );			//< �÷��̾� ���¿� ���� ó��
	void ProcessEnemyTransform( char* Buf );	//< ���� ��ȯ�� ���� ó��
	void ProcessEnemyStatus( char* Buf );			//< ���� ���¿� ���� ó��
	void ProcessHpReduce( int nHp );				//< �÷��̾� HP ��ȭ�� ���� ó��

	void MoveOtherClient( void );
	void RenderOtherClient( void );
	void UpdateEnemy( void );
	void GetWeaponMeshPtr( std::vector<cObjFileMesh*>& pMesh );

	//< �������̽�
	void SendToServer( stCharacterInfo chInfo, stTransformInfo trInfo, D3DXVECTOR3 vDrt, float fMoveSpeed, PLAYER_ANIMATION CurrentAnimation ); //< �Ŵ������� ����ü ���� (� Ŭ������, � ó����, ������)
	void SendToServer( cEnemyCharacter* pCharacter, stTransformInfo Info ); //< �Ŵ������� ����ü ���� (� Ŭ������, � ó����, ������)
	void SendToServer( IN int nMonsterIndex, IN int nHpReduce );	//< �ش� ������ HP ����ġ�� ������ �����ϴ� �Լ�
};

