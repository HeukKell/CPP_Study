#pragma once

#include <assert.h>
#include <stdio.h>

namespace heukkell {


	// ���� �迭

	template <typename T>
	class vector {

	private:

		T* pStorage;		// ������� �����ּ�

		int iCount;			// ���� ����ִ� ������ ����

		int iMaxSize;		// �����͸� ������ �ִ� �ִ� ����


	public:

		vector()
			: pStorage(nullptr)
			, iCount(0)
			, iMaxSize(2)
		{
			pStorage = new T[iMaxSize]; // �ΰ� �����͸� ���� �� �ֵ��� �Ҵ�
		}

		/* �ִ� ����� �����Ͽ� �����迭 �����ϱ�*/
		vector(int _iMaxSize)
			: pStorage(nullptr)
			, iCount(0)
			, iMaxSize(_iMaxSize)
		{
			pStorage = new T[iMaxSize];		// ������ ������ŭ �����迭 ����
		}

		~vector()
		{
			std::cout << "heap �޸� ����" << "0x" << pStorage << " �޸� ���� �Ǿ����ϴ�" << std::endl;
			delete[] pStorage;	// heap ���� ����

		}

	public:


		class iterator;

		/* ���� ����� ��ȯ�մϴ�*/
		int getSize() { return iMaxSize; };

		/* ���� ������ ������ ��ȯ */
		int getCount() { return iCount; };

		/* ������ �߰� */
		void push_back(const T& data);

		/* ������ ������Ʈ */
		void updateSize(int newSize);

		/* iterator �� ����Ű�� �����͸� �����մϴ�
			@Param _iter �����ϰ��� �ϴ� �����͸� ����Ű�� ���ͷ�����
			@Return itertor iterator �� ����Ű�� �����͸� �����ϰ� ���� �����ʹ� �ջ�ȴ�. ���ο� �����͸� ����Ű�� itertor �� ��ȯ�մϴ�.
		*/
		iterator erase(iterator& _iter);

		/* ���� ���ͷ����͸� ��ȯ�մϴ� */
		iterator begin();

		/* �� ���ͷ����͸� ��ȯ�մϴ� */
		iterator end();

		/* ��� �����͸� �ֿܼ� ǥ���մϴ� */
		void showAllData();

		/* ������ ���� */
		void clear();

		/* heap ������� Ư�� �ε����� �ش��ϴ� �����͸� ��ȯ�մϴ�. */
		T& operator [] (int idx)
		{
			assert(idx < iCount);

			return pStorage[idx];
		}

		/* �����͸� �߰��մϴ� */
		vector& operator << (const T& data)
		{
			push_back(data);
			return *this;
		}


		/* �ݺ���
		*  �����̳��� �����͸� ����Ű�� �ݺ���
		*/
		class iterator {
		private:

			friend class vector;


			vector* pContainer;			// �����̳�
			T* pContainerStorage;		// �����
			int idx;					// �ε���
			bool bValid;				// iteraetor �� ��ȿ��

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

			/* ���ͷ����Ͱ� ����Ű�� �����͸� ��ȯ�մϴ� */
			T& operator* ()
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				return pContainerStorage[idx];
			}

			/* ++ ���� ����
			*	�ε��� ����
			*/
			iterator& operator ++ ()
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				// �����̳��� iCount �� �Ѿ�� ������ ������� �ʴ´�. �ִ� iCount ������ �� �� �ִ�. �ִ� �ε����� iCount -1 ������,iCount �� �������� end iterator �� �ȴ�.
				assert(pContainer->iCount > idx);

				if (idx == pContainer->iCount - 1)
				{
					// �� ���� ������ �ε����� ���� �ߴٴ� ��, �׷��ٸ� ������ end iterator �� ���·� ���������.
					idx = -1;
					bValid = false;
				}
				else {
					++idx;
				}

				return *this;
			}

