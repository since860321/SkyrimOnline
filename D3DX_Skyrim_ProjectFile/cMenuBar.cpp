#include "stdafx.h"
#include "cMenuBar.h"
#include "CSprite.h"
#include "cDevice.h"
#include "cStateBar.h"
#include "cInventory.h"

#include "cResourceMgr.h"

cMenuBar::cMenuBar(void)
{
	static bool m_bImage = false;			 // 이미지 사용여부
	static bool m_bToggle = true;			 // 토글관련

	m_MenuIndex = 0;
	m_ItemIndex = 0;

	m_pCsprite = new CSprite(sgDevice);
	
}

cMenuBar::~cMenuBar(void)
{
	SAFE_DELETE(m_pStateBar);
	SAFE_DELETE(m_pCsprite);
}
void cMenuBar::Setup(cResourceMgr*	pRes, cCharacter* Player, cInventory*	Inven,cStateBar* pState)
{
	m_pRes = pRes;
	m_pPlyer = Player;
	m_Inven = Inven;
	m_pStateBar = pState;
	
}
void cMenuBar::Render()
{
	//m_pRes->GetRes()[IMG_SHOW4]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.1f);
	switch(m_MenuIndex)
	{
	case 0:
		{
			MenuSelect();
		}
		break;
	case 1:
		{
			MenuItem();
		}
		break;

	}
}

void cMenuBar::MenuSelect()
{
	POINT pt;
	GetCursorPos(&pt);
	static bool tCheck = true;
	static bool tRock2 = true;

	

	if(tCheck == true)
	{
		m_pCsprite->_CreateFont(20,13);
		tCheck = false;
	}
	RECT rcright={WINSIZEX/2+300,WINSIZEY/2-10,30,30};
	RECT rcleft={WINSIZEX/2-350,WINSIZEY/2-10,30,30};
	RECT rcdown={WINSIZEX/2-28,WINSIZEY/2+180,30,30};
	RECT rcup={WINSIZEX/2-28,WINSIZEY/2-180,30,30};


	//메뉴
	if(m_bImage)
	{
		m_pRes->GetRes()[IMG_MENU]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_MENU1]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_SHOW]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.4f);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"물 품",6,&rcright,DT_LEFT | DT_NOCLIP | DT_TOP , 0xff777777);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"마 법",6,&rcleft,DT_LEFT | DT_NOCLIP | DT_TOP , 0xff777777);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"지 도",6,&rcdown,DT_LEFT | DT_NOCLIP | DT_TOP , 0xff777777);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"기 술",6,&rcup,DT_LEFT | DT_NOCLIP | DT_TOP , 0xff777777);
	}

	//if(충돌하면)
	//그림 나타나게 해라
	//윈도우를 4등분
	//>수정해야함
	RECT WindowLeft,WindowRight,WindowBottom,WindowUp;
	SetRect(&WindowLeft,0,0,WINSIZEX/2,WINSIZEY/2);
	SetRect(&WindowRight,WINSIZEX/2,WINSIZEY/2,WINSIZEX,WINSIZEY);
	SetRect(&WindowBottom,0,WINSIZEY/2,WINSIZEX/2,WINSIZEY);
	SetRect(&WindowUp,WINSIZEX/2,0,WINSIZEX,WINSIZEY);

	//그림 마다 각각의 플래그값을 넣어서 그려질까 말까 판단.
	if(PtInRect(&WindowLeft,pt))
	{
		m_bImage = true;
		m_pRes->GetRes()[IMG_CROSSLEFT]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_FONT2]->RenderSprite(D3DXVECTOR3(WINSIZEX/2-400,WINSIZEY/2,0),1.0f);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"마 법",6,&rcleft,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	}
	//충돌되고

	if(PtInRect(&WindowRight,pt))
	{
		
		if(m_bImage)
		{
			m_pRes->GetRes()[IMG_CORSSRIGHT]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);//오른쪽 메뉴 빛
			m_pRes->GetRes()[IMG_FONT1]->RenderSprite(D3DXVECTOR3(WINSIZEX/2+400,WINSIZEY/2,0),1.0f);
			m_pCsprite->_GetFont()->DrawTextA(NULL,"물 품",6,&rcright,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
			tRock2 = false;	
			if(GetKeyState(MK_LBUTTON)&0x8000)
			{
				if(m_bToggle)
				{
					m_bImage = !m_bImage;
					m_bToggle = false;
					m_MenuIndex = 1;
				}
			}
			else
			{
				m_bToggle = true;
			}
		}	
	}
	
	//<아이템 메뉴
	
	else if(PtInRect(&WindowBottom,pt))
	{
		m_bImage = true;
		m_pRes->GetRes()[IMG_CORSSBOTTOM]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_FONT4]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2+250,0),1.0f);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"지 도",6,&rcdown,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	}
	else if(PtInRect(&WindowUp,pt))
	{	
		m_bImage = true;
		m_pRes->GetRes()[IMG_CORSSUP]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_FONT3]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2-250,0),1.0f);
		m_pCsprite->_GetFont()->DrawTextA(NULL,"기 술",6,&rcup,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
	}


}
void cMenuBar::MenuItem()
{
	POINT pt;
	GetCursorPos(&pt);
	static bool bcheck = true;
	static bool bToggle2 = true;
	static bool btoggle = true;

	if(!m_bImage)
	{
		
		m_pRes->GetRes()[IMG_MENUITEM]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);
		m_pRes->GetRes()[IMG_SHOW2]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.4f);
		

		RECT rc={60,WINSIZEY/2,110,WINSIZEY/2+50};

		if(bcheck == true)
		{
			m_pCsprite->_CreateFont(20,11);
			bcheck = false;
		}
	
		m_pCsprite->_GetFont()->DrawTextA(NULL,"모두",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);

		if(PtInRect(&rc,pt))
		{
			m_pCsprite->_GetFont()->DrawTextA(NULL,"모두",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xff000000);
			
			static bool bPresh = false;
			
			if( GetAsyncKeyState(MK_LBUTTON)& 0x8000 )
			{
				if( bPresh == false )
				{
					bPresh = true;

					if( btoggle == true)
					{
						btoggle = false;
					}
					else
					{
						btoggle = true;
					}
				}
			}
			else
			{
				bPresh = false;
			}
		}
		if(btoggle == false)
		{
			
			m_Inven->Inventory(m_pRes,m_pCsprite,m_pPlyer);
		}
		
	
		rc.top += 60;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"무기",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
		rc.top += 45;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"의류",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
		rc.top += 45;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"물약",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
		rc.top += 45;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"음식",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
		rc.top += 45;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"재료",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
		rc.top += 45;
		m_pCsprite->_GetFont()->DrawTextA(NULL,"열쇠",6,&rc,DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);

	}
}