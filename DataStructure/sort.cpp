#include "sort.h"

/* 정렬::버블소트
		@param pArr 배열의 시작주소
		@param count 몇개의 요소가 있는지
*/
void bubbleSort(int* pArr, int iCount) {

	for (int i = iCount; i > 0; i--) {
		for (int k = 0; k < i - 1; k++) {

			// 왼쪽이 오른쪽보다 크다면 자리바꿔
			if (pArr[k] > pArr[k + 1]) {
				int temp = pArr[k];
				pArr[k] = pArr[k + 1];
				pArr[k + 1] = temp;
			}
		}
	}
}