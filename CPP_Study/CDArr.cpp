#include "CDArr.h"
#include <assert.h>
#include <iostream>

void CDArr::pushBack(int data)
{
	if (currentCount >= maxSize)
	{
		// 영역 재설정
		resize(currentCount * 2);
	}

	DataArr[currentCount++] = data;

}

void CDArr::resize(int ExpantionSize)
{
	// 입력한 사이즈 만큼 확충
	// 하지만 지금개수보다 적으면 안돼.
	if (ExpantionSize <= currentCount) {
		assert(nullptr);
	}

	// 새 공간 확보
	int* newStorage = new int[ExpantionSize];

	// 데이터 이사
	for (int i = 0; i < currentCount; i++) {
		newStorage[i] = DataArr[i];
	}

	// 기존 공간 해지후 새공간 가리키기
	delete[] DataArr;
	DataArr = newStorage;

	maxSize = ExpantionSize; // 최대영역 재기입

}

CDArr::CDArr()
	: DataArr(nullptr),
	currentCount(0),
	maxSize(2)
{
	// 초기화
	// 2개가 들어갈 수 있을만큼 힙영역 확보
	DataArr = new int[2]; // int 2개 들어갈 영역 확보

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