			/* ++ ���� ���� ����
			*	�ε��� ����, �켱 ������ ���ͷ������� ���纻 ��ȯ
			*/
			iterator operator ++ (int)
			{
				assert(nullptr != pContainer && nullptr != pContainerStorage && 0 <= idx && bValid);

				// ���纻�� �����
				vector<T>::iterator iterator_copy(*this); // ���� ���ͷ������� ���纻

				++(*this); // ���� ++ ����
				return iterator_copy;	// �켱 ���� iterator �� ��ȯ
			}

			/* = ���� ����
			* ���� ����...
			* ������ ���ͷ������� ���� �״�� �ִ´�?
			*/
			iterator& operator =(const vector<T>::iterator& _other)
			{
				// ���� �����̳��� ���ͷ����� ������ �����ϰ� �� ������.
				pContainer = _other.pContainer;
				pContainerStorage = _other.pContainerStorage;
				idx = _other.idx;
				bValid = _other.bValid;
				return *this;
			}

			/* == ����
			* ���� ���ͷ����Ϳ� �Է��� ���ͷ����Ͱ� ������ ���θ� ��ȯ
			*/
			bool operator ==(const vector<T>::iterator& _other)
			{
				if (_other.pContainer == pContainer && _other.pContainerStorage == pContainerStorage && _other.idx == idx && _other.bValid == bValid)
				{
					return true;
				}

				return false;
			}

			/* != ����
			*	�μ��� ���� ���ͷ����Ϳ� ���� �ʾƾ� true �� ��ȯ�Ѵ�.
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
		// ���� �����͸� ���� �� �ִ� �������� Ȯ�κ��� ����.
		if (iCount >= iMaxSize)
		{	// ���� ����� �����ϴٸ�
			updateSize(iMaxSize * 2); // �ι�� ���̱�
		}

		// ������ �Է�
		pStorage[iCount] = data;
		std::cout << "heap " << "0x" << pStorage << "[" << iCount << "] = " << data << std::endl;

		// ������ ���� �߰�
		++iCount;
	}

	template<typename T>
	void vector<T>::updateSize(int newSize)
	{
		// �Է��� ����� ������ ������� ������ �ȵ�. assert
		// ���� ���ο� ������ Ȯ���� ���� ( �Է��� ������ ��ŭ )
		// �����͵��� �� �������� �̵�
		// �׸��� ������ ���� ����

		assert(newSize > iMaxSize); // ���ο� ������� ���� �ִ������� Ŀ�� �մϴ�.

		T* pNewStorage = new T[newSize]; // heap ������ ���ο� ���� Ȯ��, �� ������ ��ŭ

		for (int i = 0; i < iCount; i++)
		{
			pNewStorage[i] = pStorage[i]; // ���� �����͸� �ϳ��� �̻�
		}

		std::cout << "heap �޸� ����" << "0x" << pStorage << " �޸� ���� �Ǿ����ϴ�" << std::endl;
		delete[] pStorage;	// �������� ����
		pStorage = pNewStorage; // ���� �缳��
		std::cout << "heap �޸� ����" << "0x" << pStorage << " �޸� �Ҵ� �Ǿ����ϴ�" << std::endl;
		iMaxSize = newSize; // ũ�� �缳��
	}

	template<typename T>
	typename vector<T>::iterator vector<T>::erase(iterator& _iter)
	{
		// ���� ���ͷ����Ͱ� ����Ű�� �����̳ʰ� �ڽŰ� ���ƾ� �ϸ�, storage �ּҵ� ���ƾ� �Ѵ�.
		// idx �� �ش��ϴ� �������� �ּҸ� ����ϴ�.
		// �׷���.. �����̳� ������ �����ϴ� ������ �̰ű���!. ����� �Ǹ� �ϳ��� ���� �;���. ��ܿ��� idx -= 1


		assert(this == _iter.pContainer
			&& pStorage == _iter.pContainerStorage
			&& iCount > _iter.idx
			&& _iter.bValid);
		// ���� ����

		// ������ �ε��� �������� �ݺ��Ѵ�
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
		// ���� ���ͷ����� ��ȯ
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
		std::cout << "��� �����͸� �������ϴ�" << std::endl;
		iCount = 0;
	}

}
