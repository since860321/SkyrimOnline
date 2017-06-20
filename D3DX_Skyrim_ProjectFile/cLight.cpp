#include "StdAfx.h"
#include "cLight.h"
#include <d3dx9.h>

cLight::cLight(void)
{
	nMaxIndex = 0;
}

cLight::~cLight(void)
{
}

cLight* cLight::GetInstance()
{
	static cLight Light;
	return &Light;
}

bool	cLight::SetLight(IN D3DLIGHT9*	LightInfo,IN int nIndex)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
    sgDevice->SetLight( nIndex, LightInfo );
	nMaxIndex++;
	return true;
}

bool	cLight::GetLight(OUT	D3DLIGHT9* LightInfo,IN int nIndex)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
    sgDevice->GetLight( nIndex, LightInfo );
	return true;
}


bool	cLight::EnableLight(IN int nIndex,IN bool	bEnable)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
    sgDevice->LightEnable( nIndex, bEnable );
	return true;
}
void	cLight::SetAmbient(IN DWORD dwColor)
{

    sgDevice->SetRenderState( D3DRS_AMBIENT, dwColor );

}


bool	cLight::SetRange(IN int nIndex,IN float fRange)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
	D3DLIGHT9	light;
	sgDevice->GetLight( nIndex, &light );
	light.Range = fRange;
	sgDevice->SetLight( nIndex, &light );
	return true;
}
bool	cLight::SetType(IN int nIndex,IN D3DLIGHTTYPE Type)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
	D3DLIGHT9	light;
	sgDevice->GetLight( nIndex, &light );
	light.Type = Type;
	sgDevice->SetLight( nIndex, &light );
	return true;
}

bool	cLight::SetDiffuse(IN int nIndex,IN D3DCOLORVALUE Diffuse)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
	D3DLIGHT9	light;
	sgDevice->GetLight( nIndex, &light );
	light.Diffuse = Diffuse;
	sgDevice->SetLight( nIndex, &light );
	return true;
}
bool	cLight::SetAmbient(IN int nIndex,IN D3DCOLORVALUE Ambient)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
	D3DLIGHT9	light;
	sgDevice->GetLight( nIndex, &light );
	light.Ambient = Ambient;
	sgDevice->SetLight( nIndex, &light );
	return true;
}
bool	cLight::SetPLAYER_ANIMATION(IN int nIndex,IN D3DXVECTOR3 PLAYER_ANIMATION)
{
	if(nIndex>nMaxIndex || nIndex < 0) return false;
	D3DLIGHT9	light;
	sgDevice->GetLight( nIndex, &light );
	light.Direction = PLAYER_ANIMATION;
	sgDevice->SetLight( nIndex, &light );
	return true;
}