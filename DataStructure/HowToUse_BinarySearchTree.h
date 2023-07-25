#pragma once
#include "pch.h"
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

	cout << endl << "--- BST 중위순회 출력 테스트 ---" << endl;
	// key 값에 따라 순자차적으로 반환하는 방법
	BST<int, int> map2;

	map2.insert(make_BSTPair(100, 100));
	map2.insert(make_BSTPair(150, 150));
	map2.insert(make_BSTPair(50, 50));
	map2.insert(make_BSTPair(25, 25));
	map2.insert(make_BSTPair(75, 75));
	map2.insert(make_BSTPair(125, 125));
	map2.insert(make_BSTPair(175, 175));


	BST<int, int>::iterator map2Iter = map2.begin(); // 시작점을 가리키도록

	for (map2Iter; map2Iter != map2.end(); ++map2Iter) {

		cout << "Key : " << map2Iter->first << ", Value : " << map2Iter->second << endl;
	}
	cout << "---------------------------------" << endl << endl;

	int deleteKeyValue = 150;
	BST<int, int>::iterator map2Iter2 = map2.find(deleteKeyValue);	// deleteKeyValue 에 입력한 key 값을 가지고 있는 노드를 iterator 로 가리켜
	map2Iter2 = map2.erase(map2Iter2);	// iterator 가 가리키고 있는 노드를 삭제합니다. 삭제후에는 삭제한 노드의 중위후속자를 가리키는 iterator 를 반환합니다.
	cout << "key ( " << deleteKeyValue << " ) 을 가지는 노드를 삭제한, 다음 노드는 (" << "Key : " << map2Iter2->first << ", Value : " << map2Iter2->second << ") 입니다. " << endl;

	return;
};