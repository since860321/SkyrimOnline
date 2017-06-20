#include "StdAfx.h"
#include "cCharacter.h"


cCharacter::cCharacter(void)
{
	memset(&m_stCharacterInfo,0,sizeof(m_stCharacterInfo));
}


cCharacter::~cCharacter(void)
{
}

bool cCharacter::WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return false;
}
