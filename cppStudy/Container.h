#pragma once

#include <iostream>
#include <assert.h>

template <typename T>
class Container {


private:
	T* pStorage; // 저장 데이터
	int iCount; // 현재 개수
	int iMaxCount; // 최대 개수

public:

	Container();
	Container(int initialSize);
	~Container();

public:
	/* 사이즈 추가 설정함수 */
	void updateSize(int newSize);

	/* 데이터 추가 함수 */
	void push_back(T Data);

	int getCurrentCount() const {
		return iCount;
	};

	void ShowDatas() {

		int i = 0;

		for (int i = 0; i < iCount; i++) {
			printf("storage[%d] = %d\n", i, pStorage[i]);
		}
	}

	T& operator [] (int idx);

	Container<T>& operator << (const T Data);

	// 데이터의 시작점 반환
	// iterator 초기화시에 대입할 초기 이터레이터

	class iterator;

	iterator begin();

	iterator end();


	// 값은 예상한대로 삭제된 데이터 다음의 데이터가 땡겨와져서 그것을 이터가 가리키고 있지만.
	// 사용된 이터레이터는 망가져서 더이상 사용할 수 없는 이터레이터가 된다.
	// 새로운 이터레이터를 반환하고 있다. ( 기존의 이터레이터는 망가졌으니. 새로운 데이터를 가리키는 새로운 이터레이터 ( 정상화된 ) 를 반환한다.
	// 즉 이 이터레이터를 받아야 해.
	// 책갈피
	iterator erase(iterator& _iter);

	void clear();

	/* 반복자
		inner class
		inner class 를 포함하고 있는 class 의 private 멤버에 접근이 가능하다.
	*/
	class iterator {

	private:

		Container* pContainer;	// iterator 가 가리킬 데이터를 관리하는 컨테이너 주소
		T* pContainerStorage;	// 가리키는 데이터의 시작 주소
		int idx;				// 데이터의 시작 주소로부터 몇번째 데이터인지 가리키기위한 인덱스. ( -1 일경우 end iterator 라 하자 )
		bool bValid;			// 이터레이터가 유효한 이터레이터인지 표시하기 위한 변수

	public:

		friend class Container; // 친구선언. outer class 에서 inner class 의 private 멤버도 접근할수 있는 사이.
		// 원래는 많이 사용하지 않는것이 안좋다. 애초에 private 는 접근제한 할려고 하는 접근제어지시자인데.
		// 멤버함수를 통해서 접근하는것이 좋다.
		// 하지만 이경우 container 와 inner class 는 긴밀한 관계이기 때문에 이렇게 접근을 허용하도록 해준다.

		iterator()
			:pContainer(nullptr)
			, pContainerStorage(nullptr)
			, idx(-1)
			, bValid(false)
		{

		}

		iterator(Container* _pContainer, T* _pContainerStorage, int _idx)
			:pContainer(_pContainer)
			, pContainerStorage(_pContainerStorage)
			, idx(_idx)
			, bValid(false)
		{
			if (nullptr != pContainer && 0 <= _idx)
			{	// 정상적인 컨테이너가 들어왔는지 체크 & 인덱스 체크
				bValid = true;
			}
		}

	public:

		/*
			반복자를 역참조 한다면, 가리키고 있는 데이터 원본을 반환합니다.
		*/
		T& operator * ()
		{
			// iterator 가 알고있는 주소와, 가변배열이 알고 있는 주소가 달라진 경우 ( 공간 확장으로 주소가 달라진 경우 )
			// idx 만 중요한게 아니라, 메모리의 주소도 중요해! 라고 할 경우를 위한 설계
			// 또한 idx 가 -1 즉 end iterator 일 경우에도 문제를 발생시킨다.
			// 유효한 상태여야 하고
			// 그런데 데이터가 하나도 없다면, 안돼.
			if (pContainer->pStorage != pContainerStorage || -1 == idx || !bValid || pContainer->iCount == 0)
			{
				assert(nullptr);
			}


			return pContainerStorage[idx];
		}

