#pragma once
class cAxis
{
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	stGridVertex								m_Vertices[2];

public:
	cAxis(void);
	~cAxis(void);

	void Setup( HWND hWnd, D3DCOLOR color, AXIS axis );
	void Render( void );
};

