#include "sort.h"

/* ����::�����Ʈ
		@param pArr �迭�� �����ּ�
		@param count ��� ��Ұ� �ִ���
*/
void bubbleSort(int* pArr, int iCount) {

	for (int i = iCount; i > 0; i--) {
		for (int k = 0; k < i - 1; k++) {

			// ������ �����ʺ��� ũ�ٸ� �ڸ��ٲ�
			if (pArr[k] > pArr[k + 1]) {
				int temp = pArr[k];
				pArr[k] = pArr[k + 1];
				pArr[k + 1] = temp;
			}
		}
	}
}