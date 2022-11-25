#pragma once
/*
	LinkedList;

	heap 메모리 영역 상에 흩어진 배열들을 포인터로 연결시켜,
	단방향으로 연결 된 자료구조


	- 특정 순서에 데이터를 삽입할때 굉장히 유리한 자료구조
		- 데이터를 넣을때, 결국 이동만 잘해서 포인터로 연결만 시켜주면 된다.
		- 순서바꾸기 용이하다.
		- 포인터의 연결관계만 바꾸어 주면 된다. ( 데이터 순서 바꾸기가 동적배열보다 효율적이다. )
	- 포인터 연산이 불가능하다.
		- 주소연산 한번에 원하는 위치로 갈 수 없다.(가변베열이나 배열은 가능하다). 데이터의 개수가 영향을 준다.



*/

/* 단일 연결 리스트 노드

*/
typedef struct SListNode {

	int Data;
	SListNode* pNext;
	//struct *SListNode; // 아직 타입 제정의를 하기 전이기 때문에 struct 를 써주고 원래 이름을 써준다. 써줘야 하는데 에러가 생기네..

}SListNode;

/*	리스트 관리자

*/
typedef struct SList {

	int iCount;
	SListNode* pHeaderNode;

} SList;

/* 연결형 리스트 초기화 */
void SListInit(SList* pSList);

/* 데이터 넣기 */
void SListPushDataBack(SList* pSList, int Data);

/* 데이터 넣기 - 앞쪽에 */
void SListPushDataFront(SList* pSList, int Data);

/* 연결형 리스트 메모리 해제 */
void SListRelease(SList* pSList);

