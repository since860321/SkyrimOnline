#pragma once

#include "targetver.h"

#include <stdio.h>
#ifdef WIN32
#include <tchar.h>
#endif

#include <iostream>
#include <assert.h>
#include <queue>
#include <map>
#include <winsock2.h>

#include "TaskCompileFlag.h"

#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
extern CRITICAL_SECTION cs;
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_

#include "macros.h"
#include "constants.h"

#ifdef _S_MOD_D3DX9_API_CUSTOM_
#include "customVector.h"
#else //_S_MOD_D3DX9_API_CUSTOM_
#include <d3dx9.h>
#endif //_S_MOD_D3DX9_API_CUSTOM_

#include "../Shared_Skyrim_ProjectFile/PacketStructure.h"

