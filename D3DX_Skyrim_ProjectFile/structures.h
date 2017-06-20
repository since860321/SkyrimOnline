#pragma once
//< ����ü ���Ǻ�
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

	PLAYER_ANIMATION		m_Animation;			//<< �� ������ ��Ÿ���� enum
	bool			m_bAlive;

	//<< ī�޶� ���޿�
	D3DXVECTOR3		m_vLook;
	D3DXVECTOR3		m_vView;
	D3DXVECTOR3		m_vEye;
	float			m_fCamere_UpDownValue;
	bool			m_bMouseMove;

	//��Ŀ�� ��ǥ��
	cCharacter*			m_pFocus;
	bool				m_bAtk;
	
	//�״½ð�
	DWORD	dwDeadTime;

	std::vector<cObjFileMesh*>*				m_pWeapon;
	WEAPON_TYPE								m_UseWeapon;
};
#pragma pack()

struct	stKeyInfo                                               // �Է�Ű���� ����ü AI��������� -PC �ϼ��� �������- (��)
{
	int nFrontMove,nBackMove,nTurnLeft,nTurnRight;
};

//< �ʱ� ���� �� ���Ǵ� ��Ŷ
#pragma pack( 1 )
struct stConnectPacket
{
	BYTE header; //< ( = sizeof(stConnectPacket.ID)
	DWORD ID;
};
#pragma pack()

//< ��Ŷ LENGTH ����
#pragma pack( 1 )
struct stLength
{
	WORD Length; //< DATA ũ��stLength
	WORD Option; //< ��Ŷ�Ӽ�
	WORD Sender; //< ���� Ŭ����
	int		clientId;	//< Ŭ���̾�Ʈ ID 
};
#pragma pack()

//< ��ȯ ����
#pragma pack( 1 )
struct stTransformInfo 
{
	D3DXVECTOR3	vScale;
	D3DXVECTOR3	vRot;
	D3DXVECTOR3	vPos;
};
#pragma pack()

//< ��ȯ���� ��Ŷ
#pragma pack( 1 )
struct stTransformPacket
{
	//< PART_HEADER 
	BYTE header; //< LENGTH ũ�� ( = sizeof(packet.Length) + sizeof(packet.Option) + sizeof(packet.Sender) + sizeof(packet.clientId); )
	//< PART_LENGTH
	stLength Length;
	//< PART_DATA
	stTransformInfo Info;
};
#pragma pack()

//< Ŭ���̾�Ʈ�� ���� ��� ������ ����ִ� ����ü(�������� ������ ����)
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
	BaseD3D::BaseSkinnedMesh*	pModel; //< ���ۿ��� ������� ����
	WEAPON_TYPE useWeapon;
	stClientInfo()
		: pModel(NULL)
	{
	}
};
#pragma pack()

//< Ŭ���̾�Ʈ ���� ��Ŷ
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
struct stEnemyTransportInfo //< ��������
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