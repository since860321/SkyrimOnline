#pragma once

#include "cStaticObject.h"

class cSKybox : public cStaticObject
{
public:
	D3DXVECTOR3* vEyePos;

public:

	cSKybox(void);
	~cSKybox(void);
	void	SetTexture(char* top, char* bottom, char* left, char* right,char* front,char* back);
	void	SetEyePos(D3DXVECTOR3* EyePos) {vEyePos = EyePos;}

	void Render();

public:
	LPDIRECT3DVERTEXBUFFER9		m_Vtx[6];
	LPDIRECT3DINDEXBUFFER9		m_Idx[6];
	LPDIRECT3DTEXTURE9			m_Tex[6];


};