		// 전위 ++ 연산
		iterator& operator ++()
		{
			// 예외사항
			// 1. 
			// 
			// 2.end iterator 에서 ++ 을 시키면 에러 발생시킨다. 
			// 시작 주소가 달라졌거나. end iterator 경우.
			if (pContainer->pStorage != pContainerStorage || -1 == idx)
			{
				assert(nullptr);
			}
			else if (pContainer->iCount - 1 == idx) {
				// 현재 가리키고 있는곳이 마지막 인덱스 인 경우
				idx = -1; // end iterator 로 설정
			}
			else {
				++idx;
			}

			return *this;
		}

		// 후위 ++ 연산
		// 시점을 설정할수는 없다. 우리가 만든 전위나 이 후위나 호출시점은 같다.
		// 하지만 설계를 달리 해야 한다.
		// 후위 연산은 모든 식이 완료된 다음 가장 나중에 수행되는것 처럼 보인다.
		// 하지만 실제로는 멤버함수처럼 동작하기 때문에 바로 호출이 된다.
		// 나중에 호출되는것 처럼 보이기 위해
		// 연산이 수행되기 직전의 상태를 복사받아 그 복사본을 반환하고, 원본은 몰래 증가시키면. 마치 나중에 수행되는것 처럼 보인다.
		// 그렇게 원본은 나중에 빼놓고, 일단 복사본을 받은 명령줄은 해당 복사본을 가지고 모든 연산을 수행하고, 명령줄이 다 끝나게 된다. 하지만 뒤로는 이미 원본은 연산을 수행한 상태이다.
		// 마치 허물벗어놓고 몰래 숨어 뒷일을 진행하는 그런 그림
		// 
		// 결국 이때문에 특별한 의도가 없다면 후위보다 전위를 호출을 장려하는 이유가 여기에 있다.
		// 쓸데없는 객체 생성비용, 복사비용, 삭제비용이 발생하기 때문
		// 
		// 아무 이름도 의미도 없는 int 를 인수로 넣어주면 컴파일러가 후위연산으로 생각한다고 한다. 일종의 약속인가 보다
		// 다른 자료형은 안된다. 반드시 int 만이 허락된것 같다. C2807
		iterator operator ++(int)
		{
			// 우선 현재상태의 복사본을 받는다.
			iterator copycurrent = *this; // 현재 상태의 복사본
			++(*this); // 몰래 원본은 전위연산을 시킨다.
			return copycurrent; // 복사본을 내보낸다.
		}


		// 전위 -- 연산
		iterator& operator --()
		{
			// 예외사항
			// begin iterator 에서 -- 을 시키면 에러를 발생시킨다.
			// begin iterator 는 idx 가 0 일때가 begin, 그리고, -1 일때도 문제다.
			if (0 >= idx) {
				assert(nullptr);
			}
			--idx;

			return *this;
		}

		bool operator ==(const iterator& other) {

			// 가리키는 데이터의 시작주소가 같고, idx 도 같아야해
			if (pContainerStorage == other.pContainerStorage && idx == other.idx)
			{
				return true;
			}

			return false;
		}

		bool operator != (const iterator& other) {
			return !(*this == other);
		}



	};



};


template<typename T>
Container<T>::Container()
	:pStorage(nullptr)
	, iCount(0)
	, iMaxCount(0)

{
	pStorage = new T[2]; // 힙영역에 두개 저장할 영역 확보.
	iMaxCount = 2;

}

template<typename T>
Container<T>::Container(int initialSize)
	:pStorage(nullptr)
	, iCount(0)
	, iMaxCount(initialSize)
{
	pStorage = new T[initialSize];
}

template<typename T>
Container<T>::~Container()
{
	delete[] pStorage;
}

template<typename T>
void Container<T>::updateSize(int newSize)
{
	// 현재 확보 개수 보다 작게 사이징한다면 에러 발생
	if (newSize < iMaxCount)
	{
		assert(nullptr);
	}

	// 더 넓게 확장한다면
	// 두배 넒은 저장공관 확보후
	// 이사하고
	// 기존공간 허물고
	T* newStorage = new T[iMaxCount * 2];

	for (int i = 0; i < iCount; i++) {
		newStorage[i] = pStorage[i]; // 이사
	}

	delete[] pStorage;
	pStorage = newStorage;
	iMaxCount = iMaxCount * 2;
}

