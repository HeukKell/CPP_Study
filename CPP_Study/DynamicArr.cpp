#include "DynamicArr.h"
#include <iostream>

void DArr_Init(DArr* DArr)
{
	// �ʱ�ȭ.
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

/* �迭 ���Ҵ� �Լ�
   ������ �޸� ��� ������ �����ϰ�
   ���� ū �������� ���ο� ������ ���Ҵ� �޽��ϴ�.
*/
void DArr_Reallocate(DArr* DArr) {
	int newSize = DArr->iMax * 2; // ���� 2�踸ŭ ���Ҵ� �����ž�.
	int preIndex = DArr->iCount; // ���� ī��Ʈ
	int* newStorage = (int*)malloc(DArr->iMax * 2 * sizeof(int));

	// ���ο� ������ �����͸� �ϳ��� �Ű�
	for (int i = 0; i < preIndex; i++) {
		newStorage[i] = DArr->pStorage[i]; // ���� �迭���� ���ʴ�� �����͸� ������ ���ο� ���� �Ű�
	}
	// �� �Ű����� ������
	free(DArr->pStorage);
	// ���ο� �ּҸ� ������
	DArr->pStorage = newStorage;
	// �ִ� ����� ����������
	DArr->iMax = newSize;

}

void DArr_PushData(DArr* DArr, int Data)
{
	if (DArr->iMax == DArr->iCount) {
		// ���� Ȯ���� �ʿ��ϴ�
		int preSize = DArr->iMax;

		DArr_Reallocate(DArr);

		printf("������ �߰� ���Ҵ� �Ǿ����ϴ�. ������ ���� ( %d -> %d )\n", preSize, DArr->iMax);
	}


	DArr->pStorage[DArr->iCount++] = Data; // ���� ������ ����Ͽ� ��� ���� ������ ++
}



void DArr_PrintInfo(DArr* DArr) {

	printf("�����迭�� ������ ����մϴ�. \n");
	printf("�ִ� ������ : %d\n��� ���� : %d\n", DArr->iMax, DArr->iCount);
	for (int i = 0; i < DArr->iCount; i++) {
		printf("DArr[%d] = %d\n", i, DArr->pStorage[i]);
	}
	printf("�̻��Դϴ�.\n");
}

void DArr_Sort(DArr* DArr)
{
	// ���� �ϳ��� ���ؼ� ū���� ���� �ε����� �ű��.
	// �׷��� ���� ū���� ��ġ�� ��������.
	// �̰��� ���� index -- �ϸ鼭 �ݺ��Ѵ�.

	for (int i = 0; i < DArr->iCount; i++) {

		// ���� �پ��� �ݺ�Ƚ��.
		// 0���� �����ϵ� ��¥�� ���� �������� ������ �Ǿ� ���� ���̴� �������� ���� �ݺ��Ѵ�.
		for (int k = 0; k < DArr->iCount - i - 1; k++) {

			// ���� ���� ���ڰ� ������° ���ں��� ũ�ٸ� �ڸ��ٲ�
			if (DArr->pStorage[k] > DArr->pStorage[k + 1]) {
				int temp = DArr->pStorage[k];
				DArr->pStorage[k] = DArr->pStorage[k + 1];
				DArr->pStorage[k + 1] = temp;
			}

		}
	}
	printf("�迭 ���� �Ϸ�");
	DArr_PrintInfo(DArr);
}

void customSort(int* arr, int icount, void(*SortFunc)(int*, int))
{
	// sort �Լ� ����
	SortFunc(arr, icount);
}


