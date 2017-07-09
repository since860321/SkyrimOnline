#pragma once
#include "defines.h"
//< ����ü ���Ǻ�

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

#pragma pack( 1 )
struct stGeneratePacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length; //< �ɼǿ� PO_GENERATE �ɼ��� ����, Ŭ���̾�Ʈ ID������ ������ Ŭ���̾�Ʈ ID�� ���´�.
};
#pragma pack()

#pragma pack( 1 )
struct stDeletePacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length; //< �ɼǿ� PO_DESTROY �ɼ��� ����, Ŭ���̾�Ʈ ID������ ������ Ŭ���̾�Ʈ ID�� ���´�.
};
#pragma pack()
#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
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

struct stEnemyInfo
{
	int				nEnemyIndex;
	CHARACTERTYPE	MonsterType;		// ��������
	D3DXVECTOR3		vPos;				// ������ġ
	D3DXVECTOR3		vTargetPos;			// ��ǥ����
	int				nAnimationIndex;
	bool			bAlive;	
	DWORD			dwHp;
	DWORD			dwHpReduce;
	DWORD			dwHpMax;
	DWORD			dwAttackDamage;
	D3DXVECTOR3		vStartPos;			// �ʱ�ȭ��ġ
	
	float			fMoveSpeed;			//	�̵��ӵ�
	float			fPerceptionLength;	// ������ �����Ÿ�
	stClientInfo*	pTarget;			// ���� ���
	float			fAttackLange;		// ���ݰŸ�
	DWORD			dwAttackCoolTime;	// ���� ��Ÿ��
	DWORD			dwAttackTime;		// ���� �ð�

	DWORD			dwRecoveryCoolTime;	// ȸ�� ��Ÿ��
	DWORD			dwRecoveryTime;		// ȸ�� �ð�

	DWORD			dwGenerationCoolTime;	// ���� ��Ÿ��
	DWORD			dwGenerationTime;		// ���� �ð�

};

#pragma pack(1)
struct stClientHpReducePacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length;
};
#pragma pack()
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
