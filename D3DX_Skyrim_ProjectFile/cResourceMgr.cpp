#include "stdafx.h"
#include "CSprite.h"
#include "cResourceMgr.h"
#include "cDevice.h"
#include "cMenuBar.h"
#include "cCharacter.h"
#include "cStateBar.h"
#include "cSoundMgr.h"

cResourceMgr::cResourceMgr(void)
{
	memset(m_KeyDown,0,sizeof(KEYMAX));
	m_check = true;

}
cResourceMgr::~cResourceMgr(void)
{
	Delete();
	SAFE_DELETE(m_pMenuBar);
}
void cResourceMgr::SetUp(cCharacter* character,cCharacter* Enemy,cInventory* pInven)
{
	ImageLoad(); // �̹����ε�

	m_pStateBar = new cStateBar;
	m_pStateBar->Setup(this,character,Enemy);

	m_pPlayer = character;
	m_pMenuBar = new cMenuBar;
	m_pMenuBar->Setup(this,character,pInven,m_pStateBar);


}
//��������Ʈ ����
void cResourceMgr::Insert(char* FileName, int nWidth, int nHeight, D3DCOLOR td3dColor)
{
	m_pSprite = new CSprite(sgDevice);

	m_pSprite->CreateSprite(FileName,nWidth,nHeight,td3dColor); 

	m_vResourceMgr.push_back(m_pSprite);
	
}
void cResourceMgr::Delete()
{
	vector<CSprite*>::iterator itr;
	for(itr = m_vResourceMgr.begin(); itr!=m_vResourceMgr.end(); ++itr)
	{
		delete(*itr);
		//>�����ؾ���. 
	}
	m_vResourceMgr.clear();
}
void cResourceMgr::Render()
{
	//���⿡�� ����UI�� ���� �Լ��� ����

	//< �ӽ÷� �� ü��â ���
	if(m_pPlayer->GetCharacterInfo()->m_pFocus != NULL)
	{
		m_pStateBar->EnemyRender();
		if(m_check == true)
		{
			if(m_pPlayer->GetCharacterInfo()->m_bAtk == !false)
			{
				m_vResourceMgr[IMG_RCROSSHEIR]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.5f);
			}
		}
	}
	
	
	
	m_pStateBar->PlayerRender();
	
	
	
	KeyInput();
	//��ǰ Ŭ���ϸ� ������
	// 	if(bLive == false)
	// 	m_pStateBar->Render();

}
void cResourceMgr::ImageLoad()
{
	//< �̹��� �ѹ��� �ε�

	//< ���콺 ������
	Insert("Resources/UIResource/mousePoint.png",44,45,NULL);

	//< �޴�
	Insert("Resources/UIResource/CrossVar.png",553,298,NULL);
	Insert("Resources/UIResource/menubar1.png",1024,768,NULL);
	
	//< �޴�����
	Insert("Resources/UIResource/Cross2Varbottom.png",553,298,NULL);
	Insert("Resources/UIResource/CrossVar2Left.png",553,298,NULL);
	Insert("Resources/UIResource/CrossVar2Right.png",553,298,NULL);
	Insert("Resources/UIResource/CrossVar2Up.png",553,298,NULL);

	Insert("Resources/UIResource/menu-select.png",36,36,NULL);
	Insert("Resources/UIResource/menu-select_left.png",36,36,NULL);
	Insert("Resources/UIResource/menu-select_up.png",36,36,NULL);
	Insert("Resources/UIResource/menu-select_down.png",36,36,NULL);


	//< �޴� ������
	Insert("Resources/UIResource/ITEM-MENU.png",1024,768,NULL);
	Insert("Resources/UIResource/item-menu-weapon.png",1024,768,NULL);
	

	//< ����
	Insert("Resources/UIResource/hp.png",148,15,NULL);
	Insert("Resources/UIResource/hpbar-ar.png",191,22,NULL);

	//< ������
	Insert("Resources/UIResource/enemyhp.png",317,14,NULL);
	Insert("Resources/UIResource/enemyhpar.png",297,8,NULL);

	//< TEST 
	Insert("Resources/UIResource/show.png",WINSIZEX,WINSIZEY,NULL);
	Insert("Resources/UIResource/show2.png",WINSIZEX,WINSIZEY,NULL);
	Insert("Resources/UIResource/show3.png",WINSIZEX,WINSIZEY,NULL);
	


	//< ũ�ν����
	Insert("Resources/UIResource/CrossHeir.png",32,30,NULL);
	Insert("Resources/UIResource/RedCrossHeir.png",32,30,NULL);

	Insert("Resources/UIResource/itemmenu.png",528,186,NULL);
	Insert("Resources/UIResource/show4.png",1024,768,NULL);


}
void cResourceMgr::KeyInput() //Ű
{
	POINT pt;
	GetCursorPos(&pt);

	//< �޴��� ���ȴ��� �ȿ��ȴ��� �Ǵ�
	static bool bLive = false;
	
	//< Ű�� ���Ǵ��� �ȴ��Ǵ��� �Ǵ�
	static bool push = true;

	
	
	
	if(m_check == true)
	{
	
		m_vResourceMgr[IMG_CROSSHEIR]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.5f);

	}

	if(GetKeyState(VK_TAB)&0x8000 )
	{	
		//< ���� ������ �κ�â �ʱ�ȭ?
		m_pMenuBar->SetbImage(true);
		

		if(push)
		{
			bLive = !bLive;
			push= !push;	
			cSoundMgr::GetInstance()->SoundPlay(SOUND_U_OFF);
			m_check = !m_check;
			m_pPlayer->GetCharacterInfo()->m_bMouseMove = !m_pPlayer->GetCharacterInfo()->m_bMouseMove;
		}
	}
	else
	{
		push = true;
		
	}
	if(bLive)
	{
		m_pMenuBar->Render();
		
		//< ���콺 ������
		m_vResourceMgr[IMG_MOUSEPT]->RenderSprite(D3DXVECTOR3(pt.x,pt.y,0),1.0f);
	}
	else
	{
		m_pMenuBar->SetMenuInex(0);
	}
	
	if(GetKeyState('R')&0x8000 )
	{
		m_pPlayer->GetCharacterInfo()->bAlive = true;
		m_pPlayer->GetCharacterInfo()->dwHP = 50;
		m_pPlayer->GetTransformInfo()->vRot.x = 0;
		m_pPlayer->GetTransformInfo()->vPos	= D3DXVECTOR3(0,0,1500);
		m_pPlayer->GetCharacterInfo()->dwDeadTime	=	0;

	}
		
}

// bool cResourceMgr::KeyDown(int vKey)
// {
// 	return false;
// }
