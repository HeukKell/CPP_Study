#pragma once

/*	가변배열 자료형
*/
typedef struct DynamicArr {

	int* pStorage;	// 데이터창고 주소
	int iCount;	// 다음 저장될 인덱스,이자 저장량
	int iMax;	// 최대 저장량

} DArr;


/* DArr 자료형을 초기화 하는 함수
*/
void DArr_Init(DArr* DArr);

/* 메모리 배열 해지 함수*/
void DArr_Release(DArr* DArr);

/* 데이터 추가 함수
	@Param DArr 데이터를 맡길 객체의 주소
	@Param Data 맡길 데이터
*/
void DArr_PushData(DArr* DArr, int Data);

// 외부 공개를 하지 않기 위해 주석처리, 및 헤더에만 지우기
///* 배열 재할당 함수
//   기존의 메모리 사용 공간을 해지하고
//   좀더 큰 사이즈의 새로운 공간을 재할당 받습니다.
//*/
//void DArr_Reallocate(DArr* DArr);

/* 가변 배열의 정보를 출력합니다.
*/
void DArr_PrintInfo(DArr* DArr);


/* 오름차순으로 정렬 합니다.
*/
void DArr_Sort(DArr* DArr);

/*소트 함수를 받는 함수*/
void customSort(int* arr, int icount, void(*SortFunc)(int*, int));