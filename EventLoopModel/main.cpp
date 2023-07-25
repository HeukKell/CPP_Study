/*
	Event Loop 모델을 구현합니다.

	유저의 입력을 받음과 동시에
	뒤에서는 유저의 요청을 처리한다.
	유저의 입력을 blocking 없이 받아내면서,
	뒤에서는 유저의 입력을 순차적으로 처리하는 모델

	# main
		메인 함수
		UIThread 를 돌려 유저의 입력을 받아내면서, EventLoop 를 돌린다.
		EventDispatcher 를 통해 이벤트를 가져오면, EventDispatcher 를 호출한다.
		이를 반복한다.

	# UIThread
		유저의 입력을 받을때마다,
		Queue 에 이벤트를 추가한다.
		UIThread 에서 처리한다

	# EventDispatcher
		Quque 에 이벤트가 들어있으면, 이벤트를 꺼낸다.
		이벤트가 들어잇지 않으면, 들어올때까지 기다린다.
		이는 while 문으로 대기하다가 Event 가 있으면 반환하자.
		이는 이벤트 를 찾아오는 이벤트관리자 함수, EventDispatcher 가 처리한다

	# EventProcedure
		이벤트 를 꺼냈다면, 처리하는 로직도 구성하자.
		이벤트를 처리하는 함수는 EventProcedure 함수이다.


	# 역할을 정해보자

		- MainThread : 이벤트 루프를 실행하는 함수. 이벤트를 꺼내고, 이벤트를 처리하는 함수를 호출한다.
			- EventProcedure : 이벤트 처리기. main thread 에 의해 호출되어 이벤트를 받아 이벤트의 처리를 담당한다.
				- 일반 요청 : 바로바로 끝날수 있는 요청의 경우 동기적으로 처리하도록 한다.
				- FileIO : 비동기 파일입출력 함수. 별도의 thread 에서 처리하도록 한다
		- UIThread : 유저의 입력을 받아 이벤트를 추가한다.

*/
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <map>
#include <Windows.h>

using std::thread;
using std::queue;
using std::string;
using std::mutex;
using std::lock_guard;
using std::map;

#define LOCK(LockName,mutexInstance) lock_guard<mutex> LockName(mutexInstance)
#define EVENT_EXIT -1

using std::cout;

/*이벤트에 대한 정보를 가진 구조체*/
struct EventMessage
{
	int EventID; // 이벤트를 식별하기 위한 코드
	string EventName;

	int param;

	float param2;

	string param3;
};

// 이벤트를 담을 큐
queue<EventMessage> EventQueue;

// 가장 최근 이벤트를 가리키는 인덱스
int NextEventIdx = 0;
mutex mtx;

// 이벤트를 만드는 함수를 모아놓은 테이블
map<string, void(*)(EventMessage)> EFxTable;

void MakeEvent_Default(EventMessage EMsg);
void MakeEvent_Echo(EventMessage EMsg);
void MakeEvent_Add(EventMessage EMsg);
void MakeEvent_FileIO(EventMessage Event);
void MakeEvent_EXIT(EventMessage Event);

/* Event 처리기 */
void EventProcedure(EventMessage Event);

void EventDebug_Add(EventMessage Event)
{
	printf("\nEvent[%d](%s, %d, %.8f, %s) 가 추가되었습니다.\n",
		Event.EventID,
		Event.EventName.c_str(),
		Event.param,
		Event.param2,
		Event.param3.c_str()
	);
}

// 이벤트를 만드는 함수를 초기화 합니다
void AssignEventFunction()
{
	string EventName = "default";
	EFxTable.insert(std::pair<string, void(*)(EventMessage)>(EventName, MakeEvent_Default));

	EventName = "echo";
	EFxTable.insert(std::pair<string, void(*)(EventMessage)>(EventName, MakeEvent_Echo));

	EventName = "add";
	EFxTable.insert(std::pair<string, void(*)(EventMessage)>(EventName, MakeEvent_Add));

	EventName = "fileIO";
	EFxTable.insert(std::pair<string, void(*)(EventMessage)>(EventName, MakeEvent_FileIO));

	EventName = "EXIT";
	EFxTable.insert(std::pair<string, void(*)(EventMessage)>(EventName, MakeEvent_EXIT));

}

