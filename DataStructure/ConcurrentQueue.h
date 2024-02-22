#pragma once
#include <mutex>
#include <queue>

using std::queue;
using std::mutex;
using std::lock_guard;
using std::condition_variable;

/*
	순차적으로 처리해야 하는 데이터의 경우 Queue 를 사용한다.
	게임쪽에서는 Client 로부터 날아오는 패킷을 순차적으로 처리해야 하므로, Stack 보단 Queue 를 많이 사용한다.

*/

template<typename T>
class LockQueue
{
public:

	LockQueue() { };

	// 복사는 MultiThread 환경에서 취약하기 때문에 삭제
	LockQueue(const LockQueue&) = delete;
	LockQueue& operator=(const LockQueue&) = delete;

public:

	/**
		인수로 지정된 값을 queue 에 psuh 합니다. (소유권이전)
		사용된 인수는 더이상 사용할 수 없습니다.		*/
	void Push(T value) {
		lock_guard<mutex> lock(_mtx);
		_queue.push(std::move(value));
		_condiVar.notify_one();
	}

	/* 비어있는지 확인 후 Pop 한다
		@param value queue 에서 꺼낸 값을 넣어줍니다.
	*/
	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mtx);

		// 비어있는지 확인
		if (_queue.empty()) { return false; }

		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	/* 데이터가 들어올때까지 기다렸다가 Pop 한다
		@param value queue 에서 꺼낸 값을 넣어줍니다.
	*/
	void WaitPop(T& value)
	{
		std::unique_lock<mutex> lock(_mtx);

		_condiVar.wait(lock, [this]() { return _queue.empty() == false; }); // 데이터가 들어올때까지 대기

		value = std::move(_queue.front());
		_queue.pop();
	}

	/** 사이즈 반환 */
	int getSize()
	{
		return _queue.size();
	}

private:

	queue<T> _queue;
	mutex _mtx;
	condition_variable _condiVar;

};