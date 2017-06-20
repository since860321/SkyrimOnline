#pragma once

//< 클라이언트와 서버의 통신에 필요한 정보들을 관리하는 싱글턴 클래스입니다. (형석)

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

	void Init( void );						//< 윈속 초기화
	void Cleanup( void );				//< 윈속 종료
	void CloseSocket( void );			//< 클라이언트 소켓 닫기

	cNetworkMgr(void);

public:
	virtual ~cNetworkMgr(void);
	static cNetworkMgr& GetInstance(); 

	void CreateSocket( void );				//< Socket 생성
	LRESULT ConnectServer( void );			//< 소켓 주소 구조체와 바인딩
	void CreateNetworkThread( void ); //< 데이터 통신용 네트워크 스레드 생성
	void SetReceiver( cPlayerCharacter* pPlayer, cEnemyCharacter* pEnemy );		//< 전송받는 객체 설정
	void ReceiveClientInformation( void ); //< 클라이언트 접속 정보 보냄
	int recvn( SOCKET s, char* buf, int len, int flags );
	
	void err_quit( char* msg );			//< 에러메시지 출력 후 프로그램 비정상 종료
	void err_display( char* msg );		//< 출력창에 에러메시지 출력
	int PacketToken( SOCKET s, std::queue<char>* QueueBuf, int len, int flags, PART& part, int& nNeedSize  );

	void ProcessData( SENDER Sender, PACKET_OPTION PacketOption, char* Buf, int nClientID );	//< 서버로부터 받은 데이터 처리
	void GenerateClient( int nClientID );					//< 클라이언트 데이터 생성
	void DeleteClient( int nClientID );						//< 클라이언트 데이터 삭제
	void ProcessTotalClient( int nTotalClient );			//< 현재 접속한 클라이언트 수 파악
	void ProcessOtherClientTransform( char* Buf );	//< 다른 클라이언트에 변환에 관한 처리
	void ProcessEnemy( char* Buf, int nClientID );						//< 몬스터에 대한 패킷 처리
	void ProcessPlayerStatus( char* Buf );			//< 플레이어 상태에 관한 처리
	void ProcessEnemyTransform( char* Buf );	//< 적의 변환에 관한 처리
	void ProcessEnemyStatus( char* Buf );			//< 적의 상태에 관한 처리
	void ProcessHpReduce( int nHp );				//< 플레이어 HP 변화에 관한 처리

	void MoveOtherClient( void );
	void RenderOtherClient( void );
	void UpdateEnemy( void );
	void GetWeaponMeshPtr( std::vector<cObjFileMesh*>& pMesh );

	//< 인터페이스
	void SendToServer( stCharacterInfo chInfo, stTransformInfo trInfo, D3DXVECTOR3 vDrt, float fMoveSpeed, PLAYER_ANIMATION CurrentAnimation ); //< 매니저에게 구조체 전달 (어떤 클래스가, 어떤 처리를, 데이터)
	void SendToServer( cEnemyCharacter* pCharacter, stTransformInfo Info ); //< 매니저에게 구조체 전달 (어떤 클래스가, 어떤 처리를, 데이터)
	void SendToServer( IN int nMonsterIndex, IN int nHpReduce );	//< 해당 몬스터의 HP 감소치를 서버로 전달하는 함수
};

