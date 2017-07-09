#pragma once
#include "defines.h"
//< 구조체 정의부

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

#pragma pack( 1 )
struct stGeneratePacket
{
	//< PART_HEADER 
	BYTE header; 
	//< PART_LENGTH
	stLength Length; //< 옵션에 PO_GENERATE 옵션을 놓고, 클라이언트 ID란에는 생성할 클라이언트 ID를 적는다.
};
#pragma pack()

#pragma pack( 1 )
struct stDeletePacket
{
	//< PART_HEADER 
	BYTE header; 
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
	CHARACTERTYPE	MonsterType;		// 몬스터종류
	D3DXVECTOR3		vPos;				// 현제위치
	D3DXVECTOR3		vTargetPos;			// 목표지점
	int				nAnimationIndex;
	bool			bAlive;	
	DWORD			dwHp;
	DWORD			dwHpReduce;
	DWORD			dwHpMax;
	DWORD			dwAttackDamage;
	D3DXVECTOR3		vStartPos;			// 초기화위치
	
	float			fMoveSpeed;			//	이동속도
	float			fPerceptionLength;	// 몬스터의 감지거리
	stClientInfo*	pTarget;			// 목적 대상
	float			fAttackLange;		// 공격거리
	DWORD			dwAttackCoolTime;	// 공격 쿨타임
	DWORD			dwAttackTime;		// 공격 시간

	DWORD			dwRecoveryCoolTime;	// 회복 쿨타임
	DWORD			dwRecoveryTime;		// 회복 시간

	DWORD			dwGenerationCoolTime;	// 리젠 쿨타임
	DWORD			dwGenerationTime;		// 리젠 시간

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
