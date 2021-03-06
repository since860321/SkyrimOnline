//#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
//#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
//#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
//< 싱글턴으로 구현한 서버클래스 (형석)

#include "defines.h"

#ifdef _S_LINUX_EPOLL_
class cEpollSocketServer
{
	cEpollSocketServer(void);
	~cEpollSocketServer(void);

	unsigned int m_dwSendTime;

        std::map<int, stClientInfo> m_Client;
        stEnemyInfo* m_pEnemy;
        bool m_bUsed[MAXCLIENT];

public:
	static cEpollSocketServer& GetInstance();

	void SendInitiateInfo(int fd);
	void SendEnemyInitiateInfo(int fd);
};
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

	//< 옜� 옜옜 옜옜옜� 옜옜 ID. 옜 옜옜 ID� 옜옜 bool 옜 true.
	bool				m_bUsed[MAXCLIENT];	

	void Init( void );			//< 옜 옜�
	void Cleanup( void );	//< 옜 옜

	void CreateListenSocket( void );		//< Listen Socket 옜
	void Bind( void );							//< 옜 옜 옜옜 옜�
	void Listen( void );
	void AcceptClient( void );				//< 옜옜� 옜� 옜� 옜� 옜
	void CloseSocket( void );

	void SendInitiateInfo(SOCKET sock, DWORD* clientID );
	void SendEnemyInitiateInfo(SOCKET sock );

	int recvn( SOCKET s, char* buf, int len, int flags );
	void DeleteClientProcess(SOCKET deleteClientSock );

	//< 옜 옜 옜
	void err_quit( char* msg );		//< 옜 옜� 옜 � 옜옜 옜
	void err_display( char* msg );	//< 옜 옜옜 옜옜 옜

	int PacketToken( SOCKET s, std::queue<char>& QueueBuf, int len, int flags, PART& part, int& nNeedSize ); //< 옜 옜�
	void ProcessData( SOCKET s, SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< 옜옜옜옜 옜 옜� 옜
	void ProcessPlayerTransform( SOCKET s, char* Buf, int nClientID );	//< 옜옜 옜� 옜 옜
	void ProcessEnemyTransform( SOCKET s, char* Buf, int nClientID );	//< 옜 옜� 옜 옜
	void ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID );		//< 옜 HP 옜� 옜 옜

	//< properties
	inline SOCKET* GetListenSocketPtr( void ) { return &m_ListenSock; }
	void SendToServer( stEnemyTransportInfo Info ); //< 옜옜� 옜� 옜
	void SendToServer(int nClientIndex, int nClientHp ); //< 옜옜� 옜옜� HP 옜 옜
};
#endif //_S_LINUX_EPOLL_
