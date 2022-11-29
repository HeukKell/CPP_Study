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

	class iterator;

	/* 가장 왼쪽 노드를 반환합니다. */
	iterator begin();

	iterator end();

	/* key 값을 받아 iterator 를 반환하는 함수 */
	iterator find(const KeyType& key);


public:

	/* iterator
	*  노드를 가리키는 이터레이터
	*/
	class iterator {

	private:
		BST<KeyType, ValueType>* pBST;		// BST 본체도 알고 있다.
		BSTNode<KeyType, ValueType>* pNode; // null 인경우 end iterator;

	public:
		iterator()
			: pBST(nullptr)
			, pNode(nullptr)
		{

		}

		iterator( BST<KeyType, ValueType>* _pBST, BSTNode<KeyType, ValueType>* _PNode)
			: pBST(_pBST)
			, pNode(_PNode)
		{
			
		}


		/* 값을 반환 */
		Pair<KeyType,ValueType>& operator *()
		{
			return pNode->pair;
		}
		
	};

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
		return true;
	}
	else {
		/* 위치찾는 작업
		*	- 첫번째 데이터가 아니라면, 루트노드가 있다는것, 비교를 시작한다. 
		*	- 시작은 루트노트 부터
		*	- 새 노드가 단말노드가 될 때까지
		*	- pair 내의 first 값과 비교해서 크다면 left, 작다면 right
		*/
		
		BSTNode<KeyType, ValueType>* pNode = pRootNode; // 루트노드를 가리키고
		NodeType  nodeType = NodeType::END;		// 아무것도 가리키지 않는 타입으로 선언, 이것을 BSTNode 의 인덱스로 넣어줄것이야. PARENT = 0, LCHILD = 1, RCHILD =2, END = 3

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
				return true;
			}
			else {
				// 가고자 하는 쪽에 자식이 이미 있는경우
				pNode = pNode->arrNode[(int)nodeType];
			}

		}

	}
	
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType,ValueType>::iterator BST<KeyType, ValueType>::begin()
{
	BSTNode<KeyType, ValueType>* pNode = pRootNode; // 루트노드부터 시작하여

	while (nullptr != pNode->arrNode[(int)NodeType::LCHILD])
	{
		pNode = pNode->arrNode[(int)NodeType::LCHILD]; // 왼쪽 자식을 가리키도록 이동
	}

	return iterator(this,pNode);
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType, ValueType>::iterator BST<KeyType, ValueType>::end()
{
	return iterator(this,nullptr);
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType,ValueType>::iterator BST<KeyType, ValueType>::find(const KeyType& key)
{
	// 찾는 과정이 insert 와 비슷하다.
	// 크기 비교 : 크면 오른쪽 작으면 왼쪽으로 가다가 같으면 그것이 key 값이야.

	BSTNode<KeyType, ValueType>* pNode = pRootNode; // 루트노드부터 시작하여
	NodeType nodeType = NodeType::END; // 방향 초기화

	// 위치찾을때까지 반복
	while (nullptr != pNode)
	{
		
		if (pNode->pair.first > key) {	// key 값이 현재 노드의 key 값보다 작은경우 방향을 왼쪽자식으로 설정
			nodeType = NodeType::LCHILD;

		}
		else if (pNode->pair.first < key){	// key 값이 현재노드의 key 값보다 큰경우 방향을 오른쪽 자식으로 설정
			nodeType = NodeType::RCHILD;
		}
		else {	// key 값과 현재노드의 key 값이 같은경우. 반환
			return iterator(this, pNode);
		}

		pNode = pNode->arrNode[(int)nodeType];	// 이동후 반복
	}

	// 반복했는데 nullptr 까지 못찾았다면 
	return end();
}
