#pragma once
#include "pch.h"
#include <assert.h>

/*
	template Class �� ��� .h �� ���´�.
		data type �� �������� �ʾ�����, ������ Ÿ�ӿ� typename �� �������鼭 data type �� �������� ������.

	template class ���� �ۿ���,
		template class �� ����� typename �� �����ش�.
		pointer �� typename �� ���� �ʴ´�.
*/

/* BST Node �� ���� ��� �迭�� �ε����� ����� ������ */
enum NodeType {

	PARENT,	// 0, �θ��� �ε���
	LCHILD,	// 1, ���� �ڽĳ�� �ε���
	RCHILD,	// 2, ������ �ڽĳ�� �ε���
	END,	// 3, �������� ����, ����� ����, �� null index �� ���
};


/* data �κп� �ش��ϴ� pair ����ü*/
template <typename KeyType, typename ValueType>
struct Pair {
	KeyType first;		// data �� key �� �ش��ϴ� ��
	ValueType second;	// data �� value �� �ش��ϴ� ��

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

/* �Է��� key ���� value ���� �޾� , Pair �� ����� ��ȯ�մϴ�.*/
template<typename KeyType, typename ValueType>
typename Pair<KeyType, ValueType> make_BSTPair(KeyType key, ValueType value)
{
	return Pair<KeyType, ValueType>(key, value);
}


template <typename KeyType, typename ValueType>
struct BSTNode
{
	Pair<KeyType, ValueType> pair;			// data

	/*
	BSTNode* pParent;		// �θ���
	BSTNode* pLeftChild;	// �ڽĳ�� ����, �ڽź��� ������
	BSTNode* pRightChild;	// �ڽĳ�� ������, �ڽź��� ū��
	*/ // �Ʒ� �迭�� �ٲپ� ����

	BSTNode* arrNode[(int)NodeType::END];			 // �̷��� �ϸ� 3����� �����Ͱ� ����ȴ�. NodeType::END �� type�� �� ���� 3�� �ȴ�.


	BSTNode()
		: pair()
		, arrNode{}
	{

	}

	BSTNode(const Pair<KeyType, ValueType>& _pair, BSTNode* pParentNode, BSTNode* pLeftChildNode, BSTNode* pRightChildNode)
		: pair(_pair)
		, arrNode{ pParentNode, pLeftChildNode, pRightChildNode }
	{

	}

public:

	/* ���� �ڽ� ������� �����ϴ� �Լ�
	* ����ϱ� ���� �θ��尡 nullptr ���� Ȯ���� ����ϵ��� ����
	*/
	bool isLeftChild()
	{
		return this->arrNode[(int)NodeType::PARENT]->arrNode[(int)NodeType::LCHILD] == this ? true : false;
	}

	/* ���� �ڽ� ������� �����ϴ� �Լ�
	* ����ϱ� ���� �θ��尡 nullptr ���� Ȯ���� ����ϵ��� ����
	*/
	bool isRightChild()
	{
		return this->arrNode[(int)NodeType::PARENT]->arrNode[(int)NodeType::RCHILD] == this ? true : false;
	}

	/* ȣ���� ��尡 root node ���� Ȯ�� �ϴ� �Լ� */
	bool isRoot()
	{
		return this->arrNode[(int)NodeType::PARENT] == nullptr ? true : false;
	}

	/* �ڽ��� ���� ���(�ܸ����) ���� ��ȯ�ϴ� �Լ� */
	bool isLeaf() {
		if (nullptr == this->arrNode[(int)NodeType::LCHILD] && nullptr == this->arrNode[(int)NodeType::RCHILD]) {
			return true;
		}
		return false;
	}

	/* �ڽ��� ��� �ִ� ��� ���� ���θ� ��ȯ�ϴ� �Լ� */
	bool isFull() {
		if (nullptr != this->arrNode[(int)NodeType::LCHILD] && nullptr != this->arrNode[(int)NodeType::RCHILD]) {
			return true;
		}
		return false;
	}
};


/*BinarySearchTree
* BST �� ��Ʈ��常 �˸� �ȴ�.
*/
template <typename KeyType, typename ValueType>
class BST
{
private:

	BSTNode<KeyType, ValueType>* pRootNode;		// ��Ʈ��� �ּ�
	int				   		    iCount;			// ������ ����

public:

