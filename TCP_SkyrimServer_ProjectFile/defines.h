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

#include "TaskCompileFlag.h"

#include "macros.h"
#include "constants.h"

#ifdef _S_MOD_D3DX9_API_CUSTOM_
#include "customVector.h"
#else //_S_MOD_D3DX9_API_CUSTOM_
#include <d3dx9.h>
#endif //_S_MOD_D3DX9_API_CUSTOM_
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
#include <winsock2.h>
#endif //_S_LINUX_EPOLL_

#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
extern CRITICAL_SECTION cs;
#endif //_S_LINUX_EPOLL_
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_

#include "../Shared_Skyrim_ProjectFile/PacketStructure.h"

