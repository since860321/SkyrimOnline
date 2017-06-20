#include "StdAfx.h"
#include "cAI.h"
#include "cAIStatus.h"
#include "cDragonAI.h"

cAI::cAI(void)
{

m_DragonAIStatus = new cDragonAI;
}

cAI::~cAI(void)
{
	SAFE_DELETE(m_DragonAIStatus);
}


cAI& cAI::GetInstance( void )
{
static cAI AI;
return AI;
}
	
void cAI::SetPlayerInfo(IN	std::map<int ,stClientInfo>* m_Client)
{
	this->PlayerInfo = m_Client;
}

//struct stEnemyInfo
	//int				nEnemyIndex;
	//CHARACTERTYPE	MonsterType;
	//D3DXVECTOR3		vPos;
	//D3DXVECTOR3		vTarget;
	//int				nAnimationIndex;
	//bool			bAlive;
	//DWORD			dwHp;
	//DWORD			dwHpMax;
	//DWORD			dwAttackDamage;
	//D3DXVECTOR3		vStartPos;
void cAI::AI(IN	stEnemyInfo*  MonsterInfo)
{
	switch(MonsterInfo->MonsterType)
	{
	case MONSTER_DRAGON:
		{
			
		m_AIStatus = m_DragonAIStatus;
		}
		break;
	default :
		{
			return;
		}
		break;
	}

	int nLiveCnt = 0;

	if(!PlayerInfo)
		return;

	EnterCriticalSection(&cs);


	//system("cls");
	std::map<int ,stClientInfo>::iterator iter = PlayerInfo->begin();

	for( iter = PlayerInfo->begin() ; iter != PlayerInfo->end(); iter ++ )
	{
		if( iter->second.bAlive == false ) continue;

		std::cout << iter->first << "Ŭ���̾�Ʈ HP : " << iter->second.nHP << std::endl;
		m_AIStatus->AISet(&iter->second,MonsterInfo);
		if( iter->second.bAlive == true )
		{
			nLiveCnt ++;
		}
	}

	LeaveCriticalSection(&cs);

	std::cout << "������ Ŭ���̾�Ʈ �� : " << nLiveCnt << std::endl;

	
}