#pragma once
#include "pch.h"
#include <iostream>
#include <assert.h>

template <typename T>
class TemplateContainer {


private:
	T* pStorage; // ���� ������
	int iCount; // ���� ����
	int iMaxCount; // �ִ� ����

public:

	TemplateContainer();
	TemplateContainer(int initialSize);
	~TemplateContainer();

public:
	/* ������ �߰� �����Լ� */
	void updateSize(int newSize);

	/* ������ �߰� �Լ� */
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

	TemplateContainer<T>& operator << (const T Data);

	// �������� ������ ��ȯ
	// iterator �ʱ�ȭ�ÿ� ������ �ʱ� ���ͷ�����

	class iterator;

	iterator begin();

	iterator end();


	// ���� �����Ѵ�� ������ ������ ������ �����Ͱ� ���ܿ����� �װ��� ���Ͱ� ����Ű�� ������.
	// ���� ���ͷ����ʹ� �������� ���̻� ����� �� ���� ���ͷ����Ͱ� �ȴ�.
	// ���ο� ���ͷ����͸� ��ȯ�ϰ� �ִ�. ( ������ ���ͷ����ʹ� ����������. ���ο� �����͸� ����Ű�� ���ο� ���ͷ����� ( ����ȭ�� ) �� ��ȯ�Ѵ�.
	// �� �� ���ͷ����͸� �޾ƾ� ��.
	// å����
	iterator erase(iterator& _iter);

	void clear();

	/* �ݺ���
		inner class
		inner class �� �����ϰ� �ִ� class �� private ����� ������ �����ϴ�.
	*/
	class iterator {

	private:

		TemplateContainer* pContainer;	// iterator �� ����ų �����͸� �����ϴ� �����̳� �ּ�
		T* pContainerStorage;	// ����Ű�� �������� ���� �ּ�
		int idx;				// �������� ���� �ּҷκ��� ���° ���������� ����Ű������ �ε���. ( -1 �ϰ�� end iterator �� ���� )
		bool bValid;			// ���ͷ����Ͱ� ��ȿ�� ���ͷ��������� ǥ���ϱ� ���� ����

	public:

		friend class TemplateContainer; // ģ������. outer class ���� inner class �� private ����� �����Ҽ� �ִ� ����.
		// ������ ���� ������� �ʴ°��� ������. ���ʿ� private �� �������� �ҷ��� �ϴ� ���������������ε�.
		// ����Լ��� ���ؼ� �����ϴ°��� ����.
		// ������ �̰�� TemplateContainer �� inner class �� ����� �����̱� ������ �̷��� ������ ����ϵ��� ���ش�.

		iterator()
			:pContainer(nullptr)
			, pContainerStorage(nullptr)
			, idx(-1)
			, bValid(false)
		{

		}

		iterator(TemplateContainer* _pContainer, T* _pContainerStorage, int _idx)
			:pContainer(_pContainer)
			, pContainerStorage(_pContainerStorage)
			, idx(_idx)
			, bValid(false)
		{
			if (nullptr != pContainer && 0 <= _idx)
			{	// �������� �����̳ʰ� ���Դ��� üũ & �ε��� üũ
				bValid = true;
			}
		}

	public:

		/*
			�ݺ��ڸ� ������ �Ѵٸ�, ����Ű�� �ִ� ������ ������ ��ȯ�մϴ�.
		*/
		T& operator * ()
		{
			// iterator �� �˰��ִ� �ּҿ�, �����迭�� �˰� �ִ� �ּҰ� �޶��� ��� ( ���� Ȯ������ �ּҰ� �޶��� ��� )
			// idx �� �߿��Ѱ� �ƴ϶�, �޸��� �ּҵ� �߿���! ��� �� ��츦 ���� ����
			// ���� idx �� -1 �� end iterator �� ��쿡�� ������ �߻���Ų��.
			// ��ȿ�� ���¿��� �ϰ�
			// �׷��� �����Ͱ� �ϳ��� ���ٸ�, �ȵ�.
			if (pContainer->pStorage != pContainerStorage || -1 == idx || !bValid || pContainer->iCount == 0)
			{
				assert(nullptr);
			}


			return pContainerStorage[idx];
		}

