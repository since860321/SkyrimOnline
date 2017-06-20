#pragma once
//< 구조체 정의부
class cTerrainObject;
class cObjFileMesh;
class cCamera;
class cCharacter;
class cObjFileMesh;


struct stIndex
{
	WORD w0;
	WORD w1;
	WORD w2;
};

struct stGridVertex
{
	D3DXVECTOR3 vPos;
	D3DCOLOR color;
};

struct stObjVertex
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
};

#pragma pack( 1 )
struct	stCharacterInfo
{
	bool	bAlive;

	DWORD		dwHP;
	DWORD		dwMaxHP;
	DWORD		dwAttackDamage;
	DWORD		dwRadius;
	
	float				fMoveSpeed;
	float				fRotSpeed;
	CHARACTERTYPE		CharacterType;

	PLAYER_ANIMATION		m_Animation;			//<< 그 방향을 나타내는 enum
	bool			m_bAlive;

	//<< 카메라 전달용
	D3DXVECTOR3		m_vLook;
	D3DXVECTOR3		m_vView;
	D3DXVECTOR3		m_vEye;
	float			m_fCamere_UpDownValue;
	bool			m_bMouseMove;

	//포커싱 목표물
	cCharacter*			m_pFocus;
	bool				m_bAtk;
	
	//죽는시간
	DWORD	dwDeadTime;

	std::vector<cObjFileMesh*>*				m_pWeapon;
	WEAPON_TYPE								m_UseWeapon;
};
#pragma pack()

struct	stKeyInfo                                               // 입력키정보 구조체 AI실형용생성 -PC 완성후 삭제요망- (백)
{
	int nFrontMove,nBackMove,nTurnLeft,nTurnRight;
};

//< 초기 접속 시 사용되는 패킷
#pragma pack( 1 )
struct stConnectPacket
{
	BYTE header; //< ( = sizeof(stConnectPacket.ID)
	DWORD ID;
};
#pragma pack()

//< 패킷 LENGTH 정보
#pragma pack( 1 )
struct stLength
{
	WORD Length; //< DATA 크기stLength
	WORD Option; //< 패킷속성
	WORD Sender; //< 보낸 클래스
	int		clientId;	//< 클라이언트 ID 
};
#pragma pack()

//< 변환 정보
#pragma pack( 1 )
struct stTransformInfo 
{
	D3DXVECTOR3	vScale;
	D3DXVECTOR3	vRot;
	D3DXVECTOR3	vPos;
};
#pragma pack()

//< 변환정보 패킷
#pragma pack( 1 )
struct stTransformPacket
{
	//< PART_HEADER 
	BYTE header; //< LENGTH 크기 ( = sizeof(packet.Length) + sizeof(packet.Option) + sizeof(packet.Sender) + sizeof(packet.clientId); )
	//< PART_LENGTH
	stLength Length;
	//< PART_DATA
	stTransformInfo Info;
};
#pragma pack()

//< 클라이언트에 대한 모든 정보가 들어있는 구조체(서버에서 맵으로 관리)
namespace BaseD3D
{
	class BaseSkinnedMesh;
};
#pragma pack(1)
struct stClientInfo 
{
	SOCKET sock;
	int		nID;
	int		nHP;	
	bool	bAlive;
	float	fSpeed;
	stTransformInfo transformInfo;
	D3DXVECTOR3 vDrt;
	PLAYER_ANIMATION CurrentAnimation;
	BaseD3D::BaseSkinnedMesh*	pModel; //< 전송에는 사용하지 않음
	WEAPON_TYPE useWeapon;
	stClientInfo()
		: pModel(NULL)
	{
	}
};
#pragma pack()

//< 클라이언트 정보 패킷
#pragma pack(1)
struct stClientInfoPacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length;
	//< PART_DATA
	stClientInfo clientInfo;
};

#pragma pack()

class cCharacter;

struct stMonsterInfo
{
	float fPerceptionLength;
	D3DXVECTOR3 vStartPos;
	D3DXVECTOR3 vTargetPos;
	int nAniIndex;
	DWORD	dwUpdateTime;
	DRAGON_ANIMATION	NowAni;
	int bMyIndex;
};

#pragma pack(1)
struct stEnemyTransportInfo //< 전송정보
{
	bool	bAlive;
	int nEnemyIndex;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vTarget;
	int nAnimationIndex;
	DWORD			nHpReduce;

};
#pragma pack()

#pragma pack(1)
struct stEnemyTransportInfoPacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length;
	//< PART_DATA
	stEnemyTransportInfo EnemyInfo;
};
#pragma pack()

struct stEnemyInfo
{
	int nEnemyIndex;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vTarget;
	int nAnimationIndex;
	bool bAlive;
	DWORD dwHp;
	DWORD dwHpMax;
	DWORD dwAttackDamage;
	D3DXVECTOR3 vStartPos;
};

#pragma pack(1)
struct stHpReduceInfo 
{
	int nMonsterIndex;
	int nHpReduce;
};
#pragma pack()

#pragma pack(1)
struct stHpReduceInfoPacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length;
	//< PART_DATA
	stHpReduceInfo ReduceInfo;
};
#pragma pack()

struct stInvenObjInfo
{
	cObjFileMesh*	ObjInfo;
	int				nAttack;
	int				nDefence;
	int				nweight;
	char*			nObjName;
	
	
};

struct Ray
{
	D3DXVECTOR3	origin;
	D3DXVECTOR3	direction;
};

struct stSphere
{
	D3DXVECTOR3 vPos;
	float		fRad;
};

struct stShowDamage
{
	DWORD	dwDamage;
	DWORD	dwTime;

};