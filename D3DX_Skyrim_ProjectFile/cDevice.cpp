#include "StdAfx.h"
#include "cDevice.h"


cDevice::cDevice(void)
{
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
	m_pFont = NULL;
	m_bWireFrameMode = false;
}


cDevice::~cDevice(void)
{
	SAFE_RELEASE( m_pD3D );
	SAFE_RELEASE( m_pd3dDevice );
	SAFE_RELEASE( m_pFont );
	for( size_t i = 0 ; i < WF_END ; ++ i )
	{
		SAFE_RELEASE( m_pWireFrameTexture[i] );
	}
}

cDevice& cDevice::GetInstance( void )
{
	static cDevice device;
	return device;
}

LPDIRECT3DDEVICE9 cDevice::GetDevice( void )
{
	return m_pd3dDevice;
}

HRESULT cDevice::CreateDevice( HWND hWnd )
{
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3D == NULL )
	{
		MessageBox( g_hWnd, "IDirect3D9 인터페이스 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}

	D3DCAPS9 caps;
	int nVP;
	m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
	if( caps.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		nVP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset( &d3dpp, 0, sizeof(d3dpp) );
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if( FAILED (m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, nVP, &d3dpp, &m_pd3dDevice ) ))
	{
		MessageBox( g_hWnd, "IDirect3DDevice9 인터페이스 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}

	//< 폰트 생성
	D3DXCreateFont( m_pd3dDevice, 20, 10, 100, 0, false, DEFAULT_CHARSET, 0, DEFAULT_QUALITY, 0, "신명조", &m_pFont ); 

	//< 동영상 촬영모드 WHITE TEXTURE  할당
	if( FAILED ( D3DXCreateTextureFromFile( m_pd3dDevice, "Resources/textures/whitetex.bmp", &m_pWireFrameTexture[WF_WHITE] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}
	if( FAILED ( D3DXCreateTextureFromFile( m_pd3dDevice, "Resources/textures/redtex.bmp", &m_pWireFrameTexture[WF_RED] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}
	if( FAILED ( D3DXCreateTextureFromFile( m_pd3dDevice, "Resources/textures/yellowtex.bmp", &m_pWireFrameTexture[WF_YELLOW] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}
	if( FAILED ( D3DXCreateTextureFromFile( m_pd3dDevice, "Resources/textures/bluetex.bmp", &m_pWireFrameTexture[WF_BLUE] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}
	if( FAILED ( D3DXCreateTextureFromFile( m_pd3dDevice, "Resources/textures/greentex.bmp", &m_pWireFrameTexture[WF_GREEN] ) ))
	{
		MessageBox( g_hWnd, "텍스쳐 버퍼 생성 실패", "실패", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

void cDevice::SetWireFrame( void )
{
	if( m_bWireFrameMode == false )
	{
		m_bWireFrameMode = true;
	}
	else
	{
		m_bWireFrameMode = false;
	}
}

void cDevice::SetWireFrameTexture( WIREFRAMECOLOR color )
{
	if( m_bWireFrameMode == false )
	{
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}
	else
	{
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		if( color == WF_WHITE )
		{
			m_pd3dDevice->SetTexture( 0, m_pWireFrameTexture[WF_WHITE] );
		}
		else if ( color == WF_RED )
		{
			m_pd3dDevice->SetTexture( 0, m_pWireFrameTexture[WF_RED] );
		}
		else if ( color == WF_YELLOW )
		{
			m_pd3dDevice->SetTexture( 0, m_pWireFrameTexture[WF_YELLOW] );
		}
		else if ( color == WF_BLUE )
		{
			m_pd3dDevice->SetTexture( 0, m_pWireFrameTexture[WF_BLUE] );
		}
		else if ( color == WF_GREEN )
		{
			m_pd3dDevice->SetTexture( 0, m_pWireFrameTexture[WF_GREEN] );
		}
	}
}