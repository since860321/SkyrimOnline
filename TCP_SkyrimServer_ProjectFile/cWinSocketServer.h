//#ifdef _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
//#else //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
#pragma once 
//#endif //_S_MOD_REMOVE_PRAGMA_ONCE_WARNING_
//< ½Ì±ÛÅÏÀ¸·Î ±¸ÇöÇÑ ¼­¹öÅ¬·¡½º (Çü¼®)

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

	//< ¿¿¿ ¿¿¿¿ ¿¿¿¿¿¿¿ ¿¿¿¿ ID. ¿¿ ¿¿¿¿ ID¿ ¿¿¿¿ bool ¿¿ true.
	bool				m_bUsed[MAXCLIENT];	

	void Init( void );			//< ¿¿ ¿¿¿
	void Cleanup( void );	//< ¿¿ ¿¿

	void CreateListenSocket( void );		//< Listen Socket ¿¿
	void Bind( void );							//< ¿¿ ¿¿ ¿¿¿¿ ¿¿¿
	void Listen( void );
	void AcceptClient( void );				//< ¿¿¿¿¿ ¿¿¿ ¿¿¿ ¿¿¿ ¿¿
	void CloseSocket( void );

	void SendInitiateInfo(SOCKET sock, DWORD* clientID );
	void SendEnemyInitiateInfo(SOCKET sock );

	int recvn( SOCKET s, char* buf, int len, int flags );
	void DeleteClientProcess(SOCKET deleteClientSock );

	//< ¿¿ ¿¿ ¿¿
	void err_quit( char* msg );		//< ¿¿ ¿¿¿ ¿¿ ¿ ¿¿¿¿ ¿¿
	void err_display( char* msg );	//< ¿¿ ¿¿¿¿ ¿¿¿¿ ¿¿

	int PacketToken( SOCKET s, std::queue<char>& QueueBuf, int len, int flags, PART& part, int& nNeedSize ); //< ¿¿ ¿¿¿
	void ProcessData( SOCKET s, SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< ¿¿¿¿¿¿¿¿ ¿¿ ¿¿¿ ¿¿
	void ProcessPlayerTransform( SOCKET s, char* Buf, int nClientID );	//< ¿¿¿¿ ¿¿¿ ¿¿ ¿¿
	void ProcessEnemyTransform( SOCKET s, char* Buf, int nClientID );	//< ¿¿ ¿¿¿ ¿¿ ¿¿
	void ProcessEnemyHpReduce( SOCKET s, char* Buf, int nClientID );		//< ¿¿ HP ¿¿¿ ¿¿ ¿¿

	//< properties
	inline SOCKET* GetListenSocketPtr( void ) { return &m_ListenSock; }
	void SendToServer( stEnemyTransportInfo Info ); //< ¿¿¿¿¿ ¿¿¿ ¿¿
	void SendToServer(int nClientIndex, int nClientHp ); //< ¿¿¿¿¿ ¿¿¿¿¿ HP ¿¿ ¿¿
};
#endif //_S_LINUX_EPOLL_
