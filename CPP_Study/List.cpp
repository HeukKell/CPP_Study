#include "List.h"
#include <iostream>

void SListInit(SList* pSList)
{
	pSList->pHeaderNode = nullptr;
	pSList->iCount = 0;
}

void SListPushDataBack(SList* pSList, int Data)
{
	// 힙영역에 할당된 노드 구조체를 들고 있는 포인터변수
	SListNode* pNewNode = (SListNode*)malloc(sizeof(SListNode) * 1); // heap 영역에다가 생성한다.

	// node 세팅
	pNewNode->Data = Data;
	pNewNode->pNext = nullptr;

	// 처음일때 : 헤더로 임명한다.
	// 처음이 아닐때 : 반복문, 현재 iCount 번째(가장 마지막노드)에서 다음노드로 현재 만든 노드를 가리킨다. 
	if (0 == pSList->iCount) {
		// 첫번째 노드를 넣는다면?
		pSList->pHeaderNode = pNewNode; // 새로 만든 노드를 헤더노드로 지정
	}
	else {
		// 처음이 아니라면,
		// 일단 처음 노드의 주소를 저장하고, 반복해서 이동하자. 가장 마지막 노드로 ( pNext 가 null 인지 확인해도 되고, iCount 로 확인해도 되고)
		SListNode* pCurrentNode = pSList->pHeaderNode; // 일단 처음 
		/*
			for (int i = 0; i < pSList->iCount-1 ; i++) {
				pCurrentNode = pCurrentNode->pNext;// 다음 노드로 이동
			}
		*/
		while (pCurrentNode->pNext) { // nullptr 이 아니라면 반복
			pCurrentNode = pCurrentNode->pNext; // 이동
		}

		pCurrentNode->pNext = pNewNode; // 가장 마지막으로 이동했다면, 새로 만든 노드의 주소를 다음 주소로 설정한다.
	}

	++pSList->iCount;
}

void SListPushDataFront(SList* pSList, int Data)
{
	// 새로운 노드 만들었다.
	SListNode* pNewNode = (SListNode*)malloc(sizeof(SListNode) * 1); // 힙영역에 할당
	pNewNode->Data = Data;
	pNewNode->pNext = nullptr;

	// 앞쪽에다가 넣을껀데..
	// 하나도 없을땐 그냥 해더노드로 임명하면 되고.
	// 다수라면, 해더 노드로 임명하고 기존 해더 노드를 가리키면 되겠다.
	// 그리고 count ++

	if (0 == pSList->iCount) {
		pSList->pHeaderNode = pNewNode;
	}
	else {
		pNewNode->pNext = pSList->pHeaderNode;
		pSList->pHeaderNode = pNewNode;
	}

	++pSList->iCount;


}

void SListRelease(SList* pSList)
{
	SListNode* currentNode = pSList->pHeaderNode; // 노드 받아놓고
	while (currentNode) {

		SListNode* pNext = currentNode->pNext; // 다음주소를 받아놓고
		free(currentNode); // 현재녀석 지우고
		currentNode = pNext;
	}
}
