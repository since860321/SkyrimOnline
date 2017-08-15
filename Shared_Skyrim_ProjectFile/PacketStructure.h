#pragma once
#include "../TCP_SkyrimServer_ProjectFile/defines.h"
#include "CSCommonConst.h"
//< ����ü ���Ǻ�

//< �ʱ� ���� �� ���Ǵ� ��Ŷ
#pragma pack( 1 )
struct stConnectPacket
{
	unsigned char	header; //< ( = sizeof(stConnectPacket.ID)
	unsigned int	ID;
};
#pragma pack()

//< ��Ŷ LENGTH ����
#pragma pack( 1 )
struct stLength
{
	unsigned short Length; //< DATA ũ��stLength
	unsigned short Option; //< ��Ŷ�Ӽ�
	unsigned short Sender; //< ���� Ŭ����
	int		clientId;	//< Ŭ���̾�Ʈ ID 
};
#pragma pack()

//< ��ȯ ����
#pragma pack( 1 )
struct stTransformInfo 
{
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector	vScale;
	vector	vRot;
	vector	vPos;
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3	vScale;
	D3DXVECTOR3	vRot;
	D3DXVECTOR3	vPos;
#endif //_S_MOD_D3DX9_API_CUSTOM_
};
#pragma pack()

//< ��ȯ���� ��Ŷ
#pragma pack( 1 )
struct stTransformPacket
{
	//< PART_HEADER 
	unsigned char header; //< LENGTH ũ�� ( = sizeof(packet.Length) + sizeof(packet.Option) + sizeof(packet.Sender) + sizeof(packet.clientId); )
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
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
	SOCKET sock;
#endif //_S_LINUX_EPOLL_
	int		nID;
	int		nHP;	
	bool	bAlive;
	float	fSpeed;
	stTransformInfo transformInfo;
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector vDrt;
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vDrt;
#endif //_S_MOD_D3DX9_API_CUSTOM_
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
	unsigned char header; 
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
	unsigned char header; 
	//< PART_LENGTH
	stLength Length; //< �ɼǿ� PO_GENERATE �ɼ��� ����, Ŭ���̾�Ʈ ID������ ������ Ŭ���̾�Ʈ ID�� ���´�.
};
#pragma pack()

#pragma pack( 1 )
struct stDeletePacket
{
	//< PART_HEADER 
	unsigned char header; 
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
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector vPos;
	vector vTarget;
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vTarget;
#endif //_S_MOD_D3DX9_API_CUSTOM_
	int nAnimationIndex;
	unsigned int nHpReduce;
};

#pragma pack()

#pragma pack(1)
struct stEnemyTransportInfoPacket
{
	//< PART_HEADER 
	unsigned char header; 
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
	unsigned char header; 
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
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector			vPos;				// ������ġ
	vector			vTargetPos;			// ��ǥ����
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3		vPos;				// ������ġ
	D3DXVECTOR3		vTargetPos;			// ��ǥ����
#endif //_S_MOD_D3DX9_API_CUSTOM_
	int				nAnimationIndex;
	bool			bAlive;	
	unsigned int	dwHp;
	unsigned int	dwHpReduce;
	unsigned int	dwHpMax;
	unsigned int	dwAttackDamage;
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector			vStartPos;			// �ʱ�ȭ��ġ
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3		vStartPos;			// �ʱ�ȭ��ġ
#endif //_S_MOD_D3DX9_API_CUSTOM_

	float			fMoveSpeed;			//	�̵��ӵ�
	float			fPerceptionLength;	// ������ �����Ÿ�
	stClientInfo*	pTarget;			// ���� ���
	float			fAttackLange;		// ���ݰŸ�
	unsigned int	dwAttackCoolTime;	// ���� ��Ÿ��
	unsigned int	dwAttackTime;		// ���� �ð�

	unsigned int	dwRecoveryCoolTime;	// ȸ�� ��Ÿ��
	unsigned int	dwRecoveryTime;		// ȸ�� �ð�

	unsigned int	dwGenerationCoolTime;	// ���� ��Ÿ��
	unsigned int	dwGenerationTime;		// ���� �ð�

};

#pragma pack(1)
struct stClientHpReducePacket
{
	//< PART_HEADER 
	unsigned char header; 
	//< PART_LENGTH
	stLength Length;
};
#pragma pack()
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
