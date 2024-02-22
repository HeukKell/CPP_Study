#pragma once
#include <stack>
#include <mutex>
#include <atomic>

using std::stack;
using std::mutex;
using std::lock_guard;
using std::condition_variable;
using std::atomic;

using uint32 = __int32;


/*
	Stack 이나 Queue 는 데이터를 꺼낼때
	fornt 또는 top 을 통해 데이터를 엿보고
	이후에 pop 을 하여 꺼내는 방식으로
	2단계에 걸쳐서 데이터를 꺼낸다.

	그 이유는, 데이터를 꺼내는 와중에, 다른 thread 에서 접근하게되면
	Exception 충돌이 발생할 수 있기 때문이다.

	자체적으로 Lock 을 걸어줌으로서 혹시나 생길지 모르는 DeadLock 를 예방할 수 있다.
*/

template<typename T>
class LockStack
{
public:

	LockStack() { }

	// 복사는 MultiThread 환경에서 취약하다
	LockStack(const LockStack&) = delete; // 복사생성자 사용하지 않는다.
	LockStack& operator=(const LockStack&) = delete; // 대입연산자 사용하지 않는다.


public:

	void Push(T value)
	{
		lock_guard<mutex> lock(_mtx);
		_stack.push(std::move(value)); // 소유권을 이전시키는 방식으로 데이터를 옮긴다. 더 빠른 입력
		_condiVar.notify_one();
	}

	/**
		SingleThread 에서의
		Empty -> Top -> Pop
		의 과정을 한번에 처리하는 함수 	
		
		
		*/
	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mtx);
		if (_stack.empty()) {
			return false;
		}
			
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	/**
		데이터가 생길때까지 대기했다가 꺼내는 함수		
		@param value stack 에서 꺼낸값을 value 에다가 넣어줍니다.
	*/
	void WaitPop(T& value)
	{
		std::unique_lock<mutex> lock(_mtx);
		
		_condiVar.wait(lock, [this]() {return _stack.empty() == false;}); // 데이터가 있을때까지 대기
		value = std::move(_stack.top());
		_stack.pop();
	}


	/**
		사실상 MultiThread 환경에서 Empty 를 호출하여 확인하는것은 의미가 없다.
		Empty 를 확인하는 하여 다음코드로 진입하는 순간, 다른 Thread 에서 채워넣을 수 있기 때문에.
		Empty 후 Pop 을 사용하는것은 의미가 없다.		*/
	bool Empty()
	{
		lock_guard<mutex> lock(_mtx);
		return _stack.empty();
	}

	int getSize()
	{
		return _stack.size();
	}

private:

	condition_variable _condiVar; // 동기화 객체. 조건식과 lock 이 필요하다. 내부적으로 Lock 을 걸기때문에, 인수를 unique_lock 으로 받는다.
	stack<T> _stack;
	mutex _mtx;

};

/*
	Lock 을 사용하지 않는 Stack
	
	동기화를 맞춰주기 위해 여러가지 작업들이 필요하다.

	구현 난이도가 복잡하지만, 그렇다고 성능이 잘 나오기가 어렵다.

	stl stack 을 사용하는 것이 아닌 따로 자료구조를 구현해주어야 한다.

	Lock free 라고 해서 경합이 일어나지 않는것이 아니다. 명시적으로 이루어 지지 않을뿐이지.
*/
template<typename T>
class LockFreeStack {

	struct Node {
		Node(const T& value) : data(value), next(nullptr)
		{

		}

		T data;
		Node* next;
	};

public:

	void Push(const T& value)
	{
		Node* node = new Node(value); // 이거 자체는 문제없다. node 이라는 포인터 자체는 stack 영역에 있기 때문에, 다른 thread 가 접근할 수 없다.
		node->next = _head;
		{
			/*
				방법 1 : 일반적인 stack

				아래의 경우는 문제가 있다. 일반적이 stack 이라면 아래와 같이 했겠지만,
				나는 _head 가 가장 맨 위에 있는 노드라고 기대하고 있지만, 다른 thread 가 접근하기 때문에 아닐 수 있다.
				그러니까 내가 알고 있는 _head 는 진짜 head 가 아닐수 있다는 것이다.
				다른 thread 의 접근 여부를 생각하고, 내가 생각하는 값이 맞는지 의심하는 눈이 필요하다.

				node->next = _head;
				// 이 단계에서 새치기 당한다면? head 가 원래 있던 값이 아니라, 다른 thread 가 head 를 바꾸었다면?
				_head = node;

			*/
		}

		{
			/*
				방법 2 : atomic :: Campare And Swap

				// CAS 원리
				atomic<int> current;
				current.compare_exchange_weak(int& old , int new )
				{
					if( current == old ) {
						current = new;
						return true;
					}
					else{
						old = current;
						return false;
					}
				}


				// 적용
				if(_head == node->next)
				{
					_head = node;
					return true;
				}
				else{
					node->next = _head;
					return false;
				}

			*/
		}

		// 방법2 수행
		while (false == _head.compare_exchange_weak(node->next, node))
		{
			// node->next = _head;
		}
	}


