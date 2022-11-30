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

	return;
};