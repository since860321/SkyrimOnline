/*
플래그로 작업 단위를 묶음
*/

//#define _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_	// 2017.02.16 몬스터 AI 구현을 Server->Client 사이드로 이동
#define _S_MOD_REMOVE_PRAGMA_ONCE_WARNING_			// 2017.06.21 pragma once linux warning 제거
#define _S_MOD_CLEAN_UP_FILE_DEPENDANCY_			// 2017.06.21 파일간 종속성 정리

#define _S_MOD_D3DX9_API_CUSTOM_					// 2017.07.23 리눅스에 d3dx sdk 설치 불가 이슈로 인해 직접 만듦(어차피 추후에 udp 모듈 구현되면 ai 모듈 클라로 옮길것임)