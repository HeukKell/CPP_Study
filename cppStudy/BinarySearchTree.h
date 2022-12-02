#pragma once
#include <assert.h>

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

public:

	/* 왼쪽 자식 노드인지 검증하는 함수
	* 사용하기 전에 부모노드가 nullptr 인지 확인후 사용하도록 하자
	*/
	bool isLeftChild()
	{
		return this->arrNode[(int)NodeType::PARENT]->arrNode[(int)NodeType::LCHILD] == this ? true : false;
	}

	/* 왼쪽 자식 노드인지 검증하는 함수
	* 사용하기 전에 부모노드가 nullptr 인지 확인후 사용하도록 하자
	*/
	bool isRightChild()
	{
		return this->arrNode[(int)NodeType::PARENT]->arrNode[(int)NodeType::RCHILD] == this ? true : false;
	}

	/* 호출한 노드가 root node 인지 확인 하는 함수 */
	bool isRoot()
	{
		return this->arrNode[(int)NodeType::PARENT] == nullptr ? true : false;
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

	/* 중위 후속자 노드 반환 */
	BSTNode<KeyType, ValueType>* GetInOrderSuccessor(BSTNode<KeyType, ValueType>* _pNode);
	
	/* 중위 선행자 노드 반환 */
	BSTNode<KeyType, ValueType>* GetInOrderPredecessor(BSTNode<KeyType, ValueType>* _pNode);


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


		/* 값을 반환 
		*  여기에서는 다르게 수정할 수 없게 해야한다.
		*  왜냐하면, key 값에 의해 자리가 결정되었는데, key 값을 바꾸어버리면, 다 틀어진다.
		*  값만 확인할 수 있도록 한다.
		*/
		const Pair<KeyType,ValueType>& operator *(){
			assert(pNode); // 가리키고 있는 포인터가 nullptr 이라면 중단
			return pNode->pair;
		}

		/* 값을 반환
		*  & 연산은 Pair 그자체를 반환했다면 -> 연산은 한번 덜 직접적이게 반환하는 방법으로. 
		*  포인터 자료형은 -> 을 쓸 수 있으니까. 사용자가 원하는 값을 가져갈 수 있을것이다.
		*/
		const Pair<KeyType, ValueType>* operator ->(){
			assert(pNode);
			return &(pNode->pair);
		}

		/* 비교 연산자 */
		bool operator == (const iterator& _other)
		{
			if (pBST == _other.pBST && pNode == _other.pNode){
				return true;
			}
			else {
				return false;
			}
		}
		
		/* 비교 연산자 */
		bool operator != (const iterator& _other) {
			return !(*this == _other);
		}

		/* 다음 노드를 가리키는 이터레이터를 반환하는 연산자 */
		iterator& operator ++ () {
			pNode = pBST->GetInOrderSuccessor(pNode);
			return *this;
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
		NodeType nodeType = NodeType::END;		// 아무것도 가리키지 않는 타입으로 선언, 이것을 BSTNode 의 인덱스로 넣어줄것이야. PARENT = 0, LCHILD = 1, RCHILD =2, END = 3

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

template<typename KeyType, typename ValueType>
inline BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::GetInOrderSuccessor(BSTNode<KeyType, ValueType>* _pNode)
{
	/*
	if(오른쪽 자식을 보유){
		오른쪽 자식으로 이동;
		
		while (왼쪽자식이 있다면)
		{
			왼쪽자식으로 이동
		}

		반환, 중위 후속자에 도달했다.
	}
	else if ( 왼쪽 자식인 경우 ){
		부모가 중위 후속자 이다.
	}
	else if ( 오른쪽 자식인 경우 ){
		부모로 한칸 이동.
	
		while( 왼쪽 자식이 아니라면 )
		{
			if( 부모가 없다면 ) { 내가 끝노드였어, 나보다 큰 노드는 없었어. 즉 부모중에는 왼쪽 자식에 해당하는 노드가 없다는거야 }
			
			부모로 한칸 이동
		}
		부모중 왼쪽자식에 해당하는 노드 반환
	}
	else{
		// 오른쪽 자식도 없고, 왼쪽 자식도 아니고 오른쪽 자식도 아니야.
		// 하나밖에 없는 루트 노드 라는 소리.
	} 
	*/

	BSTNode<KeyType, ValueType>* pSuccessor = nullptr; 

	if (nullptr != _pNode->arrNode[(int)NodeType::RCHILD])	// 오른쪽 자식이 있는경우
	{
		pSuccessor = _pNode->arrNode[(int)NodeType::RCHILD];

		while ( nullptr != pSuccessor->arrNode[(int)NodeType::LCHILD] )
		{
			// 오른쪽 자식중 가능 왼쪽에 있는 값에 도달하면 반복문 탈출후 반환
			pSuccessor = pSuccessor->arrNode[(int)NodeType::LCHILD];
		}
	}
	else {
		
		pSuccessor = _pNode;

		//오른쪽 자식 없는 부모라면 그대로 반환. 내 다음은 없으니까.
		if (pSuccessor->isRoot()) { 
			return nullptr;
		}

		// 내가 왼쪽 자식이라면, 내 부모가 다음노드야
		if (pSuccessor->isLeftChild()) {
			pSuccessor = _pNode->arrNode[NodeType::PARENT];
		}
		else { // 내가 오른쪽 자식 이라면.
			
			pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT]; // 부모부터 시작이야
			
			// 부모중에서 반복...
			while ( pSuccessor )
			{
				if (pSuccessor->isRoot() ) {
					// 부모가 없다. 즉 헤더노드까지 올라왔어.
					/* 만약 현재 이 함수 진입시 노드보다 큰 수가 있다면
					*  부모중에는 왼쪽 자식인 노드가 있을텐데, 루트노드까지 올라왔음에도 만나지 못했다는건, 자신보다 큰 수가 없다는 것이다.
					*/
					pSuccessor = nullptr;
					break;
				}

				if (pSuccessor->isLeftChild()) {					
					// 부모중에 왼쪽자식에 해당하는 노드가 있다면, 그 노드의 부모가 다음 노드야. 꺽이는 지점이다.
					pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT];
					break;
				}
				else {
					// 부모노드가 왼쪽 자식이 아닐때 까지  올라갈꺼야. 즉 꺽이는지점. 자신보다 큰수가 있는경우를 찾는것과 마찬가지의 의미다.
					pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT]; // -> 반복 ( 부모로 올라가 )
				}

			}

		}
	}
	return pSuccessor;
}

template<typename KeyType, typename ValueType>
inline BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::GetInOrderPredecessor(BSTNode<KeyType, ValueType>* _pNode)
{
	return nullptr;
}