	BST()
		: pRootNode(nullptr)
		, iCount(0)
	{}


public:
	class iterator;

	/* ������ �Է� */
	bool insert(const Pair<KeyType, ValueType>& pair);

	/* ������ ���� */
	iterator erase(iterator deleteNodeIter);

	/* ���� ���� ��带 ��ȯ�մϴ�. */
	iterator begin();

	iterator end();

	/* key ���� �޾� iterator �� ��ȯ�ϴ� �Լ� */
	iterator find(const KeyType& key);

	/* ���� �ļ��� ��� ��ȯ */
	BSTNode<KeyType, ValueType>* GetInOrderSuccessor(BSTNode<KeyType, ValueType>* _pNode);

	/* ���� ������ ��� ��ȯ */
	BSTNode<KeyType, ValueType>* GetInOrderPredecessor(BSTNode<KeyType, ValueType>* _pNode);


private:
	/* ��� ����
	* �Է��� ���ͷ����Ͱ� ����Ű�� ��带 �����ϰ� ���� �����ļ��� ��带 ��ȯ�ϴ� �Լ�
	*/
	BSTNode<KeyType, ValueType>* deleteNode(BSTNode<KeyType, ValueType>* pDeleteNode);


public:

	/* iterator
	*  ��带 ����Ű�� ���ͷ�����
	*/
	class iterator {

		friend class BST; // BST �� iterator �� ���� ������ �� �ֵ���

	private:
		BST<KeyType, ValueType>* pBST;		// BST ��ü�� �˰� �ִ�.
		BSTNode<KeyType, ValueType>* pNode; // null �ΰ�� end iterator;

	public:
		iterator()
			: pBST(nullptr)
			, pNode(nullptr)
		{

		}

		iterator(BST<KeyType, ValueType>* _pBST, BSTNode<KeyType, ValueType>* _PNode)
			: pBST(_pBST)
			, pNode(_PNode)
		{

		}


		/* ���� ��ȯ
		*  ���⿡���� �ٸ��� ������ �� ���� �ؾ��Ѵ�.
		*  �ֳ��ϸ�, key ���� ���� �ڸ��� �����Ǿ��µ�, key ���� �ٲپ������, �� Ʋ������.
		*  ���� Ȯ���� �� �ֵ��� �Ѵ�.
		*/
		const Pair<KeyType, ValueType>& operator *() {
			assert(pNode); // ����Ű�� �ִ� �����Ͱ� nullptr �̶�� �ߴ�
			return pNode->pair;
		}

		/* ���� ��ȯ
		*  & ������ Pair ����ü�� ��ȯ�ߴٸ� -> ������ �ѹ� �� �������̰� ��ȯ�ϴ� �������.
		*  ������ �ڷ����� -> �� �� �� �����ϱ�. ����ڰ� ���ϴ� ���� ������ �� �������̴�.
		*/
		const Pair<KeyType, ValueType>* operator ->() {
			assert(pNode);
			return &(pNode->pair);
		}

		/* �� ������ */
		bool operator == (const iterator& _other)
		{
			if (pBST == _other.pBST && pNode == _other.pNode) {
				return true;
			}
			else {
				return false;
			}
		}

		/* �� ������ */
		bool operator != (const iterator& _other) {
			return !(*this == _other);
		}

