// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <WinSock2.h>
#include <iostream>
#include <assert.h>
#include <d3dx9.h>
#include <queue>
#include <map>


#include "TaskCompileFlag.h"

#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
extern CRITICAL_SECTION cs;
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_

#include "defines.h"



