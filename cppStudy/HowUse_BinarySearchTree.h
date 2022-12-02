#pragma once
#include <iostream>
#include <string>
#include "BinarySearchTree.h"

/* BinarySearchTree 사용방법 */

using std::string;
using std::cout;
using std::endl;

/*Binary Search Tree 사용방법*/
void useBST() {
	
	/* BST 선언*/
	BST<string, int> map;

	/* BST 에 값 넣기
	* make_BSTPair 를 사용하여, 페어 구조체 (key,value) 를 만들어 입력합니다.
	*/
	map.insert(make_BSTPair(string("student1"), 50));

	map.insert(make_BSTPair(string("student2"), 60));

	map.insert(make_BSTPair(string("student3"), 80));

	// 값을 받을 이터레이터 선언후, 찾은값을 받기
	BST<string, int>::iterator findIter = map.find(string("student2"));

	cout << findIter->second << "<- 찾은값" << endl;

	cout << "순차 출력 테스트 ------------------------------" << endl;
	// key 값에 따라 순자차적으로 반환하는 방법
	BST<int, int> map2;

	map2.insert(make_BSTPair(10, 10));
	map2.insert(make_BSTPair(50, 50));
	map2.insert(make_BSTPair(30, 30));
	map2.insert(make_BSTPair(80, 80));
	map2.insert(make_BSTPair(20, 20));

	BST<int, int>::iterator map2Iter = map2.begin(); // 시작점을 가리키도록

	for (map2Iter; map2Iter != map2.end(); ++map2Iter) {

		cout << "Key : " << map2Iter->first << ", Value : " << map2Iter->second << endl;
	}

	return;
};