/* EventQueue 로부터 Message 를 하나 꺼내옵니다. */
EventMessage EventDispatcher()
{

	EventMessage Result;
	while (true)
	{
		{
			LOCK(lock, mtx);
			if (!(EventQueue.empty()))
			{
				Result = EventQueue.front();
				EventQueue.pop();
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	return Result;
}


// 유저의 입력을 받아 계속해서 이벤트를 담을 thread
void UIThread()
{

	AssignEventFunction();

	// 유저의 입력을 받을 버퍼
	char userInput[255] = { 0 };

	puts("유저의 입력을 대기합니다.\n");

	char EventName[255] = { 0 };

	while (true)
	{
		// 유저의 입vv력 지우기
		memset(EventName, 0, sizeof(EventName));

		EventMessage NewEvent = { 0 };

		fflush(stdin);

		// 유저의 입력이 있을때까지 대기했다가
		// 유저의 입력이 생기면 이벤트 추가
		scanf_s(" %s", EventName, sizeof(EventName));

		NewEvent.EventName = EventName;

		fflush(stdin);

		if (EFxTable.end() != (EFxTable.find(NewEvent.EventName)))
		{
			puts("이벤트 추가 시도\n");
			EFxTable[NewEvent.EventName](NewEvent);
			puts("이벤트가 추가되었습니다.\n");
		}
		else {
			puts("일치하는 이벤트가 없습니다.\n");
		}


	}
}

// 메인함수
int main()
{

	thread UIThreadInst(UIThread);
	EventMessage CurrentEvent;

	while (true)
	{
		printf("\n아래의 요청을 하세요\n");
		printf("요청 가능한 명령 : [ \n\tdefault : 기본 요청 ,\n\techo : 에코요청 ,\n\tadd : 2개 정수 더하기 요청,\n\tfileIO : (비동기)파일입출력 ,\n\tEXIT : 프로세스 종료 \n]\n");

		// Event 가 있을때까지 대기합니다.
		if (EVENT_EXIT != (CurrentEvent = EventDispatcher()).EventID)
		{
			EventProcedure(CurrentEvent);
		}
		else {
			break;
		}
	}

	UIThreadInst.join();


	return 0;
}


void MakeEvent_Default(EventMessage EMsg)
{
	puts("EventMake Default Called\n");

	EMsg.EventID = 1;
	EMsg.EventName = "default";

	{
		LOCK(lock, mtx);
		EventQueue.push(EMsg);
	}


	EventDebug_Add(EMsg);

}

void MakeEvent_Echo(EventMessage Event)
{
	Event.EventID = 2;
	Event.EventName = "echo";

	char msg[255] = { 0 };
	puts("메시지를 입력해주세요 : ");

	fflush(stdin);
	scanf_s("%s", msg, sizeof(msg));
	fflush(stdin);

	Event.param3 = msg;

	{
		LOCK(lock, mtx);
		EventQueue.push(Event);
	}

	EventDebug_Add(Event);
}

void MakeEvent_Add(EventMessage Event)
{
	Event.EventID = 3;
	Event.EventName = "add";

	int a = 0;
	int b = 0;

	fflush(stdin);
	puts("첫번째 인수1 를 넣어주세요 : ");
	scanf_s(" %d", &a, sizeof(a));
	fflush(stdin);
	puts("첫번째 인수2 를 넣어주세요 : ");
	scanf_s(" %d", &b, sizeof(b));
	fflush(stdin);

	Event.param = a + b;

	{
		LOCK(lock, mtx);
		EventQueue.push(Event);
	}

	EventDebug_Add(Event);
}

void MakeEvent_FileIO(EventMessage Event)
{
	Event.EventID = 4;
	Event.EventName = "fileIO";

	char fileName[255] = { 0 };

	puts("입출력할 파일명을 적어주세요 : ");
	fflush(stdin);
	scanf_s("%s", fileName, sizeof(fileName));
	fflush(stdin);

	Event.param3 = fileName;

	{
		LOCK(lock, mtx);
		EventQueue.push(Event);
	}

	EventDebug_Add(Event);
}

void MakeEvent_EXIT(EventMessage Event)
{
	Event.EventID = -1;

	{
		LOCK(lock, mtx);
		EventQueue.push(Event);
	}

	EventDebug_Add(Event);
}

DWORD WINAPI FileIO(LPVOID pParam)
{
	static unsigned int seed = 0;
	++seed;
	srand(seed);
	int randomInt = rand();

	char* fileName = (char*)pParam;

	LARGE_INTEGER start, end, timer;
	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);
	Sleep(1000 * (randomInt % 10));
	QueryPerformanceCounter(&end);

	float delay = (end.QuadPart - start.QuadPart) / (float)timer.QuadPart;

	printf("file '%s' 의 입출력이 완료되었습니다. (걸린시간 : %.2f) \n", fileName, delay);

	// fileName 해지
	delete[] fileName;

	return 0;
}


/* 이벤트 처리기 */
void EventProcedure(EventMessage Event) {

	puts("이벤트 처리기 호출\n");
	switch (Event.EventID)
	{
	case 1: {
		// default
		printf("Event Default 가 처리되었습니다.\n");

		break;
	}
	case 2: {
		// echo

		printf("Echo >> %s\n", Event.param3.c_str());
		break;
	}
	case 3: {
		// add

		printf("Add >> %d\n", Event.param);
		break;
	}
	case 4: {
		// FileIO

		char* fileName = new char[255];
		strcpy_s(fileName, 255, Event.param3.c_str());

		::CreateThread(
			NULL, 0, FileIO, (LPVOID)fileName, 0, NULL
		);

		break;
	}

	default: {

		break;
	}
	}
	return;
}