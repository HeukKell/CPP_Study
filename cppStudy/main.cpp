#include "pch.h"
#include <iostream>
#include <thread>

#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

/** Multithread 환경에서 안전하게 사용할 수 있는 Queue, Stack */
LockQueue<int> gLockQueue;
LockStack<int> gLockStack;

// 
/*
	리터럴 연산자의 매개변수는
	문자열 또는 unsigned long long (64 bit 정수) 만 가능하다.

	또한 특정 클래스의 멤버함수로 사용할 수 없으며,
	꼭 클래스의 멤버함수로 넣고 싶다면, constexpr 를 붙여 일반함수? 로 지정해야 한다고 한다.
*/
constexpr std::chrono::microseconds operator"" ms(unsigned long long value)
{
	return std::chrono::microseconds(value);
}


// # Thread 1
void PushThread()
{

	while (true)
	{
		int value = rand() % 100;
		gLockQueue.Push(value);
		std::cout << "데이터를 넣었습니다 : " << value<< ", size : " << gLockQueue.getSize()  << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

// # Thread 2
void PopThread()
{
	while (true)
	{
		int data = 0; // data 를 받을 변수 
		gLockQueue.WaitPop(data); // 데이터가 들어올때까지 기다렷다가 받기
		//if (gLockQueue.TryPop(data))
		//{
		//	std::cout << "데이터를 꺼냈습니다 : " << data << std::endl;
		//}

		std::cout << "데이터를 꺼냈습니다 : " << data << std::endl;
	}

}


// # Thread 0 
int main()
{

	std::thread t1(PushThread);
	std::thread t2(PopThread);

	t1.join();
	t2.join();
		


	return 0;
}