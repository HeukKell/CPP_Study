#pragma once
/*
	LinkedList;

	heap �޸� ���� �� ����� �迭���� �����ͷ� �������,
	�ܹ������� ���� �� �ڷᱸ��


	- Ư�� ������ �����͸� �����Ҷ� ������ ������ �ڷᱸ��
		- �����͸� ������, �ᱹ �̵��� ���ؼ� �����ͷ� ���Ḹ �����ָ� �ȴ�.
		- �����ٲٱ� �����ϴ�.
		- �������� ������踸 �ٲپ� �ָ� �ȴ�. ( ������ ���� �ٲٱⰡ �����迭���� ȿ�����̴�. )
	- ������ ������ �Ұ����ϴ�.
		- �ּҿ��� �ѹ��� ���ϴ� ��ġ�� �� �� ����.(���������̳� �迭�� �����ϴ�). �������� ������ ������ �ش�.



*/

/* ���� ���� ����Ʈ ���

*/
typedef struct SListNode {

	int Data;
	SListNode* pNext;
	//struct *SListNode; // ���� Ÿ�� �����Ǹ� �ϱ� ���̱� ������ struct �� ���ְ� ���� �̸��� ���ش�. ����� �ϴµ� ������ �����..

}SListNode;

/*	����Ʈ ������

*/
typedef struct SList {

	int iCount;
	SListNode* pHeaderNode;

} SList;

/* ������ ����Ʈ �ʱ�ȭ */
void SListInit(SList* pSList);

/* ������ �ֱ� */
void SListPushDataBack(SList* pSList, int Data);

/* ������ �ֱ� - ���ʿ� */
void SListPushDataFront(SList* pSList, int Data);

/* ������ ����Ʈ �޸� ���� */
void SListRelease(SList* pSList);