		/* ���� ��带 ����Ű�� ���ͷ����͸� ��ȯ�ϴ� ������ */
		iterator& operator ++ () {
			pNode = pBST->GetInOrderSuccessor(pNode);
			return *this;
		}

	};

};

template<typename KeyType, typename ValueType>
inline bool BST<KeyType, ValueType>::insert(const Pair<KeyType, ValueType>& _pair)
{
	BSTNode<KeyType, ValueType>* pNewNode = new BSTNode<KeyType, ValueType>(_pair, nullptr, nullptr, nullptr);

	// ù��° �����Ͷ��
	if (nullptr == pRootNode)
	{
		pRootNode = pNewNode;
		++iCount;
		return true;
	}
	else {
		/* ��ġã�� �۾�
		*	- ù��° �����Ͱ� �ƴ϶��, ��Ʈ��尡 �ִٴ°�, �񱳸� �����Ѵ�.
		*	- ������ ��Ʈ��Ʈ ����
		*	- �� ��尡 �ܸ���尡 �� ������
		*	- pair ���� first ���� ���ؼ� ũ�ٸ� left, �۴ٸ� right
		*/

		BSTNode<KeyType, ValueType>* pNode = pRootNode; // ��Ʈ��带 ����Ű��
		NodeType nodeType = NodeType::END;		// �ƹ��͵� ����Ű�� �ʴ� Ÿ������ ����, �̰��� BSTNode �� �ε����� �־��ٰ��̾�. PARENT = 0, LCHILD = 1, RCHILD =2, END = 3

		// �ڸ� ã�Ƽ� ����
		while (true)
		{
			// �űԳ���� Ű���� ��Ʈ��庸�� �۴ٸ�, �����ڽ� ���� ������
			if (pNode->pair.first > pNewNode->pair.first) {
				nodeType = NodeType::LCHILD;
			}
			else if (pNode->pair.first < pNewNode->pair.first) {
				nodeType = NodeType::RCHILD;
			}
			else { // key ���� ���ٸ�, �ߺ� key ���� ������� �������̾�. insert ���� -> false �� ��ȯ
				return false;
			}

			// ������ �ϴ� pNode �� �ڽ��� ���������� �ݺ��Ұ��̾�. �ڽ��� nullptr �� �ƴ϶�� �ű⿡ �����ϸ� �ǰ���.
			if (nullptr == pNode->arrNode[(int)nodeType]) // ���� �ڽ��̵� ������ �ڽ��̵� 
			{
				pNode->arrNode[(int)nodeType] = pNewNode; // �ڽĳ�� ����
				pNewNode->arrNode[(int)NodeType::PARENT] = pNode;  // �θ� ��� ����
				++iCount;
				return true;
			}
			else {
				// ������ �ϴ� �ʿ� �ڽ��� �̹� �ִ°��
				pNode = pNode->arrNode[(int)nodeType];
			}

		}

	}

}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType, ValueType>::iterator BST<KeyType, ValueType>::erase(BST<KeyType, ValueType>::iterator deleteNodeIter)
{
	BST<KeyType, ValueType>::iterator successorIter(this, deleteNode(deleteNodeIter.pNode));
	return successorIter;
}

template<typename KeyType, typename ValueType>
inline BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::deleteNode(BSTNode<KeyType, ValueType>* pDeleteNode)
{
	BSTNode<KeyType, ValueType>* pSuccessorNode = GetInOrderSuccessor(pDeleteNode); // �켱 �ļ� ��带 ����Ű���� ����, ��ȯ�� ��尡 �ȴ�.

	// ������ ��尡 �ڽ��� ���°�� ( ������� �� ��� ) -> �θ𿡰Լ� ������ ����, ����,
	// ������ ��尡 �ڽ��� �ִ°�� : 1�� -> �����Ϸ��� ����� �θ�� �ڽ��� ������ ������ ���� ����
	// ������ ��尡 �ڽ��� �ִ°�� : 2�� -> �ش� ����� ���� �����ļ��� �� ä���, ���� �ļ����� �ڸ��� �����ļ����� �ڽĿ��θ� ���� ó��

	// case 1 : �ڽ��� ���°��
	if (pDeleteNode->isLeaf()) {

		if (pDeleteNode->isRoot()) { // �ڽĳ�尡 ���µ�, ��Ʈ��� �̱� ���� �ϴٸ�! �׳� nullptr ��ȯ
			pRootNode = nullptr;
		}
		else {	// �ڽĳ�尡 ���µ� ��Ʈ���� �ƴ϶��

			// ������Ű���� ����� �ε����� ã���ϴ�. [���� || ������] �ڽĿ� �ش��ϴ���.
			NodeType eDeleteNodeDirectionIdx = pDeleteNode->isLeftChild() ? NodeType::LCHILD : NodeType::RCHILD;
			pDeleteNode->arrNode[(int)NodeType::PARENT]->arrNode[(int)eDeleteNodeDirectionIdx] = nullptr;
		}

		--iCount;
		delete pDeleteNode;	// �ش� ��� ����.
	}
	// case 2 : �����Ϸ��� ����� �ڽ��� ���� ������ ��� �־�.
	else if (pDeleteNode->isFull())
	{
		// ������ ����� �ڸ��� ���� �ļ����� ���� ���� ��Ų��.
		pDeleteNode->pair = pSuccessorNode->pair;

		// ���� �ļ��ڸ� ������Ų��.
		deleteNode(pSuccessorNode);	// �����ļ��ڴ� �ڽ��� 1�� (������ �ڽ�) �� �ְų� ���� �� �ִ�. �̴� ����Լ��� ���� case 1 �Ǵ� 3 �� �ɸ����̴�.

		// ��ȯ�� ���� �� ������ ��尡 �ȴ�. ������ ����� ���� ���� �ļ����� ���� �����ϱ�.
		pSuccessorNode = pDeleteNode;
	}
	// case 3 : �����Ϸ��� ����� �ڽ��� �ϳ��� �ִ� ���
	else {
		// ���� ����� idx , �� �����Ϸ��� ����� �ڽ��� �ϳ� �ִµ�, �� �ڽ��� ���ʿ� �ִ��� �����ʿ� �ִ��� �ӽ� �����ϴ� enum
		NodeType eChildNodeIdx = (nullptr != pDeleteNode->arrNode[(int)NodeType::LCHILD] ? NodeType::LCHILD : NodeType::RCHILD);

		// �����Ϸ��� ��尡 ��Ʈ�� ���
		if (pDeleteNode->isRoot())
		{
			pRootNode = pDeleteNode->arrNode[(int)eChildNodeIdx];	// ��Ʈ���� �ڽ� ����
			pRootNode->arrNode[(int)NodeType::PARENT] = nullptr;	// ��Ʈ����� �θ�� ����
		}
		else {

			// ��������� idx, �� �����Ϸ��� ��尡 �����ڽ����� ������ �ڽ����� �ӽ� �����ϴ� enum
			NodeType eDeleteNodeDirectionIdx = pDeleteNode->isLeftChild() ? NodeType::LCHILD : NodeType::RCHILD;

			// �θ����� (�� || ��)�ڽ��� ����Ű�� ����� �ּҰ��� ( �����Ϸ��� ����� �ڽ�, �� ����) ���� ��������.
			pDeleteNode->arrNode[(int)NodeType::PARENT]->arrNode[(int)eDeleteNodeDirectionIdx] = pDeleteNode->arrNode[(int)eChildNodeIdx];

			// (�ڽĳ��)������ (�����Ϸ��� ����� �θ�) �� �����ؾ��Ѵ�.
			pDeleteNode->arrNode[(int)eChildNodeIdx]->arrNode[(int)NodeType::PARENT] = pDeleteNode->arrNode[(int)NodeType::PARENT];
			// ���ڿ� �θ��� ��� ���� �Ϸ�
		}
		delete pDeleteNode;
		--iCount;
	}

	return pSuccessorNode;
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType, ValueType>::iterator BST<KeyType, ValueType>::begin()
{
	BSTNode<KeyType, ValueType>* pNode = pRootNode; // ��Ʈ������ �����Ͽ�

	while (nullptr != pNode->arrNode[(int)NodeType::LCHILD])
	{
		pNode = pNode->arrNode[(int)NodeType::LCHILD]; // ���� �ڽ��� ����Ű���� �̵�
	}

	return iterator(this, pNode);
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType, ValueType>::iterator BST<KeyType, ValueType>::end()
{
	return iterator(this, nullptr);
}

template<typename KeyType, typename ValueType>
inline typename BST<KeyType, ValueType>::iterator BST<KeyType, ValueType>::find(const KeyType& key)
{
	// ã�� ������ insert �� ����ϴ�.
	// ũ�� �� : ũ�� ������ ������ �������� ���ٰ� ������ �װ��� key ���̾�.

	BSTNode<KeyType, ValueType>* pNode = pRootNode; // ��Ʈ������ �����Ͽ�
	NodeType nodeType = NodeType::END; // ���� �ʱ�ȭ

	// ��ġã�������� �ݺ�
	while (nullptr != pNode)
	{

		if (pNode->pair.first > key) {	// key ���� ���� ����� key ������ ������� ������ �����ڽ����� ����
			nodeType = NodeType::LCHILD;

		}
		else if (pNode->pair.first < key) {	// key ���� �������� key ������ ū��� ������ ������ �ڽ����� ����
			nodeType = NodeType::RCHILD;
		}
		else {	// key ���� �������� key ���� �������. ��ȯ
			return iterator(this, pNode);
		}

		pNode = pNode->arrNode[(int)nodeType];	// �̵��� �ݺ�
	}

	// �ݺ��ߴµ� nullptr ���� ��ã�Ҵٸ� 
	return end();
}

template<typename KeyType, typename ValueType>
inline BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::GetInOrderSuccessor(BSTNode<KeyType, ValueType>* _pNode)
{
	/*
	if(������ �ڽ��� ����){
		������ �ڽ����� �̵�;

		while (�����ڽ��� �ִٸ�)
		{
			�����ڽ����� �̵�
		}

		��ȯ, ���� �ļ��ڿ� �����ߴ�.
	}
	else if ( ���� �ڽ��� ��� ){
		�θ� ���� �ļ��� �̴�.
	}
	else if ( ������ �ڽ��� ��� ){
		�θ�� ��ĭ �̵�.

		while( ���� �ڽ��� �ƴ϶�� )
		{
			if( �θ� ���ٸ� ) { ���� ����忴��, ������ ū ���� ������. �� �θ��߿��� ���� �ڽĿ� �ش��ϴ� ��尡 ���ٴ°ž� }

			�θ�� ��ĭ �̵�
		}
		�θ��� �����ڽĿ� �ش��ϴ� ��� ��ȯ
	}
	else{
		// ������ �ڽĵ� ����, ���� �ڽĵ� �ƴϰ� ������ �ڽĵ� �ƴϾ�.
		// �ϳ��ۿ� ���� ��Ʈ ��� ��� �Ҹ�.
	}
	*/

	BSTNode<KeyType, ValueType>* pSuccessor = nullptr;

	if (nullptr != _pNode->arrNode[(int)NodeType::RCHILD])	// ������ �ڽ��� �ִ°��
	{
		pSuccessor = _pNode->arrNode[(int)NodeType::RCHILD];

		while (nullptr != pSuccessor->arrNode[(int)NodeType::LCHILD])
		{
			// ������ �ڽ��� ���� ���ʿ� �ִ� ���� �����ϸ� �ݺ��� Ż���� ��ȯ
			pSuccessor = pSuccessor->arrNode[(int)NodeType::LCHILD];
		}
	}
	else {

		pSuccessor = _pNode;

		//������ �ڽ� ���� �θ��� �״�� ��ȯ. �� ������ �����ϱ�.
		if (pSuccessor->isRoot()) {
			return nullptr;
		}

		// ���� ���� �ڽ��̶��, �� �θ� ��������
		if (pSuccessor->isLeftChild()) {
			pSuccessor = _pNode->arrNode[NodeType::PARENT];
		}
		else { // ���� ������ �ڽ� �̶��.

			pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT]; // �θ���� �����̾�

			// �θ��߿��� �ݺ�...
			while (pSuccessor)
			{
				if (pSuccessor->isRoot()) {
					// �θ� ����. �� ��������� �ö�Ծ�.
					/* ���� ���� �� �Լ� ���Խ� ��庸�� ū ���� �ִٸ�
					*  �θ��߿��� ���� �ڽ��� ��尡 �����ٵ�, ��Ʈ������ �ö�������� ������ ���ߴٴ°�, �ڽź��� ū ���� ���ٴ� ���̴�.
					*/
					pSuccessor = nullptr;
					break;
				}

				if (pSuccessor->isLeftChild()) {
					// �θ��߿� �����ڽĿ� �ش��ϴ� ��尡 �ִٸ�, �� ����� �θ� ���� ����. ���̴� �����̴�.
					pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT];
					break;
				}
				else {
					// �θ��尡 ���� �ڽ��� �ƴҶ� ����  �ö󰥲���. �� ���̴�����. �ڽź��� ū���� �ִ°�츦 ã�°Ͱ� ���������� �ǹ̴�.
					pSuccessor = pSuccessor->arrNode[(int)NodeType::PARENT]; // -> �ݺ� ( �θ�� �ö� )
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