	bool TryPop(T& value)
	{
		/*
			1. head 읽기
			2. head->next 읽기
			3. head = head->next;
			4. data 추출해서 반환
			5. 추출한 노드 삭제
		*/

		++_popCount;

		Node* oldHead = _head;

		while (oldHead && false == _head.compare_exchange_weak(oldHead, oldHead->next)) {


		}

		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}

		// 복사하는 과정에서 발생하는 Exception 을 신경을 쓰지 않는다. 터지게 둔다.
		value = oldHead->data;

		/*
			여기까지 왔다면, 일단 제거할 노드 자체는 빼낸 상태
			다른 thread 가 oldHead 를 참조하기 있을 수 있기 때문에,
			누군가가 참조하지 않을 때 까지 기다렸다가 삭제되도록 해야 한다.

			스마트포인터의 레퍼런스 카운팅의 원리를 생각해보자.
			Head 를 참조하고 있는 개수가 없을때를 찾으면된다.
			그럼 TryPop 에 접근하고 Thread 의 개수를 세면 된다.

			그래서, TryPop 의 각 위치에 _popCount 를 설치하여, 진입한 thread 수를 체크하고,
			TryDelete 에서 _popCount 를 체크하여, 다른 thread 가 참조하고 있지 않을때 삭제를 시도한다.
		*/
		// delete oldHead;
		TryDelete(oldHead); // 삭제를 해볼려고 시도를 한다.



		return true;
	}

	/*
		# 진행과정
			1. 데이터 분리
			2. Count 체크 ( 1번과 순서를 바꾸면 안된다.)
			3. 나 혼자면 삭제
	*/
	void TryDelete(Node* oldHead)
	{
		// 나 외에 누가 head 에 접근하고 있는지
		if (_popCount == 1)
		{
			// 혼자 점유중인경우

			/*
				이 지점까지 들어왔다면, 이후 thread 는 내가 발견한 oldHead 에 해당하는 head 를 발견할 수 없다.
				compare_exchange_weak 지점에서, 이미 _head 는 oldHead->next , 즉 다음 노드로 바뀌었기 때문.
				그래서 , 만약 이지점까지 왔는데도, pop count 가 혼자다. 라고 하면, oldHead 는 나만 알고 있는 Head 라고 할 수 있다.
			*/

			// 혼자인거, 삭제 예약된 다른 데이터들도 삭제해보자.
			// 삭제 예약된 노드들 모두를 꺼내오고 ( 하나만 꺼내면 next 에 줄줄이 연결되어 있는 상태 ), _pendingList 는 nullptr 이 된 상태
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				// 이 시점에서, 끼어든 thread 가 없다면, -> 삭제 진행
				// 이제와서  끼어들어도, 이미 데이터는 분리해 둔 상태!
				DeleteNode(node);
			}
			else {
				// 누가 끼어들었으니 다시 갖다 놓자
				// 다른 thread 가 수행하기를 기대 하는 상황
			}

			// 내 데이터는 삭제
			delete oldHead;
		}
		else {

		}
	}

	 // 어떤 노드와 , 그 노드에 연결된 모든 node 를 삭제하는 함수
	static void DeleteNode(Node* node)
	{
		// [ ] - [ ] - [ ] - [...] 

		while (node)
		{
			Node* nodeToDelete = node;
			node = node->next;
			delete nodeToDelete;
		}
	}

	/* Node 를 다시 연결해주는 함수 */
	void ChainPendingNodeList(Node* first, Node* last)
	{
		// 내가 알고 있는 노드
		//  [a] - [b] - [b] - [c] - [d] - [e] - [f] 
		// [first]                           [last] -> [next]
		//	                         [_pendingList] -> [g] - [h] - [i] - [j] - [k] 

		last->next = _pendingList;

		while (false == _pendingList.compare_exchange_weak(last->next, first)) // 현재 _pendingList 가 , 내가알고 있는 _pendingList(last->next) 임을 기대한다. 아니라면, 새로운 _pendingList 를 알게되고 반복
		{

		}
	}

	/* 위 ChaninPendingNodeList 의 Helper 함수 */
	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next){
			last = last->next;		// 마지막까지 이동
		}

		ChainPendingNodeList(node, last);
	}

	// Node 가 딱 하나밖에 없는 경우, 연결
	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}



private:

	// head
	// head 가 가리키는 곳에 추가하고, head 가 가리키는 노드를 빼면 Stack 이다.
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0; // Pop을 실행중인 Thread 개수. ( 목표는 _head 다. Push 의 경우, 자기자신만 접근할 수 있는 변수를 다루기때문에 고려하지 않고, Pop 의 경우 시작부터 Head 를 참조하기 때문에, TryPop 함수에 들어간 thread 의 개수를 센다. )
	atomic<Node*> _pendingList; // 삭제되어야 할 노드들 (첫번째 노드)
};