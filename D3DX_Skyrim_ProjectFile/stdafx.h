// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma	warning(disable:4244) // float ����ȯ����
#pragma	warning(disable:4996) // 
#pragma	warning(disable:4018) //signed unsigned ���� 

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3dx9.h>
#include <WinSock2.h> //< ���̺귯���� ws2_32.lib �߰��ϼ���. (����)
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>

#include "defines.h"		

//< �޸� �� ���� �ڵ�

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

//< extern

extern HINSTANCE g_hInst;
extern HWND g_hWnd;







