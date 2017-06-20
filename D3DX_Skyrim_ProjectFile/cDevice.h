#pragma once

class cDevice
{
private:
	LPDIRECT3D9							m_pD3D;
	LPDIRECT3DDEVICE9				m_pd3dDevice;
	LPD3DXFONT							m_pFont;

	LPDIRECT3DTEXTURE9		m_pWireFrameTexture[WF_END]; //< 동영상 촬영용 텍스쳐
public:
	bool								m_bWireFrameMode;

//private:
	cDevice(void);
	~cDevice(void);
public:
	static cDevice& GetInstance( void );
	LPDIRECT3DDEVICE9 GetDevice( void );
	HRESULT CreateDevice( HWND hWnd );

	LPD3DXFONT GetFont( void ) { return m_pFont; }
	void SetWireFrame( void );
	void SetWireFrameTexture( WIREFRAMECOLOR color );
};