		// ���� ++ ����
		iterator& operator ++()
		{
			// ���ܻ���
			// 1. 
			// 
			// 2.end iterator ���� ++ �� ��Ű�� ���� �߻���Ų��. 
			// ���� �ּҰ� �޶����ų�. end iterator ���.
			if (pContainer->pStorage != pContainerStorage || -1 == idx)
			{
				assert(nullptr);
			}
			else if (pContainer->iCount - 1 == idx) {
				// ���� ����Ű�� �ִ°��� ������ �ε��� �� ���
				idx = -1; // end iterator �� ����
			}
			else {
				++idx;
			}

			return *this;
		}

		// ���� ++ ����
		// ������ �����Ҽ��� ����. �츮�� ���� ������ �� ������ ȣ������� ����.
		// ������ ���踦 �޸� �ؾ� �Ѵ�.
		// ���� ������ ��� ���� �Ϸ�� ���� ���� ���߿� ����Ǵ°� ó�� ���δ�.
		// ������ �����δ� ����Լ�ó�� �����ϱ� ������ �ٷ� ȣ���� �ȴ�.
		// ���߿� ȣ��Ǵ°� ó�� ���̱� ����
		// ������ ����Ǳ� ������ ���¸� ����޾� �� ���纻�� ��ȯ�ϰ�, ������ ���� ������Ű��. ��ġ ���߿� ����Ǵ°� ó�� ���δ�.
		// �׷��� ������ ���߿� ������, �ϴ� ���纻�� ���� ������� �ش� ���纻�� ������ ��� ������ �����ϰ�, ������� �� ������ �ȴ�. ������ �ڷδ� �̹� ������ ������ ������ �����̴�.
		// ��ġ �㹰������� ���� ���� ������ �����ϴ� �׷� �׸�
		// 
		// �ᱹ �̶����� Ư���� �ǵ��� ���ٸ� �������� ������ ȣ���� ����ϴ� ������ ���⿡ �ִ�.
		// �������� ��ü �������, ������, ��������� �߻��ϱ� ����
		// 
		// �ƹ� �̸��� �ǹ̵� ���� int �� �μ��� �־��ָ� �����Ϸ��� ������������ �����Ѵٰ� �Ѵ�. ������ ����ΰ� ����
		// �ٸ� �ڷ����� �ȵȴ�. �ݵ�� int ���� ����Ȱ� ����. C2807
		iterator operator ++(int)
		{
			// �켱 ��������� ���纻�� �޴´�.
			iterator copycurrent = *this; // ���� ������ ���纻
			++(*this); // ���� ������ ���������� ��Ų��.
			return copycurrent; // ���纻�� ��������.
		}


		// ���� -- ����
		iterator& operator --()
		{
			// ���ܻ���
			// begin iterator ���� -- �� ��Ű�� ������ �߻���Ų��.
			// begin iterator �� idx �� 0 �϶��� begin, �׸���, -1 �϶��� ������.
			if (0 >= idx) {
				assert(nullptr);
			}
			--idx;

			return *this;
		}

