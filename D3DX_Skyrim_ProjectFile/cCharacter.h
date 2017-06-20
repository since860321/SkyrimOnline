#pragma once

#include "cActiveObject.h"

class cCharacter : public cActiveObject
{
protected :
	stCharacterInfo	m_stCharacterInfo;

public:
	cCharacter(void);
	~cCharacter(void);
	
	stCharacterInfo* GetCharacterInfo(){return &m_stCharacterInfo;}
	virtual bool WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
	
	virtual void SetMember(void* m_pVoid){}

};