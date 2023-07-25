/*
	IOCP 모델

	OS 가 개입 관리하는 고속 입/출력 모델. 
	OS 가 알아서 소켓의 변화를 감지해서 알려주는 모델
	
	IOCP 에 등록한 Socket 이 Recv ( WSARecv ) 하면 GQCS ( Callback, GetQueuedCompletionStatus )함수 를 호출하여, Socket 이 수신한 소켓과 데이터의 정보를 알려준다.
	OS 메모리를 효율적으로 관리한다 : 이를 OS 가 관리하며, 메모리 또한 OS 가 Lock 을 걸고 관리하여, 수신한 데이터를 바로 UserMode 메모리 영역에 매핑 시켜준다. 
	OS 가 Thread 를 효율적으로 관리한다 : GQCS Callback 함수를 관리하여, 이를 호출하는 Thread 를 OS 가 효율적으로 관리
	Kernel 프로세스로서, CPU 스케쥴링 또한 우선적으로 선점할 수 있어 빠르다.
*/

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <mutex>
#include <vector>

#pragma comment(lib, "ws2_32")

#define MAX_IOCP_THREAD_COUNT 4	// IOCP 에서 WSARecv 를 처리할 Thread 개수, IOCP 모델에서 OS 가 알아서 깨운다.

#define ServerIP "192.168.0.4" // Listen IP
#define ServerPORT 25000	// PortNumber

/* Client 와의 연결 정보를 저장할 구조체 */
struct Session {

	/* Client 와 통신하기 위한 버퍼*/
	SOCKET hSocketForClient;

	/* Client 로부터 받은 데이터를 받을 버퍼 */
	char recvBuffer[8192];
};

using std::cout;
using std::endl;
using std::vector;
using std::thread;

SOCKET ghListenSocket; // Lisetn Socket
HANDLE ghIOCP; // IOCP Handle

vector<Session*> Sessions;

/** 임계구역의 진입을 제어하는 객체*/
std::mutex mtxForSession;

/** Session 을 추가합니다 */
void AddSession(Session* pSession_new)
{
	{
		std::lock_guard<std::mutex> lock(mtxForSession);
		Sessions.push_back(pSession_new);
	}

	printf("새로운 Client[%d] 가 연결되었습니다\n", (int)pSession_new->hSocketForClient);
}

/** Client Socket 과 연결을 끊습니다. */
void RemoveSession(SOCKET socketForClientToDelete)
{
	vector<Session*>::iterator SessionIter;
	{
		std::lock_guard<std::mutex> lock(mtxForSession);

		for (SessionIter = Sessions.begin(); Sessions.end() != SessionIter; ++SessionIter)
		{
			// 지우고자 Session 을 찾았다
			Session* pSession = *SessionIter;
			if (socketForClientToDelete == pSession->hSocketForClient)
			{
				delete pSession; // 메모리 해지
				Sessions.erase(SessionIter);
				break;
			}
		}
	}
}

/** 서버를 종료합니다. */
void ReleaseServer()
{
	// 모든 Client 종료
	vector<Session*>::iterator SessionIter;
	{
		std::lock_guard<std::mutex> lock(mtxForSession);

		for (SessionIter = Sessions.begin(); Sessions.end() != SessionIter; ++SessionIter)
		{
			::shutdown((*SessionIter)->hSocketForClient, SD_BOTH); // 종료요청
		}
	}
	::shutdown(ghListenSocket, 0);
	::closesocket(ghListenSocket);
	::CloseHandle(ghIOCP);
	cout << "서버를 종료하였습니다" << endl;

}

/** 모든 Client 에게 메시지를 보냅니다.
	@param SenderSocket : 메시지를 보낸 Client
	@param message : 보낼 메시지
*/
void SendMessageToAllClient(SOCKET SenderSocket, const char* message)
{
	vector<Session*>::iterator SessionIter;

	char* FinalMsg = new char[8192 + 15];

	sprintf_s(FinalMsg, 8192 + 15, "Client[%d] : %s \n", (int)SenderSocket, message);

	{
		std::lock_guard<std::mutex> lock(mtxForSession);

		for (SessionIter = Sessions.begin(); Sessions.end() != SessionIter; ++SessionIter)
		{
			::send((*SessionIter)->hSocketForClient, FinalMsg, strlen(FinalMsg) + 1, 0); // 메시지 보내기
		}
	}

	delete[] FinalMsg;
}

