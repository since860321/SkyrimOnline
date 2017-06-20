// D3DX_Skyrim.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "cAutoReleasePool.h"
#include "cMainGame.h"

HINSTANCE g_hInst;
HWND g_hWnd;
char g_ClassName[128] = "Skyrim";
char g_WinName[128] = "Skyrim";

cMainGame g_MainGame;

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	int *p = new int[3];

	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	int nWidth, nHeight;

	nWidth = WINSIZEX + GetSystemMetrics( SM_CXFRAME ) * 2;
	nHeight = WINSIZEY + GetSystemMetrics( SM_CYFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC) WndProc;
	WndClass.lpszClassName = g_ClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass( &WndClass );

	g_hWnd = CreateWindow( g_ClassName, g_WinName, WS_OVERLAPPEDWINDOW, STARTX, STARTY, nWidth, nHeight, NULL, NULL, hInstance, NULL );

	ShowWindow( g_hWnd, nCmdShow );

	assert( g_MainGame.Setup() && "Message : WinMain Setup FAIL"  );

	for( ; ; )
	{
		if( PeekMessage( &Message, 0, 0, 0, PM_REMOVE ) )
		{
			if( Message.message == WM_QUIT )
			{
				break;
			}
			else
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
		else
		{
			g_MainGame.Manager();
			cAutoReleasePool::GetInstance().Drain();	//< 런루프 한바퀴 돌고 드레인( 형석 )
		}
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return g_MainGame.MainProc( hWnd, iMessage, wParam, lParam );
}
