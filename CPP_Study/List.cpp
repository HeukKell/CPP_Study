#include "List.h"
#include <iostream>

void SListInit(SList* pSList)
{
	pSList->pHeaderNode = nullptr;
	pSList->iCount = 0;
}

void SListPushDataBack(SList* pSList, int Data)
{
	// �������� �Ҵ�� ��� ����ü�� ��� �ִ� �����ͺ���
	SListNode* pNewNode = (SListNode*)malloc(sizeof(SListNode) * 1); // heap �������ٰ� �����Ѵ�.

	// node ����
	pNewNode->Data = Data;
	pNewNode->pNext = nullptr;

	// ó���϶� : ����� �Ӹ��Ѵ�.
	// ó���� �ƴҶ� : �ݺ���, ���� iCount ��°(���� ���������)���� �������� ���� ���� ��带 ����Ų��. 
	if (0 == pSList->iCount) {
		// ù��° ��带 �ִ´ٸ�?
		pSList->pHeaderNode = pNewNode; // ���� ���� ��带 ������� ����
	}
	else {
		// ó���� �ƴ϶��,
		// �ϴ� ó�� ����� �ּҸ� �����ϰ�, �ݺ��ؼ� �̵�����. ���� ������ ���� ( pNext �� null ���� Ȯ���ص� �ǰ�, iCount �� Ȯ���ص� �ǰ�)
		SListNode* pCurrentNode = pSList->pHeaderNode; // �ϴ� ó�� 
		/*
			for (int i = 0; i < pSList->iCount-1 ; i++) {
				pCurrentNode = pCurrentNode->pNext;// ���� ���� �̵�
			}
		*/
		while (pCurrentNode->pNext) { // nullptr �� �ƴ϶�� �ݺ�
			pCurrentNode = pCurrentNode->pNext; // �̵�
		}

		pCurrentNode->pNext = pNewNode; // ���� ���������� �̵��ߴٸ�, ���� ���� ����� �ּҸ� ���� �ּҷ� �����Ѵ�.
	}

	++pSList->iCount;
}

void SListPushDataFront(SList* pSList, int Data)
{
	// ���ο� ��� �������.
	SListNode* pNewNode = (SListNode*)malloc(sizeof(SListNode) * 1); // �������� �Ҵ�
	pNewNode->Data = Data;
	pNewNode->pNext = nullptr;

	// ���ʿ��ٰ� ��������..
	// �ϳ��� ������ �׳� �ش����� �Ӹ��ϸ� �ǰ�.
	// �ټ����, �ش� ���� �Ӹ��ϰ� ���� �ش� ��带 ����Ű�� �ǰڴ�.
	// �׸��� count ++

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
	SListNode* currentNode = pSList->pHeaderNode; // ��� �޾Ƴ���
	while (currentNode) {

		SListNode* pNext = currentNode->pNext; // �����ּҸ� �޾Ƴ���
		free(currentNode); // ����༮ �����
		currentNode = pNext;
	}
}
