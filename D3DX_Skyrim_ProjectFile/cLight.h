#pragma once // 광원 클래스 (백)

class cLight
{
	int nMaxIndex;
	cLight(void);
	~cLight(void);

public:
	static cLight* GetInstance();	

	bool	SetLight(IN		D3DLIGHT9*	LightInfo,IN int nIndex);
	bool	GetLight(OUT	D3DLIGHT9*	LightInfo,IN int nIndex);

	bool	EnableLight(IN int nIndex,IN bool	bEnable);
	void	SetAmbient(IN DWORD dwColor);
	bool	SetRange(IN int nIndex,IN float fRange);
	bool	SetType(IN int nIndex,IN D3DLIGHTTYPE Type);

	bool	SetDiffuse(IN int nIndex,IN D3DCOLORVALUE Diffuse);
	bool	SetAmbient(IN int nIndex,IN D3DCOLORVALUE Ambient);
	bool	SetPLAYER_ANIMATION(IN int nIndex,IN D3DXVECTOR3 PLAYER_ANIMATION);
};

