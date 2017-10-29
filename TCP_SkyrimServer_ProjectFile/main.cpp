// TCP_SkyrimServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "defines.h"
#include "cWinSocketServer.h"
#ifdef _S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
int _tmain(int argc, _TCHAR* argv[])
{
	sgNetwork.CreateListenSocket();
	sgNetwork.Bind();
	sgNetwork.Listen();
	sgNetwork.AcceptClient();
	sgNetwork.CloseSocket();

	return 0;
}
#else //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
#include "cAI.h"

#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
CRITICAL_SECTION cs;
#endif //_S_LINUX_EPOLL_

void CreateAIThread( void );
#ifdef _S_LINUX_EPOLL_
int epollfd;

int add_ev(int efd, int fd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLPRI;
	ev.data.fd = fd;

	if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		/* Error with strerror(errno) */
		return -1;
	}

	return 0;
}

int del_ev(int efd, int fd)
{
	if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		printf("fd(%d) EPOLL_CTL_DEL Error(%d:%s)", fd, errno, strerror(errno));
		return -1;
	}
	close(fd);
	return 0;
}

int fcntl_setnb(int fd)
{
	/* only influence about O_ASYNC, O_APPEND, O_NONBLOCK on Linux-specific */
	if (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL)) == -1) {    
		return errno;
	}
	return 0;
}
#else //_S_LINUX_EPOLL_
DWORD WINAPI ProcessAI( LPVOID arg ); //< AI 쓰레드
#endif //_S_LINUX_EPOLL_
#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else 
//int _tmain(int argc, char* argv[])
int main(int argc, char* argv[])
#endif
{
	printf("GameServer Start!\n");

#ifdef _S_LINUX_EPOLL_
	socklen_t	len_saddr;
	int			fd, fd_listener;
	int			i, ret_recv, ret_poll;
	char		*port, buf[1024];
	struct		epoll_event *ep_events;

	if (argc > 2)
	{
		printf("%s [port number]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (argc == 2)
		port = strdup(argv[1]);
	else
		port = strdup("0"); /* random port */
	
	struct addrinfo ai, *ai_ret;
	int		rc_gai;
	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_INET;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
	
	if ((rc_gai = getaddrinfo(NULL, port, &ai, &ai_ret)) != 0)
	{
		printf("Fail: getaddrinfo():%s", gai_strerror(rc_gai));
		exit(EXIT_FAILURE);
	}

	if ((fd_listener = socket(ai_ret->ai_family, ai_ret->ai_socktype, ai_ret->ai_protocol)) == -1)
	{
		printf("Fail: socket()");
		exit(EXIT_FAILURE);
	}

	fcntl_setnb(fd_listener); /* 넌블록킹 모드로 변경 */
	if (bind(fd_listener, ai_ret->ai_addr, ai_ret->ai_addrlen) == -1)
	{
		printf("Fail: bind()");
		exit(EXIT_FAILURE);
	}

	listen(fd_listener, LISTEN_BACKLOG);
	if ((epollfd = epoll_create(1)) == -1) /* epoll 생성, 사이즈는 의미가 없음 */
	{
		exit(EXIT_FAILURE);
	}

	if ((ep_events = (epoll_event*)calloc(max_ep_events, sizeof(struct epoll_event))) == NULL)
	{
		exit(EXIT_FAILURE);
	}

	ADD_EV(epollfd, fd_listener);

	while (1)
	{
		printf("Epoll waiting...");
		if ((ret_poll = epoll_wait(epollfd, ep_events, max_open_files, -1)) == -1)
		{
			/* error */
		}
		printf("Epoll return (%d)", ret_poll);
		for (i = 0; i <ret_poll; ++i)
		{
			if (ep_events[i].events & EPOLLIN)
			{
				if (ep_events[i].data.fd == fd_listener) // 리스너 소켓인지 확인
				{
					struct socketaddr_storage saddr_c;
					while(1)
					{
						len_saddr = sizeof(saddr_c);
						fd = accept(fd_listener, (struct socketaddr *)&saddr_c, &len_saddr);
						if (fd == -1)
						{
							if (errno == EAGAIN) // 더 이상 새로운 연결이 없는 경우
								break;
							printf("Error get connection from listen socket");
							break;
						}
						fcntl_setnb(fd); // 넌블록킹 모드
						ADD_EV(epollfd, fd); // 새로운 연결을 epoll에 등록
						printf("accept : add socket (%d)", fd);
					}
					continue; // 접속을 모두 받았다면 다시 epoll_wait로
				} // if 블록 : 리스너 소켓 확인 블록
				if ((ret_recv = recv(ep_events[i].data.fd, buf, sizeof(buf), 0)) == -1)
				{
					// 에러 발생
				}
				else
				{
					if (ret_recv == 0)
					{
						printf("fd(%d) : Session closed", ep_events[i].data.fd);
						DEL_EV(epollfd, ep_events[i].data.fd);
					}
					else
					{
						printf("recv(fd=%d,n=%d) = %.*s", ep_events[i].data.fd, ret_recv, ret_recv, buf);
					}
				}
			}
			else if (ep_events[i].events & EPOLLPRI) // OOB 데이터를 수신한 경우
			{
				printf("EPOLLPRI : Urgent data detected");
				if ((ret_recv = recv(ep_events[i].data.fd, buf, 1, MSG_OOB)) == -1)
				{
					// 에러 발생
				}
				printf("recv(fd=%d,n=1) = %.*s (OOB)", ep_events[i].data.fd, 1, buf);
			}
			else if (ep_events[i].events & EPOLLERR)
			{
				 // 에러 발생
			}
			else
			{
				printf("fd(%d) epoll event(%d) err(%s)", ep_events[i].data.fd, ep_events[i].events, strerror(errno));
			}
		}
	} // while loop

#else //_S_LINUX_EPOLL_
	InitializeCriticalSection(&cs);	
	sgNetwork.CreateListenSocket();
	sgNetwork.Bind();
	sgNetwork.Listen();

	CreateAIThread(); //< AI 쓰레드 생성

	sgNetwork.AcceptClient();
	sgNetwork.CloseSocket();

	DeleteCriticalSection(&cs);
#endif //_S_LINUX_EPOLL_

	printf("GameServer Shutdown Complete!\n");

	return 0;
}

void CreateAIThread( void )
{
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
	HANDLE hThread;		//< 스레드 핸들
	DWORD ThreadId;		//< 스레드 ID

	//< 각 클라이언트와 통신하는 스레드 생성
	hThread = CreateThread( NULL, 0, ProcessAI, NULL, 0, &ThreadId );
	if( hThread == NULL )
	{
		std::cout << "스레드 생성 실패";
	}
	else
	{
		CloseHandle( hThread );
	}
#endif //_S_LINUX_EPOLL_
}
#ifdef _S_LINUX_EPOLL_
#else //_S_LINUX_EPOLL_
DWORD WINAPI ProcessAI( LPVOID arg )
{
	DWORD m_dwSendTime[MAXENEMY];
	memset(m_dwSendTime, 0,sizeof(m_dwSendTime));
	DWORD	dwTime = 0;
	while( true )
	{
		for( size_t i = 0 ; i < MAXENEMY ; ++ i )
		{		
			dwTime = GetTickCount();
			if( m_dwSendTime[i] >= dwTime ) break;
			m_dwSendTime[i] = dwTime + 1000;
		
			sgAI.AI( &sgNetwork.m_pEnemy[i] );
			if(sgNetwork.m_pEnemy[i].bAlive == false) break;

	

			stEnemyTransportInfo Info;

			Info.bAlive			=	sgNetwork.m_pEnemy[i].bAlive;
			Info.nAnimationIndex = sgNetwork.m_pEnemy[i].nAnimationIndex;
			Info.nEnemyIndex = sgNetwork.m_pEnemy[i].nEnemyIndex;
			Info.nHpReduce = sgNetwork.m_pEnemy[i].dwHpReduce;
			Info.vPos = sgNetwork.m_pEnemy[i].vPos;
			Info.vTarget = sgNetwork.m_pEnemy[i].vTargetPos;

			sgNetwork.SendToServer( Info );
		}
	}

	std::cout << "AI 쓰레드 종료";

	return 0;
}
#endif //_S_LINUX_EPOLL_
#endif //_S_MOD_MONSTER_AI_FROM_SERVER_TO_CLIENT_
