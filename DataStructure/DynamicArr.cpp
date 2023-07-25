#include "DynamicArr.h"
#include <iostream>

void DArr_Init(DArr* DArr)
{
	// 초기화.
	DArr->pStorage = (int*)malloc(sizeof(int) * 2);
	DArr->iCount = 0;
	DArr->iMax = 2;
}

void DArr_Release(DArr* DArr)
{
	free(DArr->pStorage);
	DArr->iCount = 0;
	DArr->iMax = 0;
}

/* 배열 재할당 함수
   기존의 메모리 사용 공간을 해지하고
   좀더 큰 사이즈의 새로운 공간을 재할당 받습니다.
*/
void DArr_Reallocate(DArr* DArr) {
	int newSize = DArr->iMax * 2; // 기존 2배만큼 재할당 받을거야.
	int preIndex = DArr->iCount; // 기존 카운트
	int* newStorage = (int*)malloc(DArr->iMax * 2 * sizeof(int));

	// 새로운 공간에 데이터를 하나씩 옮겨
	for (int i = 0; i < preIndex; i++) {
		newStorage[i] = DArr->pStorage[i]; // 기존 배열에서 차례대로 데이터를 가져와 새로운 곳에 옮겨
	}
	// 다 옮겼으면 해지해
	free(DArr->pStorage);
	// 새로운 주소를 가져가
	DArr->pStorage = newStorage;
	// 최대 사이즈도 수정해주자
	DArr->iMax = newSize;

}

void DArr_PushData(DArr* DArr, int Data)
{
	if (DArr->iMax == DArr->iCount) {
		// 공간 확장이 필요하다
		int preSize = DArr->iMax;

		DArr_Reallocate(DArr);

		printf("공간이 추가 재할당 되었습니다. 사이즈 변경 ( %d -> %d )\n", preSize, DArr->iMax);
	}


	DArr->pStorage[DArr->iCount++] = Data; // 후위 연산자 사용하여 모든 식이 끝나면 ++
}



void DArr_PrintInfo(DArr* DArr) {

	printf("가변배열의 정보를 출력합니다. \n");
	printf("최대 사이즈 : %d\n사용 개수 : %d\n", DArr->iMax, DArr->iCount);
	for (int i = 0; i < DArr->iCount; i++) {
		printf("DArr[%d] = %d\n", i, DArr->pStorage[i]);
	}
	printf("이상입니다.\n");
}

void DArr_Sort(DArr* DArr)
{
	// 둘중 하나를 비교해서 큰놈을 다음 인덱스로 옮긴다.
	// 그러면 가장 큰놈의 위치가 정해진다.
	// 이것을 점점 index -- 하면서 반복한다.

	for (int i = 0; i < DArr->iCount; i++) {

		// 점점 줄어드는 반복횟수.
		// 0에서 시작하되 어짜피 가장 마지막은 정렬이 되어 있을 것이니 마지막은 빼고 반복한다.
		for (int k = 0; k < DArr->iCount - i - 1; k++) {

			// 만약 현재 숫자가 다음번째 숫자보다 크다면 자리바꿔
			if (DArr->pStorage[k] > DArr->pStorage[k + 1]) {
				int temp = DArr->pStorage[k];
				DArr->pStorage[k] = DArr->pStorage[k + 1];
				DArr->pStorage[k + 1] = temp;
			}

		}
	}
	printf("배열 정렬 완료");
	DArr_PrintInfo(DArr);
}

void customSort(int* arr, int icount, void(*SortFunc)(int*, int))
{
	// sort 함수 실행
	SortFunc(arr, icount);
}


