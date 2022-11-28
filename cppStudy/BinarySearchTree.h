#pragma once

/* BST Node 의 내부 노드 배열의 인덱스로 사용할 열거형 */
enum NodeType {

	PARENT,	// 0, 부모노드 인덱스
	LCHILD,	// 1, 왼쪽 자식노드 인덱스
	RCHILD,	// 2, 오른쪽 자식노드 인덱스
	END,	// 3, 열거형의 개수, 노드의 개수, 및 null index 로 사용
};


/* data 부분에 해당하는 pair 구조체*/
template <typename KeyType, typename ValueType>
struct Pair {
	KeyType first;		// data 의 key 에 해당하는 값
	ValueType second;	// data 의 value 에 해당하는 값

	Pair()
		: first()
		, second()
	{

	}

	Pair(KeyType key, ValueType value)
		: first(key)
		, second(value)
	{

	}
};

/* 입력한 key 값과 value 값을 받아 , Pair 를 만들어 반환합니다.*/
template<typename KeyType, typename ValueType>
typename Pair<KeyType, ValueType> make_BSTPair(KeyType key, ValueType value)
{
	return Pair<KeyType, ValueType>(key, value);
}


template <typename KeyType, typename ValueType>
struct BSTNode 
{
	Pair<KeyType,ValueType> pair;			// data
	
	/*
	BSTNode* pParent;		// 부모노드
	BSTNode* pLeftChild;	// 자식노드 왼쪽, 자신보다 작은값
	BSTNode* pRightChild;	// 자식노드 오른쪽, 자신보다 큰값
	*/ // 아래 배열로 바꾸어 선언

	BSTNode* arrNode[(int)NodeType::END];			 // 이렇게 하면 3개노드 포인터가 선언된다. NodeType::END 는 type의 총 개수 3이 된다.
	

	BSTNode()
		: pair()
		, arrNode{}
	{

	}

	BSTNode(const Pair<KeyType,ValueType>& _pair, BSTNode* pParentNode, BSTNode* pLeftChildNode, BSTNode* pRightChildNode)
		: pair(_pair)
		, arrNode{ pParentNode, pLeftChildNode, pRightChildNode }
	{

	}
};


/*BinarySearchTree
* BST 는 루트노드만 알면 된다.
*/
template <typename KeyType, typename ValueType>
class BST
{
private:

	BSTNode<KeyType,ValueType>* pRootNode;		// 루트노드 주소
	int				   		    iCount;			// 데이터 개수

public:

	BST()
		: pRootNode(nullptr)
		, iCount(0)
	{}


public:

	/* 데이터 입력 */
	bool insert(const Pair<KeyType, ValueType>& pair);

};

template<typename KeyType, typename ValueType>
inline bool BST<KeyType, ValueType>::insert(const Pair<KeyType,ValueType>& _pair)
{
	BSTNode<KeyType, ValueType>* pNewNode = new BSTNode<KeyType, ValueType>(_pair, nullptr, nullptr, nullptr);
	
	// 첫번째 데이터라면
	if (nullptr == pRootNode)
	{
		pRootNode = pNewNode;
		++iCount;
	}
	else {
		/* 위치찾는 작업
		*	- 첫번째 데이터가 아니라면, 루트노드가 있다는것, 비교를 시작한다. 
		*	- 시작은 루트노트 부터
		*	- 새 노드가 단말노드가 될 때까지
		*	- pair 내의 first 값과 비교해서 크다면 left, 작다면 right
		*/
		
		BSTNode<KeyType, ValueType>* pNode = pRootNode; // 루트노드를 가리키고
		enum NodeType  nodeType = NodeType::END;		// 아무것도 가리키지 않는 타입으로 선언, 이것을 BSTNode 의 인덱스로 넣어줄것이야. PARENT = 0, LCHILD = 1, RCHILD =2, END = 3

		// 자리 찾아서 연결
		while (true)
		{
			// 신규노드의 키값이 루트노드보다 작다면, 왼쪽자식 으로 가야지
			if (pNode->pair.first > pNewNode->pair.first){
				nodeType = NodeType::LCHILD;
			}
			else if (pNode->pair.first < pNewNode->pair.first){
				nodeType = NodeType::RCHILD;
			}
			else { // key 값이 같다면, 중복 key 값은 허용하지 않을것이야. insert 실패 -> false 로 반환
				return false;
			}

			// 가고자 하는 pNode 의 자식이 없을때까지 반복할것이야. 자식이 nullptr 이 아니라면 거기에 삽입하면 되겠지.
			if (nullptr == pNode->arrNode[(int)nodeType]) // 왼쪽 자식이든 오른쪽 자식이든 
			{
				pNode->arrNode[(int)nodeType] = pNewNode; // 자식노드 설정
				pNewNode->arrNode[(int)NodeType::PARENT] = pNode;  // 부모 노드 설정
				++iCount;
				break;
			}
			else {
				// 가고자 하는 쪽에 자식이 이미 있는경우
				pNode = pNode->arrNode[(int)nodeType];
			}

		}
		
		

	}
	
}
