#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_

//< �̱������� ������ ����Ŭ���� (����)
#ifdef _S_LINUX_EPOLL_
std::map<int, stClientInfo> m_Client;

void SendInitiateInfo(SOCKET sock, DWORD* clientID );
void SendEnemyInitiateInfo(int fd);
#else //_S_LINUX_EPOLL_
class cWinSocketServer
{
	WSADATA		m_Wsa;
	SOCKET			m_ListenSock;

	DWORD			m_dwSendTime;

	cWinSocketServer(void);
public:
	~cWinSocketServer(void);
	static cWinSocketServer& GetInstance();

	std::map<int, stClientInfo> m_Client;
	stEnemyInfo*					  m_pEnemy;

	//< �迭�� �ε����� Ŭ���̾�Ʈ���� �ο��Ǵ� ID. ���� ���Ǵ� ID�� �ش��ϴ� bool ���� true.
	bool				m_bUsed[MAXCLIENT];	

	void Init( void );			//< ���� �ʱ�ȭ
	void Cleanup( void );	//< ���� ����

	void CreateListenSocket( void );		//< Listen Socket ����
	void Bind( void );							//< ���� �ּ� ����ü�� ���ε�
	void Listen( void );
	void AcceptClient( void );				//< Ŭ���̾�Ʈ ������ �޾Ƽ� ������ ����
	void CloseSocket( void );

	void SendInitiateInfo(SOCKET sock, DWORD* clientID );
	void SendEnemyInitiateInfo(SOCKET sock );

	int recvn( SOCKET s, char* buf, int len, int flags );
	void DeleteClientProcess(SOCKET deleteClientSock );

	//< ���� ��� �Լ�
	void err_quit( char* msg );		//< ���� �޽��� ��� �� ���α׷� ����
	void err_display( char* msg );	//< ���� �޽����� �ܼ�â�� ���

	int PacketToken( SOCKET s, std::queue<char>& QueueBuf, int len, int flags, PART& part, int& nNeedSize ); //< ��Ŷ �ڸ���
	void ProcessData( SOCKET s, SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< Ŭ���̾�Ʈ�κ��� ���� ������ ó��
	void ProcessPlayerTransform( SOCKET s, char* Buf, int nClientID );	//< �÷��̾� ��ȯ�� ���� ó��
	void ProcessEnemyTransform( SOCKET s, char* Buf, int nClientID );	//< ���� ��ȯ�� ���� ó��
	void ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID );		//< ���� HP ���ҿ� ���� ó��

	//< properties
	inline SOCKET* GetListenSocketPtr( void ) { return &m_ListenSock; }
	void SendToServer( stEnemyTransportInfo Info ); //< �Ŵ������� ����ü ����
	void SendToServer(int nClientIndex, int nClientHp ); //< �Ŵ������� Ŭ���̾�Ʈ HP ���� ����
};
#endif //_S_LINUX_EPOLL_
