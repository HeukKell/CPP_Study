#pragma once
class CDArr
{
private:

	int* DataArr;
	int currentCount;
	int maxSize;

public:
	CDArr();

	/* ������ �߰�.
		@param data �Է��� ������
	*/
	void pushBack(int data);

	/*���� Ȯ��
		@param ExpantionSize �μ���ŭ ���� Ȯ��
	*/
	void resize(int ExpantionSize);

	/* ���� �ִ� �����͸� ��� ����մϴ�. */
	void showDatas();

	/* Ŭ������ �迭ó�� ����� �� �ֵ��� �ϴ� �����ڿ����ε�
	 �迭������ �ּҿ���� ����

	CDArr arr;
	arr[1] = 0;

	ó�� ����� �� �ֵ��� �Ѵ�.
	�ش� �ּ��� ���� �����ϸ鼭�� ���� �����ϱ� ���� �迭����� ����� ���� �ϱ� ���� �����Ͱ� �ƴ� �����ڸ� ��ȯ�մϴ�.
	�ش� ��ȯ���� �� �����ϰ��� �ϴ� ���� ��ü�̸�, �̸� �����ϴ°� ���� �����ϰ��� �ϴ°��� ���� �����ϴ°�ó�� �մϴ�.
	�����ڴ� �ּҸ� �޴°��� �ƴ϶�. �׿��� ��ü�� �����Ƿ�, DataArr[index] ���� �ʱ�ȭ �Ͽ� �����Ѵ�.
	@param index �����ϰ��� �ϴ� �ε���
	*/
	int& operator[] (int index);

	~CDArr();

};

