#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include "BinarySearchTree.h"

/* BinarySearchTree ����� */

using std::string;
using std::cout;
using std::endl;

/*Binary Search Tree �����*/
void useBST() {

	/* BST ����*/
	BST<string, int> map;

	/* BST �� �� �ֱ�
	* make_BSTPair �� ����Ͽ�, ��� ����ü (key,value) �� ����� �Է��մϴ�.
	*/
	map.insert(make_BSTPair(string("student1"), 50));

	map.insert(make_BSTPair(string("student2"), 60));

	map.insert(make_BSTPair(string("student3"), 80));

	// ���� ���� ���ͷ����� ������, ã������ �ޱ�
	BST<string, int>::iterator findIter = map.find(string("student2"));

	cout << findIter->second << "<- ã����" << endl;

	cout << endl << "--- BST ������ȸ ��� �׽�Ʈ ---" << endl;
	// key ���� ���� ������������ ��ȯ�ϴ� ���
	BST<int, int> map2;

	map2.insert(make_BSTPair(100, 100));
	map2.insert(make_BSTPair(150, 150));
	map2.insert(make_BSTPair(50, 50));
	map2.insert(make_BSTPair(25, 25));
	map2.insert(make_BSTPair(75, 75));
	map2.insert(make_BSTPair(125, 125));
	map2.insert(make_BSTPair(175, 175));


	BST<int, int>::iterator map2Iter = map2.begin(); // �������� ����Ű����

	for (map2Iter; map2Iter != map2.end(); ++map2Iter) {

		cout << "Key : " << map2Iter->first << ", Value : " << map2Iter->second << endl;
	}
	cout << "---------------------------------" << endl << endl;

	int deleteKeyValue = 150;
	BST<int, int>::iterator map2Iter2 = map2.find(deleteKeyValue);	// deleteKeyValue �� �Է��� key ���� ������ �ִ� ��带 iterator �� ������
	map2Iter2 = map2.erase(map2Iter2);	// iterator �� ����Ű�� �ִ� ��带 �����մϴ�. �����Ŀ��� ������ ����� �����ļ��ڸ� ����Ű�� iterator �� ��ȯ�մϴ�.
	cout << "key ( " << deleteKeyValue << " ) �� ������ ��带 ������, ���� ���� (" << "Key : " << map2Iter2->first << ", Value : " << map2Iter2->second << ") �Դϴ�. " << endl;

	return;
};