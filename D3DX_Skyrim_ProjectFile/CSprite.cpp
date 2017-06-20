#include "StdAfx.h"
#include "cDevice.h"
#include "CSprite.h"


CSprite::CSprite(LPDIRECT3DDEVICE9	t_pD3DDevice)
:	m_lpD3DDevice(t_pD3DDevice)
{
	ZeroMemory(&m_rSpriteRect, sizeof(SpriteRect));
	m_fAlPha = 1.0f;
	m_pFont = NULL;

	m_lpTexure = NULL; //< (형석)
	m_lpSprite = NULL; //< (형석)
}

CSprite::~CSprite(void)
{
	DestroySprite();

}

void CSprite::DestroySprite()
{
	SAFE_RELEASE(m_lpTexure);		
										
	SAFE_RELEASE(m_lpSprite);

	SAFE_RELEASE( m_pFont );
}

bool CSprite::_LoadSpriteTexture(char* t_strName)
{
	D3DXIMAGE_INFO	t_info;
	ZeroMemory(&t_info, sizeof(t_info));

	if(FAILED(D3DXCreateTextureFromFileEx(m_lpD3DDevice,
										  t_strName,
										  m_nWidth,
										  m_nHeigh,
										  0,
										  D3DPOOL_DEFAULT,
										  D3DFMT_UNKNOWN,
										  D3DPOOL_MANAGED,
										  D3DX_DEFAULT,
										  D3DX_DEFAULT,
										  RGB(255,255,255),
										  &t_info,
										  NULL,
										  &m_lpTexure)))
	{
		return false;
	}
	return true;
}

bool CSprite::CreateSprite(char* t_strName, int tWidth, int tHeigh, D3DCOLOR td3dColor)
{
	m_nWidth = tWidth;
	m_nHeigh = tHeigh;
	m_rSpriteRect.SetRect(0, 0, m_nWidth, m_nHeigh);

	if(FAILED(D3DXCreateSprite(m_lpD3DDevice, &m_lpSprite)))
		return false;

	if(FAILED(_LoadSpriteTexture(t_strName)))
		return false;

	return true;
}

void CSprite::RenderSprite(D3DXVECTOR3 t_vtPos,float t_AlPha,RECT t_rRect)
{
	D3DCOLOR Color		= D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, t_AlPha);
	t_vtPos	= D3DXVECTOR3(t_vtPos.x-m_nWidth/2, t_vtPos.y-m_nHeigh/2,0.0);
	t_rRect	= m_rSpriteRect.GetRect();


	m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_lpSprite->Draw(m_lpTexure,
					 &t_rRect,
					 NULL,
					 &t_vtPos,
					 Color);

	m_lpSprite->End();
}
void CSprite::RenderSprite(D3DXVECTOR3 t_vtPos,float t_AlPha)
{
	D3DCOLOR Color		= D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, t_AlPha);
	t_vtPos	= D3DXVECTOR3(t_vtPos.x-m_nWidth/2, t_vtPos.y-m_nHeigh/2,0.0);

	RECT rc = {0,0,m_nWidth,m_nHeigh};

	m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_lpSprite->Draw(m_lpTexure,
		&rc,
		NULL,
		&t_vtPos,
		Color);

	m_lpSprite->End();
}
RECT* CSprite::_GetRect()
{
	m_Rect.left = m_rSpriteRect.GetRect().left;
	m_Rect.right = m_rSpriteRect.GetRect().right;
	m_Rect.top = m_rSpriteRect.GetRect().top; 
	m_Rect.bottom = m_rSpriteRect.GetRect().bottom;
	return &m_Rect;
}

void CSprite::_CreateFont(int Height, int Width)
{
	D3DXCreateFont(m_lpD3DDevice, Height, Width, 100, 0, false, DEFAULT_CHARSET, 0, DEFAULT_QUALITY, 0, "휴먼둥근헤드라인", &m_pFont); 
}