void ErrorHandler(const char* message)
{
	printf("ERROR : %s\n", message);

	ReleaseServer();
	::WSACleanup();

	exit(1);
}

DWORD threadCompletionRoutine()
{
	PULONG_PTR key = NULL;
	Session* pSession = NULL;
	SOCKET SocketForClient = { 0 };
	SOCKADDR_IN ClientInfo = { 0 };
	WSABUF wsaBuffer = { 0 };	// IOCP 에서 LOCK 할 메모리를 정의하기 위한 객체
	LPWSAOVERLAPPED lpWSAOverlapped = NULL;
	DWORD dwRecvByteData = 0;	// 수신한 데이터 크기
	DWORD flag = 0;

	printf("IOCP CompletionRoutine Thread 생성!\n");
	/*cout << "IOCP CompletionRoutine Thread 생성!" << endl;*/

	while (true)
	{
		// GetQueuedCompletionStatus

		// IOCP 가 닫히면 false 로 반환한다. IO request 가 완료 알림을 받을때까지 기다린다.
		bool GQCS_Result = GetQueuedCompletionStatus(ghIOCP, &dwRecvByteData, (PULONG_PTR)&pSession, &lpWSAOverlapped, INFINITE);
		/*pSession = (Session*)key;*/

		if (GQCS_Result)
		{

			if (0 >= dwRecvByteData)
			{
				// Client 의 접속 종료
				RemoveSession(SocketForClient);
			}
			else {

				// 데이터를 받았다.

				SendMessageToAllClient(pSession->hSocketForClient, pSession->recvBuffer);

				::ZeroMemory(pSession->recvBuffer, sizeof(8192));

				::ZeroMemory(lpWSAOverlapped, sizeof(WSAOVERLAPPED));

				wsaBuffer.buf = pSession->recvBuffer;
				wsaBuffer.len = sizeof(pSession->recvBuffer);

				dwRecvByteData = 0;
				flag = 0;

				::WSARecv(pSession->hSocketForClient, &wsaBuffer, 1, &dwRecvByteData, &flag, lpWSAOverlapped, NULL);
				int WSAErrorCode = WSAGetLastError();
				if (!(0 == WSAErrorCode || WSA_IO_PENDING == WSAErrorCode))
				{
					printf("WSA IO Request PENDING 에 실패하였습니다. ErrorCode : %d\n", WSAErrorCode);
				}
			}


		}
		else {
			// 비정상 결과

			if (lpWSAOverlapped != NULL)
			{
				// ClientSocket 의 비정상 종료 또는 ListenSocket 의 비정상 종료
				if (ghListenSocket != NULL)
				{
					// Client Socket 의 비정상 종료
					if (pSession != NULL)
					{
						RemoveSession(pSession->hSocketForClient);
					}
					else {
						cout << "Session 이 유효하지 않은 비정상 종료 발생" << endl;
					}
				}
				else {
					ReleaseServer();
					ErrorHandler("Listen Server 가 비정상 종료 되었습니다.");
					break;
				}

			}
			else {
				// 이경우 client 연결 이상은 아니다.
				// 즉 IOCP 가 닫혔거나, IOCP 의 문제가 생긴것이다.

				ReleaseServer();
				ErrorHandler("IOCP 가 비정상 종료 되었습니다.");
				break;

			}
		}
	}

	printf("CompletionRoutine Thread 종료\n");

	return 0;
}

