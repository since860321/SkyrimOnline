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

//< 클라이언트에 대한 모든 정보가 들어있는 구조체(서버에서 맵으로 관리)
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