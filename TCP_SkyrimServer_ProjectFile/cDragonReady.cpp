#include "cDragonReady.h"
#include "cMove_BacktoStart.h"

cDragonReady::cDragonReady(void)
{
	cComposite* newData;
	newData = new cMove_BacktoStart;
	Add( newData );
}

cDragonReady::~cDragonReady(void)
{
}

void cDragonReady::Process( stEnemyInfo* target )
{

#ifdef _S_MOD_D3DX9_API_CUSTOM_
	vector vTarget	=	target->vStartPos;
	vector vMoster	=	target->vPos;
	vector vPos = vTarget - vMoster;
#else //_S_MOD_D3DX9_API_CUSTOM_
	D3DXVECTOR3 vTarget	=	target->vStartPos;
	D3DXVECTOR3 vMoster	=	target->vPos;
	D3DXVECTOR3 vPos = vTarget - vMoster;
#endif //_S_MOD_D3DX9_API_CUSTOM_

	float Length = vPos.x * vPos.x + vPos.y * vPos.y + vPos.z * vPos.z;

	if(Length < 2)
	{

	if(target->dwHpReduce > 0)
	{
	
#ifdef _S_LINUX_EPOLL_
		unsigned int dwTime;// = GetTickCount();
#else //_S_LINUX_EPOLL_
		DWORD dwTime = GetTickCount();
#endif //_S_LINUX_EPOLL_

		if(target->dwRecoveryTime == UINT_MAX)
		{
			target->dwRecoveryTime = dwTime + target->dwRecoveryCoolTime;  	
		}

		if(target->dwRecoveryTime < dwTime)
		{
		target->nAnimationIndex = (int)D_ground_injured;

		target->dwRecoveryTime = dwTime + target->dwRecoveryCoolTime;

		target->dwHpReduce -= target->dwHpMax/10;
		
		if((int)target->dwHpReduce < 0)
		{
			target->dwHpReduce = 0;
		}

		}

	}
	else
	{
		target->nAnimationIndex = (int)D_ground_bleedoutidle;
	}
	
	}
	else
	{
		Data[0]->Process(target);
	}
}