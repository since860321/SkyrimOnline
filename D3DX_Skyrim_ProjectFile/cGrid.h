#pragma once

class cAxis;
class cGrid
{
	stGridVertex						m_Vertices[GRIDSIZE*GRIDSIZE];
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	cAxis*								m_pAxis;

public:
	cGrid(void);
	~cGrid(void);

	void Setup( HWND hWnd );
	void Render( void );
	void SetGridWidth( float fGridWidth );

};

