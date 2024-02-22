#include <WinSock2.h>
#include <stdio.h>

#pragma comment (lib, "ws2_32")

#define ServerIP "192.168.0.4"
#define ServerPort 25000

DWORD WINAPI Thread2_Listener(LPVOID pParam)
{
	SOCKET hClientSocket = (SOCKET)pParam;

	int nRecv = 0;
	char RecvBuffer[255] = { 0 };
	int RecvBufferSize = sizeof(RecvBuffer);

	while (0 < (nRecv = ::recv(hClientSocket, RecvBuffer, RecvBufferSize, 0)))
	{
		printf("%s\n", RecvBuffer);
		memset(RecvBuffer, 0, sizeof(RecvBuffer));
	}

	// 서버로부터 FIN 을 받았다면 소켓을 닫는다.
	::closesocket(hClientSocket);

	return 0;
}

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ClientSocket) {
		puts("소켓을 생성하는데 실패하였습니다.\n");
		return 0;
	}

	SOCKADDR_IN ServerAddr = { 0 };
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(ServerPort);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(ServerIP);

	if (SOCKET_ERROR == ::connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)))
	{
		puts("Server 와 연결하는데 실패하였습니다.\n");
		return 0;
	}

	DWORD hThreadID = 0;

	::CreateThread(nullptr, 0, Thread2_Listener, (LPVOID)ClientSocket, 0, &hThreadID);

	char InputBuffer[255] = { 0 };

	int i = 0;

	puts("Server 에 연결되었습니다. 메시지를 입력하세요\n");
	while (true)
	{

		/*gets_s(InputBuffer, sizeof(InputBuffer));*/

		sprintf_s(InputBuffer, sizeof(InputBuffer), "%d", i++);

		if (0 == strcmp(InputBuffer, "EXIT"))
		{
			puts("종료를 시작합니다.\n");
			break;
		}

		::send(ClientSocket, InputBuffer, strlen(InputBuffer) + 1, 0);

		memset(InputBuffer, 0, sizeof(InputBuffer));
		Sleep(1000);
	}

	::closesocket(ClientSocket);
	::Sleep(100);
	::WSACleanup();
	return 0;
}