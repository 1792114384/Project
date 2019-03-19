#pragma once
#include <queue>
#include <vector>
using namespace std;

template <typename T>
struct HuffmanTreeNode
{
	typedef HuffmanTreeNode<T> Node;
	HuffmanTreeNode(const T& weight)
		:_weight(weight)
		,pLeft(nullptr)
		,pRight(nullptr)
		,pParent(nullptr)
	{}
	T _weight;
	Node* pLeft;
	Node* pRight;
	Node* pParent;
};

template <typename T>
struct great
{
	bool operator()(const T &left, const T &right)
	{
		return left->_weight > right->_weight;
	}
};

template <typename T>
class HuffmanTree
{
public:
	typedef HuffmanTreeNode<T> Node;

	HuffmanTree()
		:root(NULL)
	{}
	~HuffmanTree()
	{
		destory(root);
	}
	Node* GetRoot()
	{
		return root;
	}
	template <class T>
	HuffmanTree(T* info, int invalid)
	{
		priority_queue<Node*, vector<Node*>, great<Node*>> hTree;
		for (int i = 0; i < 256; ++i)
		{
			if (info[i]._chCount != invalid)
			{
				Node* tmp = new Node(info[i]);
				hTree.push(tmp);  
			}
		}
		Node *Left;
		Node *Right;
		while (hTree.size() > 1)
		{
			Left = hTree.top();
			hTree.pop();

			Right = hTree.top();
			hTree.pop();

			Node* Parent = new Node(Left->_weight + Right->_weight);
			Parent->pLeft = Left;
			Left->pParent = Parent;
			Parent->pRight = Right;
			Right->pParent = Parent;
			hTree.push(Parent);
		}
		root = hTree.top();
	}

	void destory(Node *root)
	{
		if (root == nullptr)
			return;
		destory(root->pLeft);
		destory(root->pRight);

		delete root;
	}
private:
	Node *root;
};


