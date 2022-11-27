#pragma once

#include <assert.h>
#include <stdio.h>

namespace heukkell {


	// 가변 배열

	template <typename T>
	class vector {

	private:

		T* pStorage;		// 저장소의 시작주소

		int iCount;			// 현재 들어있는 데이터 개수

		int iMaxSize;		// 데이터를 넣을수 있는 최대 개수


	public:

		vector()
			: pStorage(nullptr)
			, iCount(0)
			, iMaxSize(2)
		{
			pStorage = new T[iMaxSize]; // 두개 데이터를 넣을 수 있도록 할당
		}

		/* 최대 사이즈를 지정하여 가변배열 선언하기*/
		vector(int _iMaxSize)
			: pStorage(nullptr)
			, iCount(0)
			, iMaxSize(_iMaxSize)
		{
			pStorage = new T[iMaxSize];		// 지정한 개수만큼 가변배열 선언
		}

		~vector()
		{
			std::cout << "heap 메모리 영역" << "0x" << pStorage << " 메모리 해지 되었습니다" << std::endl;
			delete[] pStorage;	// heap 공간 해지

		}

	public:


		class iterator;

		/* 현재 사이즈를 반환합니다*/
		int getSize() { return iMaxSize; };

		/* 현재 데이터 개수를 반환 */
		int getCount() { return iCount; };

		/* 데이터 추가 */
		void push_back(const T& data);

		/* 사이즈 업데이트 */
		void updateSize(int newSize);

		/* iterator 가 가리키는 데이터를 삭제합니다
			@Param _iter 삭제하고자 하는 데이터를 가리키는 이터레이터
			@Return itertor iterator 가 가리키던 데이터를 삭제하고 기존 데이터는 손상된다. 새로운 데이터를 가리키는 itertor 를 반환합니다.
		*/
		iterator erase(iterator& _iter);

		/* 시작 이터레이터를 반환합니다 */
		iterator begin();

		/* 끝 이터레이터를 반환합니다 */
		iterator end();

		/* 모든 데이터를 콘솔에 표시합니다 */
		void showAllData();

		/* 데이터 비우기 */
		void clear();

		/* heap 저장소의 특정 인덱스에 해당하는 데이터를 반환합니다. */
		T& operator [] (int idx)
		{
			assert(idx < iCount);

			return pStorage[idx];
		}

		/* 데이터를 추가합니다 */
		vector& operator << (const T& data)
		{
			push_back(data);
			return *this;
		}


		/* 반복자
		*  컨테이너의 데이터를 가리키는 반복자
		*/
		class iterator {
		private:

			friend class vector;


			vector* pContainer;			// 컨테이너
			T* pContainerStorage;		// 저장소
			int idx;					// 인덱스
			bool bValid;				// iteraetor 의 유효성

		public:

			iterator()
				: pContainer(nullptr)
				, pContainerStorage(nullptr)
				, idx(-1)
				, bValid(false)
			{

			}

			iterator(vector* _pContainer, T* _pStorage, int _idx)
				: pContainer(_pContainer)
				, pContainerStorage(_pStorage)
				, idx(_idx)
				, bValid(false)
			{
				if (nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx)
				{
					bValid = true;
				}
			}

			/* 이터레이터가 가리키는 데이터를 반환합니다 */
			T& operator* ()
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				return pContainerStorage[idx];
			}

			/* ++ 연산 구현
			*	인덱스 증가
			*/
			iterator& operator ++ ()
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				// 컨테이너의 iCount 를 넘어서는 증감은 허용하지 않는다. 최대 iCount 까지만 될 수 있다. 최대 인덱스는 iCount -1 이지만,iCount 와 같아질때 end iterator 가 된다.
				assert(pContainer->iCount > idx);

				if (idx == pContainer->iCount - 1)
				{
					// 즉 현재 마지막 인덱스에 도달 했다는 것, 그렇다면 다음은 end iterator 의 상태로 만들어주자.
					idx = -1;
					bValid = false;
				}
				else {
					++idx;
				}

				return *this;
			}

			/* ++ 연산 후위 구현
			*	인덱스 증가, 우선 현재의 이터레이터의 복사본 반환
			*/
			iterator operator ++ (int)
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				// 복사본을 만들기
				vector<T>::iterator iterator_copy(*this); // 현재 이터레이터의 복사본

