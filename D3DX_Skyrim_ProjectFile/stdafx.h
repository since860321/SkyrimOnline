// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma	warning(disable:4244) // float 형변환문제
#pragma	warning(disable:4996) // 
#pragma	warning(disable:4018) //signed unsigned 문제 

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3dx9.h>
#include <WinSock2.h> //< 라이브러리에 ws2_32.lib 추가하세요. (형석)
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>

#include "defines.h"		

//< 메모리 릭 관련 코드

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

//< extern

extern HINSTANCE g_hInst;
extern HWND g_hWnd;







