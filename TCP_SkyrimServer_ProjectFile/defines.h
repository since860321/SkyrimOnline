#pragma once

#include "targetver.h"

#include <stdio.h>
#ifdef WIN32
#include <tchar.h>
#else
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char  BYTE;
#endif

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

#include "macros.h"
#include "constants.h"
#include "structures.h"