				++(*this); // 몰래 ++ 연산
				return iterator_copy;	// 우선 현재 iterator 를 반환
			}

			/* = 연산 구현
			* 대입 연산...
			* 현재의 이터레이터의 값을 그대로 넣는다?
			*/
			iterator& operator =(const vector<T>::iterator& _other)
			{
				// 같은 컨테이너의 이터레이터 끼리만 가능하게 할 것인지.
				pContainer = _other.pContainer;
				pContainerStorage = _other.pContainerStorage;
				idx = _other.idx;
				bValid = _other.bValid;
				return *this;
			}

			/* == 연산
			* 현재 이터레이터와 입력한 이터레이터가 같은지 여부를 반환
			*/
			bool operator ==(const vector<T>::iterator& _other)
			{
				if (_other.pContainer == pContainer && _other.pContainerStorage == pContainerStorage && _other.idx == idx && _other.bValid == bValid)
				{
					return true;
				}

				return false;
			}

			/* != 연산
			*	인수로 넣은 이터레이터와 같지 않아야 true 를 반환한다.
			*/
			bool operator !=(const vector<T>::iterator& _other)
			{
				return !(*this == _other);
			}
		};

	};

	template<typename T>
	void vector<T>::push_back(const T& data)
	{
		// 현재 데이터를 넣을 수 있는 상태인지 확인부터 하자.
		if (iCount >= iMaxSize)
		{	// 만약 사이즈가 부족하다면
			updateSize(iMaxSize * 2); // 두배로 늘이기
		}

		// 데이터 입력
		pStorage[iCount] = data;
		std::cout << "heap " << "0x" << pStorage << "[" << iCount << "] = " << data << std::endl;

		// 데이터 개수 추가
		++iCount;
	}

	template<typename T>
	void vector<T>::updateSize(int newSize)
	{
		// 입력한 사이즈가 기존의 사이즈보다 작으면 안돼. assert
		// 먼저 새로운 공간을 확보한 다음 ( 입력한 사이즈 만큼 )
		// 데이터들을 새 공간으로 이동
		// 그리고 기존의 공간 해지

		assert(newSize > iMaxSize); // 새로운 사이즈는 기존 최대사이즈보다 커야 합니다.

		T* pNewStorage = new T[newSize]; // heap 공간에 새로운 공간 확보, 새 사이즈 만큼

		for (int i = 0; i < iCount; i++)
		{
			pNewStorage[i] = pStorage[i]; // 현재 데이터를 하나씩 이사
		}

		std::cout << "heap 메모리 영역" << "0x" << pStorage << " 메모리 해지 되었습니다" << std::endl;
		delete[] pStorage;	// 기존공간 해지
		pStorage = pNewStorage; // 공간 재설정
		std::cout << "heap 메모리 영역" << "0x" << pStorage << " 메모리 할당 되었습니다" << std::endl;
		iMaxSize = newSize; // 크기 재설정
	}

	template<typename T>
	typename vector<T>::iterator vector<T>::erase(iterator& _iter)
	{
		// 현재 이터레이터가 가리키는 컨테이너가 자신과 같아야 하며, storage 주소도 같아야 한다.
		// idx 에 해당하는 데이터의 주소를 지웁니다.
		// 그러면.. 컨테이너 내에서 삭제하는 로직이 이거구나!. 지우게 되면 하나씩 땡겨 와야지. 댕겨오고 idx -= 1


		assert(this == _iter.pContainer
			&& pStorage == _iter.pContainerStorage
			&& iCount > _iter.idx
			&& _iter.bValid);
		// 여기 까지

		// 마지막 인덱스 전까지만 반복한다
		for (int i = _iter.idx; i < iCount - 1; i++)
		{
			pStorage[i] = pStorage[i + 1];
		}


		iCount -= 1;
		_iter.bValid = false;

		return iterator(this, pStorage, _iter.idx);
	}

	template<typename T>
	typename vector<T>::iterator vector<T>::begin()
	{
		// 시작 이터레이터 반환
		return  iCount > 0 ? iterator(this, pStorage, 0) : iterator(this, pStorage, -1);
	}

	template<typename T>
	typename vector<T>::iterator vector<T>::end()
	{
		return iterator(this, pStorage, -1);
	}

	template<typename T>
	void vector<T>::showAllData()
	{
		std::cout << std::endl;

		for (int i = 0; i < iCount; i++)
		{
			std::cout << pStorage << "[" << i << "]" << "=" << pStorage[i] << std::endl;
		}

		std::cout << std::endl;
	}

	template<typename T>
	void vector<T>::clear()
	{
		std::cout << "모든 데이터를 지웠습니다" << std::endl;
		iCount = 0;
	}

}
