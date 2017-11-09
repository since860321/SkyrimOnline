#pragma once

//< 매크로 함수

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p) {if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[] (p); (p) = NULL; } }

//#define sgNetwork cWinSocketServer::GetInstance()
#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#define sgAI cAI::GetInstance() // AI(백)
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_

