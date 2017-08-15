#pragma once
#include "../TCP_SkyrimServer_ProjectFile/defines.h"
#include "CSCommonConst.h"
//< 구조체 정의부

//< 초기 접속 시 사용되는 패킷
#pragma pack( 1 )
struct stConnectPacket
{
	unsigned char	header; //< ( = sizeof(stConnectPacket.ID)
	unsigned int	ID;
};
#pragma pack()

//< 패킷 LENGTH 정보
#pragma pack( 1 )
struct stLength
{
	unsigned short Length; //< DATA 크기stLength
	unsigned short Option; //< 패킷속성
	unsigned short Sender; //< 보낸 클래스
	int		clientId;	//< 클라이언트 ID 
};
#pragma pack()

//< 변환 정보
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

//< 변환정보 패킷
#pragma pack( 1 )
struct stTransformPacket
{
	//< PART_HEADER 
	unsigned char header; //< LENGTH 크기 ( = sizeof(packet.Length) + sizeof(packet.Option) + sizeof(packet.Sender) + sizeof(packet.clientId); )
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
	BaseD3D::BaseSkinnedMesh*	pModel; //< 전송에는 사용하지 않음
	WEAPON_TYPE useWeapon;
};
#pragma pack()

//< 클라이언트 정보 패킷
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
	stLength Length; //< 옵션에 PO_GENERATE 옵션을 놓고, 클라이언트 ID란에는 생성할 클라이언트 ID를 적는다.
};
#pragma pack()

#pragma pack( 1 )
struct stDeletePacket
{
	//< PART_HEADER 
	unsigned char header; 
	//< PART_LENGTH
	stLength Length; //< 옵션에 PO_DESTROY 옵션을 놓고, 클라이언트 ID란에는 제거할 클라이언트 ID를 적는다.
};
#pragma pack()
#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#pragma pack(1)
struct stEnemyTransportInfo //< 전송정보
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
	CHARACTERTYPE	MonsterType;		// 몬스터종류
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector			vPos;				// 현재위치
	vector			vTargetPos;			// 목표지점
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3		vPos;				// 현제위치
	D3DXVECTOR3		vTargetPos;			// 목표지점
#endif //_S_MOD_D3DX9_API_CUSTOM_
	int				nAnimationIndex;
	bool			bAlive;	
	unsigned int	dwHp;
	unsigned int	dwHpReduce;
	unsigned int	dwHpMax;
	unsigned int	dwAttackDamage;
#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector			vStartPos;			// 초기화위치
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3		vStartPos;			// 초기화위치
#endif //_S_MOD_D3DX9_API_CUSTOM_

	float			fMoveSpeed;			//	이동속도
	float			fPerceptionLength;	// 몬스터의 감지거리
	stClientInfo*	pTarget;			// 목적 대상
	float			fAttackLange;		// 공격거리
	unsigned int	dwAttackCoolTime;	// 공격 쿨타임
	unsigned int	dwAttackTime;		// 공격 시간

	unsigned int	dwRecoveryCoolTime;	// 회복 쿨타임
	unsigned int	dwRecoveryTime;		// 회복 시간

	unsigned int	dwGenerationCoolTime;	// 리젠 쿨타임
	unsigned int	dwGenerationTime;		// 리젠 시간

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
