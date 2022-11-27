#pragma once
class CDArr
{
private:

	int* DataArr;
	int currentCount;
	int maxSize;

public:
	CDArr();

	/* 데이터 추가.
		@param data 입력할 데이터
	*/
	void pushBack(int data);

	/*영역 확장
		@param ExpantionSize 인수만큼 영역 확장
	*/
	void resize(int ExpantionSize);

	/* 현재 있는 데이터를 모두 출력합니다. */
	void showDatas();

	/* 클래스를 배열처럼 사용할 수 있도록 하는 연산자오버로딩
	 배열에서의 주소연산과 같이

	CDArr arr;
	arr[1] = 0;

	처럼 사용할 수 있도록 한다.
	해당 주소의 값에 접근하면서도 값을 변경하기 위해 배열연산과 모양을 같게 하기 위해 포인터가 아닌 참조자를 반환합니다.
	해당 반환값이 곧 접근하고자 하는 변수 자체이며, 이를 수정하는것 역시 접근하고자 하는곳의 값을 변경하는것처럼 합니다.
	참조자는 주소를 받는것이 아니라. 그영역 자체를 받으므로, DataArr[index] 으로 초기화 하여 반한한다.
	@param index 접근하고자 하는 인덱스
	*/
	int& operator[] (int index);

	~CDArr();

};

