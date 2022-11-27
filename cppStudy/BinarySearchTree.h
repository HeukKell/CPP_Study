#pragma once


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
};


template <typename KeyType, typename ValueType>
struct BSTNode 
{
	Pair<KeyType,ValueType> pair;			// data
	BSTNode* pParent;	// 부모노드
	BSTNode* pLeftChild;	// 자식노드 왼쪽, 자신보다 작은값
	BSTNode* pRightChild;	// 자식노드 오른쪽, 자신보다 큰값

	BSTNode()
		: pair()
		, pParent(nullptr)
		, pLeftChild(nullptr)
		, pRightChild(nullptr)
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
	BSTNode<KeyType, ValueType>* pNewNode = new BSTNode<KeyType, ValueType>();
	pNewNode->pair = _pair;	// 새로만든 노드에 데이터 부분은 복사를 통해 채워졌다.

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
		
		// 자리 찾아서 연결
		while (true)
		{
			if (pNode->pair.first > pNewNode->pair.first)
			{
				if (nullptr == pNode->pLeftChild)
				{
					pNode->pLeftChild = pNewNode;
					pNewNode->pParent = pNode;
					++iCount;
					break;
				}
				else {
					pNode = pNode->pLeftChild;
				}
			}
			else if (pNode->pair.first < pNewNode->pair.first)
			{
				if (nullptr == pNode->pRightChild)
				{
					pNode->pRightChild = pNewNode;
					pNewNode->pParent = pNode;
					++iCount;
					break;
				}
				else {
					pNode = pNode->pRightChild;
				}
			}
			else {
				/* 값이 같을경우..
				* 중복되는 key 값을 허용하는 map 이 따로 존재합니다. multi map
				* map 의 경우 중복되는 key 값이 있으면 안넣기 때문에 미리 체크하는것이 중요하다.
				*/ 
				return false;
			}
		}
		
		

	}
	
}
