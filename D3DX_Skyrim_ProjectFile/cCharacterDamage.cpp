#include "StdAfx.h"
#include "cCharacterDamage.h"



cCharacterDamage::cCharacterDamage(void)
{
	iter		=	stData.begin();
	dwDelay		=	6000;
	dwMoveSpeed	=	3;	
}


cCharacterDamage::~cCharacterDamage(void)
{
}
cCharacterDamage& cCharacterDamage::GetInstance()
{
	static cCharacterDamage	Instance;
	return Instance;
}
void cCharacterDamage::SetUp(DWORD	dwDelay,	DWORD			dwMoveSpeed)
{
	this->dwDelay		= dwDelay;
	this->dwMoveSpeed	= dwMoveSpeed;

}
void cCharacterDamage::SetDamage(DWORD	dwDamage)
{
	DWORD	dwTime = GetTickCount();



	stShowDamage Temp;

	Temp.dwDamage	=dwDamage;
	Temp.dwTime		=dwTime	+	this->dwDelay;

	
	stData.push_back(Temp);
}
void cCharacterDamage::Update()
{
	int	size = stData.size();

	for(int i = size - DAMAGESHOWMAX;i > 0;--i)
	{
		stData.pop_front();
	}

	DWORD	dwTime = GetTickCount();


	for(iter=stData.begin(); iter!=stData.end() ;)
	{
		if(iter->dwTime < dwTime)
		{
			iter = stData.erase(iter);
		}
		else
		{
		++iter;
		}
	}
	
}
void cCharacterDamage::Render()
{
	char szBuff[128];
	DWORD dwTime = GetTickCount();
	for(iter=stData.begin(); iter!=stData.end() ;++iter)
	{

			int posy = (WINSIZEY/2) - (this->dwDelay - (iter->dwTime - dwTime))/100 * dwMoveSpeed;
			wsprintf(szBuff,"%d",(int)iter->dwDamage);
			RECT rc2 = { WINSIZEX/2+50 ,posy-50 , 0 ,0};
			cDevice::GetInstance().GetFont()->DrawText
			(NULL, szBuff, strlen(szBuff), &rc2, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffff0000);
		
	}
	
}

		