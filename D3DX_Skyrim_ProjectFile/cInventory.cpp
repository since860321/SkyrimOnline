#include "StdAfx.h"
#include "cObjFileMesh.h"
#include "cDevice.h"
#include "cResourceMgr.h"
#include "CSprite.h"
#include "cCharacter.h"
#include "cInventory.h"
#include "cSoundMgr.h"

cInventory::cInventory(void)
{
}

cInventory::~cInventory(void)
{
	Delete();
}
void cInventory::GetMesh(cObjFileMesh* obj, char* ObjName,int Attack, int Defence, int Weight)
{	
	m_InvenObject[ObjName].ObjInfo = obj;
	m_InvenObject[ObjName].nAttack = Attack;
	m_InvenObject[ObjName].nDefence = Defence;
	m_InvenObject[ObjName].nweight = Weight;
	m_InvenObject[ObjName].nObjName = ObjName;

}
void cInventory::Inventory(cResourceMgr* ResMgr, CSprite* Sprite, cCharacter*	character)
{
	
	char szBuf[128];
	m_Player = character;
	POINT pt;
	GetCursorPos(&pt);
	static bool _bcheck = true;
	static bool _bcheck2 = true;
	RECT ObjRc;
	
	RECT Fontrc = {WINSIZEX/2+220,WINSIZEY/2+130,WINSIZEX/2+310,WINSIZEY/2+180};

	ResMgr->GetRes()[IMG_ITEMSELECT]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),1.0f);	
	ResMgr->GetRes()[IMG_SHOW3]->RenderSprite(D3DXVECTOR3(WINSIZEX/2,WINSIZEY/2,0),0.4f);

	if(_bcheck == true)
	{
		Sprite->_CreateFont(20,12);
		_bcheck = false;
	}

	std::map<char*,stInvenObjInfo>::iterator iter;

	int temp = 0;	
	
	stTransformInfo TransInfo;
	for(iter = m_InvenObject.begin(); iter!=m_InvenObject.end(); ++iter)
	{

		SetRect(&ObjRc,300,WINSIZEY/2+(50*temp),450,WINSIZEY/2+50+(50*temp));
	
		Sprite->_GetFont()->DrawTextA(NULL,iter->second.nObjName,strlen(iter->second.nObjName), &ObjRc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xff777777);	
	
		if(m_Player->GetCharacterInfo()->m_UseWeapon == WEAPON_TYPE(temp))
		{
			RECT	rCheckPos=ObjRc;
			rCheckPos.left	 -= 30;
			wsprintf(szBuf,"V ");
			Sprite->_GetFont()->DrawTextA(NULL,szBuf,3, &rCheckPos, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffff88);	//무게
		}

		if(PtInRect(&ObjRc,pt))
		{
				Sprite->_GetFont()->DrawTextA(NULL,iter->second.nObjName,strlen(iter->second.nObjName), &ObjRc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);	
				
				TransInfo.vScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
				TransInfo.vPos = D3DXVECTOR3(70,0,280);
				Render(iter->second.nObjName,TransInfo); // 무기출력

				ResMgr->GetRes()[IMG_ITEM_MENU]->RenderSprite(D3DXVECTOR3(WINSIZEX/2+280,WINSIZEY/2+200,0),0.7f);
				Fontrc.top += 10;
				Sprite->_GetFont()->DrawTextA(NULL,iter->second.nObjName,strlen(iter->second.nObjName), &Fontrc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);	//이름

				Fontrc.left -= 100;
				Fontrc.top += 90;
				wsprintf(szBuf,"피해 %d",iter->second.nAttack);
				Sprite->_GetFont()->DrawTextA(NULL,szBuf,7, &Fontrc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);	//피해

				Fontrc.left += 150;
				wsprintf(szBuf,"무게 %d",iter->second.nweight);
				Sprite->_GetFont()->DrawTextA(NULL,szBuf,7, &Fontrc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);	//무게

				Fontrc.left += 150;
				wsprintf(szBuf,"방어 %d",iter->second.nDefence);
				Sprite->_GetFont()->DrawTextA(NULL,szBuf,7, &Fontrc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);	//방어력
		
				if(GetKeyState(MK_LBUTTON)&0x8000)
				{
					m_Player->GetCharacterInfo()->m_UseWeapon = WEAPON_TYPE(temp);
					cSoundMgr::GetInstance()->SoundPlay(SOUND_U_CHOICE);

				}
		}
		temp++;

	}
	
}
void cInventory::Render(char* ObjName,stTransformInfo TransInfo)
{
	//오브젝트가 캐릭터를 따라다녀야함
	D3DXMATRIXA16 mView, mInvView, mScale, mPos, mRoty,mRotz,matWorld;
	static float fRot = 0.0f;
	sgDevice->GetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixInverse(&mInvView, 0, &mView);
	
	TransInfo.vScale = D3DXVECTOR3(TransInfo.vScale.x, TransInfo.vScale.y, TransInfo.vScale.z);
	TransInfo.vPos = D3DXVECTOR3(TransInfo.vPos.x,TransInfo.vPos.y,TransInfo.vPos.z);
	D3DXMatrixScaling(&mScale,TransInfo.vScale.x,TransInfo.vScale.y,TransInfo.vScale.z);
	D3DXMatrixTranslation(&mPos,TransInfo.vPos.x,TransInfo.vPos.y,TransInfo.vPos.z);
	D3DXMatrixRotationY(&mRoty,fRot);
	D3DXMatrixRotationX(&mRotz,-D3DX_PI/2);

	fRot+= 0.01f;
	matWorld = mScale*mRotz*mRoty*mPos*mInvView;
	

	//(*m_stCharacterInfo.m_pWeapon)[m_stCharacterInfo.m_UseWeapon]->m_pTex);
	sgDevice->SetTransform(D3DTS_WORLD, &matWorld);
	sgDevice->SetStreamSource(0,m_InvenObject[ObjName].ObjInfo->m_pVB,0,sizeof(stObjVertex));
	sgDevice->SetTexture( 0, m_InvenObject[ObjName].ObjInfo->m_pTex  );
	sgDevice->SetMaterial( &m_InvenObject[ObjName].ObjInfo->m_Mtrl );
	sgDevice->SetFVF(OBJ_D3DFVF);
	sgDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,m_InvenObject[ObjName].ObjInfo->m_Vertices.size()/3);

	
}
void cInventory::Delete()
{
	std::map<char*,stInvenObjInfo> ::iterator iter;
	for(iter = m_InvenObject.begin(); iter!=m_InvenObject.end();)
	{
		iter = m_InvenObject.erase(iter);
	}
}