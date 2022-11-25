#include "CDArr.h"
#include <assert.h>
#include <iostream>

void CDArr::pushBack(int data)
{
	if (currentCount >= maxSize)
	{
		// ���� �缳��
		resize(currentCount * 2);
	}

	DataArr[currentCount++] = data;

}

void CDArr::resize(int ExpantionSize)
{
	// �Է��� ������ ��ŭ Ȯ��
	// ������ ���ݰ������� ������ �ȵ�.
	if (ExpantionSize <= currentCount) {
		assert(nullptr);
	}

	// �� ���� Ȯ��
	int* newStorage = new int[ExpantionSize];

	// ������ �̻�
	for (int i = 0; i < currentCount; i++) {
		newStorage[i] = DataArr[i];
	}

	// ���� ���� ������ ������ ����Ű��
	delete[] DataArr;
	DataArr = newStorage;

	maxSize = ExpantionSize; // �ִ뿵�� �����

}

CDArr::CDArr()
	: DataArr(nullptr),
	currentCount(0),
	maxSize(2)
{
	// �ʱ�ȭ
	// 2���� �� �� ������ŭ ������ Ȯ��
	DataArr = new int[2]; // int 2�� �� ���� Ȯ��

}

void CDArr::showDatas()
{
	for (int i = 0; i < currentCount; i++) {
		printf("data[%d] : %d\n", i, DataArr[i]);
	}
}

int& CDArr::operator[](int index)
{
	return DataArr[index];
}

CDArr::~CDArr()
{
	delete[] DataArr;
}
