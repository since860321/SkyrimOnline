#pragma once

#include "constants.h"


#pragma warning(disable:4995)

#include <iostream>
using namespace std;

struct SpriteRect
{
	int	nLeft;
	int	nTop;
	int	nRight;
	int	nBottom;
	RECT Rect;

	void SetRect( int tLeft, int tTop, int tRight, int tBottom)
	{
		nLeft  = tLeft;
		nTop   = tTop;
		nRight = tRight;
		nBottom= tBottom;
	}
	RECT GetRect()
	{
		Rect.left  = nLeft;
		Rect.top   = nTop;
		Rect.right = nRight;
		Rect.bottom= nBottom;

		return Rect;
	}
};


class CSprite
{
	LPDIRECT3DDEVICE9			m_lpD3DDevice;
	LPDIRECT3DTEXTURE9			m_lpTexure;
	LPD3DXSPRITE				m_lpSprite;


	SpriteRect		m_rSpriteRect;
	//D3DXVECTOR2		m_vtPosition;
	RECT			m_Rect;

	int				m_nWidth;
	int				m_nHeigh;
	float			m_fAlPha;
	D3DCOLOR		m_d3dColor;

	LPD3DXFONT		m_pFont;

public:
	CSprite(LPDIRECT3DDEVICE9	t_pD3DDevice);
	~CSprite(void);

	void DestroySprite();

public:
	bool CreateSprite(IN char* t_strName, IN int tWidth, IN int tHeigh, IN D3DCOLOR td3dColor=0x00000000);
	void RenderSprite(IN D3DXVECTOR3 t_vtPos,IN float t_AlPha);
	void RenderSprite(IN D3DXVECTOR3 t_vtPos,IN float t_AlPha,IN RECT t_rRect);
	void _CreateFont( IN int Height , IN int Width);


	int			GetWidth(){return m_nWidth/2;}
	int			_GetWidth(){return m_nWidth;}
	void		SetWidth(int width){m_nWidth = width;}
	int			GetHeigh(){return m_nHeigh/2;}

	void		SetRect(SpriteRect tRect){m_rSpriteRect=tRect;}
	RECT*		_GetRect();
	LPD3DXFONT	_GetFont(){return m_pFont;}

private:
	bool _LoadSpriteTexture(IN char* t_strName);
};