template<typename T>
void Container<T>::push_back(T Data)
{

	if (iMaxCount == iCount) {
		updateSize(iMaxCount * 2);
	}

	pStorage[iCount++] = Data;
}

template<typename T>
T& Container<T>::operator[](int idx)
{
	return pStorage[idx];
}

template<typename T>
Container<T>& Container<T>::operator<<(const T Data)
{
	push_back(Data);
	return *this;
}

template<typename T>
typename Container<T>::iterator Container<T>::begin()
{
	/*
		만약 데이터가 하나도 없을경우.
		시작이 곧 end
		end iterator 를 반환합니다
	*/
	if (0 == iCount) {
		return iterator(this, pStorage, -1);	// 데이터가 없는경우, begin() == end()
	}
	else {
		return iterator(this, pStorage, 0);
	}

}

template<typename T>
typename Container<T>::iterator Container<T>::end()
{
	return iterator(this, pStorage, -1);	// 데이터가 없는경우, begin() == end()
}

/*
	iterator 가 가리키고 있는 데이터를 삭제해 주겠다.
*/
template<typename T>
typename Container<T>::iterator Container<T>::erase(iterator& _iter)
{
	// 컨테이너 주소
	// 다른 컨테이너와 관계를 갖고 있는경우 허용할 수 없다!
	// 또한 end iterator 라면 허용할 수 없다.
	assert(this == _iter.pContainer && end() != _iter && iCount > _iter.idx);


	// 제거된 다음 데이터를 한칸씩 당겨오고
	// 카운트도 하나 줄여주고
	// 그러면 구지 삭제 안해도 되네? 어짜피 순회할 수 있는 데이터는 인덱스로 정해지고, count의 값에 해당하는 인덱스는 다음 추가될 데이터가 덮어 씌워지니까
	// 반복횟수는. iCount - _iter.idx - 1 만큼 당겨오는것을 반복하면 돼
	// 하지만 가장 마지막에 있는 데이터를 삭제할 경우, 그냥 마지막 데이터만 삭제하고 카운트만 줄여주면돼. 
	// 만약 이렇게 하지 않으면 마지막인데도 마지막 다음 인덱스의 데이터를 덮어 쓰면서 문제가 발생할 수도 있다.
	if (iCount - 1 == _iter.idx)
	{
		// 어짜피 다음에 들어올 데이터로 덮어 쓸것이니 그냥 그대로 두고 나중에 카운트만 줄이면 되겠다.
	}
	else {
		int loopCount = iCount - (_iter.idx + 1); // 반복횟수

		for (int i = 0; i < loopCount; i++)
		{
			pStorage[i + _iter.idx] = pStorage[i + 1 + _iter.idx];  // n 번째 데이터를 n+1 번째 데이터로 덮어 씌우기
		}
	}


	// 사용한 iter 는 망가진다.


	--iCount;

	// 새로운 이터레이터 반환
	// 사실상 가변배열에서 삭제된 인덱스를 그대로 가리켜도 문제가 없긴 하다. 사용하던 이터레이터를 계속 써도 문제는 없다는 말이다.
	// 하지만 기존의 이터레이터를 사용하지 못하게 해야한다.
	// 왜냐하면, 이 상황은 이 컨테이너가 배열이기 때문에 생기는 상황이고, 다른 자료구조에서는, 삭제된 인덱스를 가진 이터레이터를 사용할 수 없기 때문이다. list 같은
	// 때문에, 다른 자료구조와의 통일성을 맞추어 주기 위해서
	// 기존의 이터레이터를 사용하지 못하도록 하자. 망가졌음을 알려주자.
	_iter.bValid = false;

	return iterator(this, pStorage, _iter.idx);
}

template<typename T>
void Container<T>::clear()
{
	// 어짜피 다 없던걸로 하고 처음부터 채워나가면 그만이기에
	// count 만 0 으로 만들어준다.
	iCount = 0;
}

/*
inner class 는 자신을 포함하는 class 의 priavte 에 접근할 수 잇지만.
class 는 inner class 의 private 에 접근할 수 없다.


 */

 // template 라는것이 애초에 만드어지지 않은 코드이기 때문에, intellisence 가 작동하기 힘들다.