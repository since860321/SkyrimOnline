#include "stdafx.h"
#include "CSprite.h"
#include "cResourceMgr.h"
#include "cCharacter.h"
#include "cStateBar.h"


cStateBar::cStateBar(void)
{
	
}

cStateBar::~cStateBar(void)
{
	
}
void cStateBar::Setup(cResourceMgr*	pRes, cCharacter* Player, cCharacter* Enemy )
{
	m_pRes = pRes;
	m_pPlayer = Player;
	m_pEnemy = Enemy;
	
}
void cStateBar::PlayerRender()
{
	if(m_pPlayer->GetCharacterInfo()->dwHP > 0)
	{
		m_pRes->GetRes()[IMG_HPAR]->RenderSprite(D3DXVECTOR3(WINSIZEX-120,WINSIZEY-30,0),0.5f);

		int nWidth = 0;
		int Width = m_pRes->GetRes()[IMG_HP]->_GetRect()->right;
		nWidth = Width/(float)(m_pPlayer->GetCharacterInfo()->dwMaxHP)*(float)m_pPlayer->GetCharacterInfo()->dwHP;

		m_pRes->GetRes()[IMG_HP]->SetWidth(nWidth);
		m_pRes->GetRes()[IMG_HP]->RenderSprite(D3DXVECTOR3(WINSIZEX-120,WINSIZEY-30,0),1.0f);
	}
	else
	{
	
	}
	
}
void cStateBar::EnemyRender()
{
	m_pRes->GetRes()[IMG_ENEMYHPAR]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,100,0),0.5f);

	int nWidth = 0;
	int Width = m_pRes->GetRes()[IMG_ENEMYHP]->_GetRect()->right;
	nWidth = Width/(float)(m_pEnemy->GetCharacterInfo()->dwMaxHP)*(float)m_pEnemy->GetCharacterInfo()->dwHP;

	m_pRes->GetRes()[IMG_ENEMYHP]->SetWidth(nWidth);
	m_pRes->GetRes()[IMG_ENEMYHP]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,100,0),1.0f);

	char szBuf[128] = "《드래곤》 ";
	RECT rc3 = { WINSIZEX/2-50 , 60 , 0 ,0};
	cDevice::GetInstance().GetFont()->DrawText
		(NULL, szBuf, strlen(szBuf), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
}
//기본 체력창 
