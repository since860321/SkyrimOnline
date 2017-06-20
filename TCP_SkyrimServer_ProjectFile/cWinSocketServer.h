#pragma once

//< 싱글턴으로 구현한 서버클래스 (형석)

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

	//< 배열의 인덱스는 클라이언트에게 부여되는 ID. 현재 사용되는 ID에 해당하는 bool 값은 true.
	bool				m_bUsed[MAXCLIENT];	

	void Init( void );			//< 윈속 초기화
	void Cleanup( void );	//< 윈속 종료

	void CreateListenSocket( void );		//< Listen Socket 생성
	void Bind( void );							//< 소켓 주소 구조체와 바인딩
	void Listen( void );
	void AcceptClient( void );				//< 클라이언트 접속을 받아서 쓰레드 생성
	void CloseSocket( void );

	void SendInitiateInfo( IN SOCKET sock, OUT DWORD* clientID );
	void SendEnemyInitiateInfo( IN SOCKET sock );

	int recvn( SOCKET s, char* buf, int len, int flags );
	void DeleteClientProcess( IN SOCKET deleteClientSock );

	//< 에러 출력 함수
	void err_quit( char* msg );		//< 에러 메시지 출력 후 프로그램 종료
	void err_display( char* msg );	//< 에러 메시지만 콘솔창에 출력

	int PacketToken( SOCKET s, std::queue<char>& QueueBuf, int len, int flags, PART& part, int& nNeedSize ); //< 패킷 자르기
	void ProcessData( SOCKET s, SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< 클라이언트로부터 받은 데이터 처리
	void ProcessPlayerTransform( SOCKET s, char* Buf, int nClientID );	//< 플레이어 변환에 관한 처리
	void ProcessEnemyTransform( SOCKET s, char* Buf, int nClientID );	//< 적의 변환에 관한 처리
	void ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID );		//< 적의 HP 감소에 관한 처리

	//< properties
	inline SOCKET* GetListenSocketPtr( void ) { return &m_ListenSock; }
	void SendToServer( stEnemyTransportInfo Info ); //< 매니저에게 구조체 전달
	void SendToServer( IN int nClientIndex, IN int nClientHp ); //< 매니저에게 클라이언트 HP 정보 전달
};

