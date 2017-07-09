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

//< Ŭ���̾�Ʈ�� ���� ��� ������ ����ִ� ����ü(�������� ������ ����)
namespace BaseD3D
{
	class BaseSkinnedMesh;
};

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