		bool operator ==(const iterator& other) {

			// ����Ű�� �������� �����ּҰ� ����, idx �� ���ƾ���
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
TemplateContainer<T>::TemplateContainer()
	:pStorage(nullptr)
	, iCount(0)
	, iMaxCount(0)

{
	pStorage = new T[2]; // �������� �ΰ� ������ ���� Ȯ��.
	iMaxCount = 2;

}

template<typename T>
TemplateContainer<T>::TemplateContainer(int initialSize)
	:pStorage(nullptr)
	, iCount(0)
	, iMaxCount(initialSize)
{
	pStorage = new T[initialSize];
}

template<typename T>
TemplateContainer<T>::~TemplateContainer()
{
	delete[] pStorage;
}

template<typename T>
void TemplateContainer<T>::updateSize(int newSize)
{
	// ���� Ȯ�� ���� ���� �۰� ����¡�Ѵٸ� ���� �߻�
	if (newSize < iMaxCount)
	{
		assert(nullptr);
	}

	// �� �а� Ȯ���Ѵٸ�
	// �ι� ���� ������� Ȯ����
	// �̻��ϰ�
	// �������� �㹰��
	T* newStorage = new T[iMaxCount * 2];

	for (int i = 0; i < iCount; i++) {
		newStorage[i] = pStorage[i]; // �̻�
	}

	delete[] pStorage;
	pStorage = newStorage;
	iMaxCount = iMaxCount * 2;
}

template<typename T>
void TemplateContainer<T>::push_back(T Data)
{

	if (iMaxCount == iCount) {
		updateSize(iMaxCount * 2);
	}

	pStorage[iCount++] = Data;
}

template<typename T>
T& TemplateContainer<T>::operator[](int idx)
{
	return pStorage[idx];
}

template<typename T>
TemplateContainer<T>& TemplateContainer<T>::operator<<(const T Data)
{
	push_back(Data);
	return *this;
}

template<typename T>
typename TemplateContainer<T>::iterator TemplateContainer<T>::begin()
{
	/*
		���� �����Ͱ� �ϳ��� �������.
		������ �� end
		end iterator �� ��ȯ�մϴ�
	*/
	if (0 == iCount) {
		return iterator(this, pStorage, -1);	// �����Ͱ� ���°��, begin() == end()
	}
	else {
		return iterator(this, pStorage, 0);
	}

}

template<typename T>
typename TemplateContainer<T>::iterator TemplateContainer<T>::end()
{
	return iterator(this, pStorage, -1);	// �����Ͱ� ���°��, begin() == end()
}

/*
	iterator �� ����Ű�� �ִ� �����͸� ������ �ְڴ�.
*/
template<typename T>
typename TemplateContainer<T>::iterator TemplateContainer<T>::erase(iterator& _iter)
{
	// �����̳� �ּ�
	// �ٸ� �����̳ʿ� ���踦 ���� �ִ°�� ����� �� ����!
	// ���� end iterator ��� ����� �� ����.
	assert(this == _iter.pContainer && end() != _iter && iCount > _iter.idx);


	// ���ŵ� ���� �����͸� ��ĭ�� ��ܿ���
	// ī��Ʈ�� �ϳ� �ٿ��ְ�
	// �׷��� ���� ���� ���ص� �ǳ�? ��¥�� ��ȸ�� �� �ִ� �����ʹ� �ε����� ��������, count�� ���� �ش��ϴ� �ε����� ���� �߰��� �����Ͱ� ���� �������ϱ�
	// �ݺ�Ƚ����. iCount - _iter.idx - 1 ��ŭ ��ܿ��°��� �ݺ��ϸ� ��
	// ������ ���� �������� �ִ� �����͸� ������ ���, �׳� ������ �����͸� �����ϰ� ī��Ʈ�� �ٿ��ָ��. 
	// ���� �̷��� ���� ������ �������ε��� ������ ���� �ε����� �����͸� ���� ���鼭 ������ �߻��� ���� �ִ�.
	if (iCount - 1 == _iter.idx)
	{
		// ��¥�� ������ ���� �����ͷ� ���� �����̴� �׳� �״�� �ΰ� ���߿� ī��Ʈ�� ���̸� �ǰڴ�.
	}
	else {
		int loopCount = iCount - (_iter.idx + 1); // �ݺ�Ƚ��

		for (int i = 0; i < loopCount; i++)
		{
			pStorage[i + _iter.idx] = pStorage[i + 1 + _iter.idx];  // n ��° �����͸� n+1 ��° �����ͷ� ���� �����
		}
	}


	// ����� iter �� ��������.


	--iCount;

	// ���ο� ���ͷ����� ��ȯ
	// ��ǻ� �����迭���� ������ �ε����� �״�� �����ѵ� ������ ���� �ϴ�. ����ϴ� ���ͷ����͸� ��� �ᵵ ������ ���ٴ� ���̴�.
	// ������ ������ ���ͷ����͸� ������� ���ϰ� �ؾ��Ѵ�.
	// �ֳ��ϸ�, �� ��Ȳ�� �� �����̳ʰ� �迭�̱� ������ ����� ��Ȳ�̰�, �ٸ� �ڷᱸ��������, ������ �ε����� ���� ���ͷ����͸� ����� �� ���� �����̴�. list ����
	// ������, �ٸ� �ڷᱸ������ ���ϼ��� ���߾� �ֱ� ���ؼ�
	// ������ ���ͷ����͸� ������� ���ϵ��� ����. ���������� �˷�����.
	_iter.bValid = false;

	return iterator(this, pStorage, _iter.idx);
}

template<typename T>
void TemplateContainer<T>::clear()
{
	// ��¥�� �� �����ɷ� �ϰ� ó������ ä�������� �׸��̱⿡
	// count �� 0 ���� ������ش�.
	iCount = 0;
}

/*
inner class �� �ڽ��� �����ϴ� class �� priavte �� ������ �� ������.
class �� inner class �� private �� ������ �� ����.


 */

 // template ��°��� ���ʿ� ��������� ���� �ڵ��̱� ������, intellisence �� �۵��ϱ� �����.