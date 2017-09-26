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

#ifdef _S_LINUX_EPOLL_
#define LISTEN_BACKLOG 256
#define ADD_EV(a, b) if (add_ev(a, b) == -1){ pr_err("Fail: add_ev"); exit(1); }
#define DEL_EV(a, b) if (del_ev(a, b) == -1){ pr_err("Fail: del_ev"); exit(1); }
const int max_ep_events = 256;
int epollfd; //epoll 파일기술자
int add_ev(int efd, int fd); //파일기술자 fd를 epoll에 추가
int del_ev(int efd, int fd); //파일기술자 fd를 epoll에서 제거
int fcntl_setnb(int fd); //파일기술자를 넌블록킹 모드로 변경
#endif //_S_LINUX_EPOLL_

#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
extern CRITICAL_SECTION cs;
#endif //_S_LINUX_EPOLL_
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_

#include "../Shared_Skyrim_ProjectFile/PacketStructure.h"