// thread : Client 의 연결을 기다리는 Thread
DWORD threadAcceptLoop()
{
	SOCKET SocketForClient = { 0 };
	SOCKADDR_IN ClientInfo = { 0 };
	int SizeOfClientInfo = sizeof(ClientInfo);
	WSABUF wsaBuffer = { 0 };	// IOCP 에서 LOCK 할 메모리를 정의하기 위한 객체
	LPWSAOVERLAPPED lpWSAOverlapped = NULL;
	DWORD dwRecvByteData = 0;	// 수신한 데이터 크기
	DWORD flag = 0;


	printf("Client 연결을 대기합니다\n");
	while (SocketForClient = ::accept(ghListenSocket, (SOCKADDR*)&ClientInfo, &SizeOfClientInfo))
	{
		// Client 가 연결되었습니다.
		if (INVALID_SOCKET == SocketForClient) {
			// 유효하지 않은 소켓
			continue;
		}

		// Session 만들고 Sessions 에 추가
		Session* pSession = new Session;
		/*memset(pSession->recvBuffer, 0, sizeof(8192));*/
		::ZeroMemory(pSession, sizeof(Session));
		pSession->hSocketForClient = SocketForClient;
		/*::ZeroMemory((void*)pSession->recvBuffer, sizeof(8192));*/

		AddSession(pSession);

		// Socket 을 ICOP Queue 에 등록 . key 로 Session 의 주소를 지정
		CreateIoCompletionPort((HANDLE)SocketForClient, ghIOCP, (ULONG_PTR)pSession, 0);

		wsaBuffer.buf = pSession->recvBuffer; // socket recv Buffer 를 받는, Lock 할 메모리 지정. IOCP 에서 OS 가 Lock을 걸어 관리한다.
		wsaBuffer.len = sizeof(pSession->recvBuffer);

		// WSAOverlapped 생성 ( WSA 비동기 IO 요청에 필요한 정보 )
		lpWSAOverlapped = new WSAOVERLAPPED;
		::ZeroMemory(lpWSAOverlapped, sizeof(WSAOVERLAPPED)); // WSAOVERLAPPED 초기화

		flag = 0;
		dwRecvByteData = 0;

		// WSARecv 를 IOCP Queue 에 등록하기 . WSABUF 필요. NULL 로 설정하여 GetQueuedCompletionStatus 로 데이터를 받을 수 있도록 한다.
		::WSARecv(SocketForClient, &wsaBuffer, 1, &dwRecvByteData, &flag, lpWSAOverlapped, NULL);
		
		/*
			WSAGetLastError () 가
			0 : 이미 작업이 완료되었음을 의미한다. 아마 Recv를 걸자마자 바로 Recv 받아서 그런것이 아닐까?
			6 : 유효하지 않은 핸들이다. OVERLAPPED 초기화, WSA 설정 또한 잘 체크해보자
			997 (WSA_IO_PENDING) :  아직 비동기작업이 완료되지 않았을 뿐, 정상이다.
		*/
		int WSAErrorCode = WSAGetLastError();
		if (!(0 == WSAErrorCode || WSA_IO_PENDING == WSAErrorCode))
		{
			// 방금 추가한 IO 요청이 완료되었거나. Pending 상태가 아니라면
			printf("WSA IO Request PENDING 에 실패하였습니다. ErrorCode : %d\n", WSAErrorCode);
		}

	}

	return 0;
}




int main()
{

	// WSA 초기화
	WSADATA wsaData;
	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		ErrorHandler("WSA 를 초기화 하는데 실패하였습니다.");
	}

	// threadCompletionRoutine 생성
	vector<thread> vec_CompRoutineThread;

	vec_CompRoutineThread.reserve(MAX_IOCP_THREAD_COUNT);

	for (int i = 0; i < MAX_IOCP_THREAD_COUNT; ++i) {
		vec_CompRoutineThread.push_back(thread(threadCompletionRoutine));
	}

	// ICOP 생성
	ghIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


	// Listen Socket 생성 및 초기화
	ghListenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == ghListenSocket) {
		ErrorHandler("ListenSocket 을 생성하는데 실패하였습니다.");
	}

	SOCKADDR_IN ListenAddr = { 0 };
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
	ListenAddr.sin_port = htons(ServerPORT);

	if (SOCKET_ERROR == ::bind(ghListenSocket, (SOCKADDR*)&ListenAddr, sizeof(ListenAddr))) {
		ErrorHandler("ListenSocket 에 ip/port binding 실패");
	}

	if (SOCKET_ERROR == ::listen(ghListenSocket, SOMAXCONN)) {
		ErrorHandler("ListenSocket 을 Listen 상태로 전환하는데 실패하였습니다.");
	}

	cout << "Server 를 시작합니다" << endl;

	// ListenSocket Accept thread 생성
	thread hThreadAcceptLoop(threadAcceptLoop);

	// threadCompletionRoutine 이 끝나지 않도록
	char exit = 'x';
	while (true)
	{
		exit = getchar();
		if ('x' == exit)
		{
			break;
		}
	}

	ReleaseServer(); // ThreadAccpetLoop , threadCompletionRoutine 가 모두 종료될 것이다.
	::WSACleanup();

	for (int i = 0; i < MAX_IOCP_THREAD_COUNT; i++)
	{
		vec_CompRoutineThread[i].join();
	}
	hThreadAcceptLoop.join();

	cout << "Process 를 종료합니다" << endl;

	return 0;
}