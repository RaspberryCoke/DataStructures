#include<iostream>
#include"arrayQueue.h"
#include"arrayStack.h"
using namespace std;

template<class T>
struct binaryTreeNode {
	T element;
	binaryTreeNode<T>* left;
	binaryTreeNode<T>* right;

	binaryTreeNode(const T& ele, binaryTreeNode<T>* l = nullptr, binaryTreeNode<T>* r = nullptr) {
		element=ele;
		left=l;
		right=r;
	}
};

template<class T>
class binaryTree {
public:
	virtual ~binaryTree() {}
	virtual bool empty()const = 0;
	virtual int size() const = 0;
	virtual void preOrder(void (*)(binaryTreeNode<T>*)) = 0;
	virtual void inOrder(void (*)(binaryTreeNode<T>*)) = 0;
	virtual void postOrder(void (*)(binaryTreeNode<T>*)) = 0;
	virtual void levelOrder(void(*)(binaryTreeNode<T>*)) = 0;
};

template<class T>
class linkedBinaryTree :public binaryTree<T> {
	
protected:
	binaryTreeNode<T>* root;
	int treeSize;
	
	//函数指针的声明
	static void (*visit)(binaryTreeNode<T>*);
	//delete函数
	static void dispose(binaryTreeNode<T>* t) { delete t; }
	//高度
	static int height(binaryTreeNode<T>* t){
		//中序遍历
		if (t == nullptr)
			return 0;
		int h1 = height(t->left);
		int h2 = height(t->right);
		if (h1 > h2) {
			return ++h1;
		}
		else {
			return ++h2;
		}
	}
public:
	
	linkedBinaryTree() { root = nullptr; treeSize = 0; }

	~linkedBinaryTree() { erase(); }

	void makeTree(const T& theElement, linkedBinaryTree<T>& theLeft, linkedBinaryTree<T>& theRight) {
		root = new binaryTreeNode<T>(theElement, theLeft.root, theRight.root);
		treeSize = theLeft.treeSize + theRight.treeSize+1;

		theLeft.root = theRight.root = nullptr;
		theLeft.treeSize = theRight.treeSize = 0;
	}

	bool empty()const { return treeSize == 0; }

	int size()const { return treeSize; }

	void inOrder(void (*theVisit)(binaryTreeNode<T>*)) {
		if (root == nullptr) {
			return;
		}

		linkedBinaryTree<T>::visit = theVisit;
		arrayStack<binaryTreeNode<T>*> stack(treeSize);
		auto current = root;

		while (!stack.empty()||current!=nullptr) {
			if (current != nullptr) {
				stack.push(current);
				current = current->left;
			}
			else {
				current = stack.top();
				stack.pop();
				visit(current);
				current = current->right;
			}
		}
	}

	void preOrder(void (*theVisit)(binaryTreeNode<T>*)) {
		if (root == nullptr) {
			return;
		}

		linkedBinaryTree<T>::visit = theVisit;
		arrayStack<binaryTreeNode<T>*>stack(treeSize);
		auto current = root;

		stack.push(current);

		while (!stack.empty()) {
			current = stack.top();
			stack.pop();
			visit(current);

			if (current->right != nullptr) {
				stack.push(current->right);
			}
			if (current->left != nullptr) {
				stack.push(current->left);
			}
		}
	}

	void postOrder(void (*theVisit)(binaryTreeNode<T>*)) {
		
		if (root == nullptr)
			return;
		linkedBinaryTree<T>::visit = theVisit;
		arrayStack<binaryTreeNode<T>*> stack1(treeSize);
		arrayStack<binaryTreeNode<T>*> stack2(treeSize);
		
		binaryTreeNode<T>* current = root;
		stack1.push(current);

		while (!stack1.empty()) {
			current = stack1.top();
			stack1.pop();
			stack2.push(current);
			if (current->left != nullptr) {
				stack1.push(current->left);
			}
			if (current->right != nullptr) {
				stack1.push(current->right);
			}
		}
		while (!stack2.empty())
		{
			current = stack2.top();
			visit(current);
			stack2.pop();
		}
	}

	void levelOrder(void (*theVisit)(binaryTreeNode<T>*)) {
		arrayQueue<binaryTreeNode<T>*> q;
		linkedBinaryTree<T>::visit = theVisit;
		binaryTreeNode<T>* p = root;
		q.push(p);
		while (!q.empty()) {
			p = q.front();
			q.pop();
			linkedBinaryTree<T>::visit(p);
			if (p->left != nullptr) {
				q.push(p->left);
			}
			if (p->right != nullptr) {
				q.push(p->right);
			}
		}
	}

	void erase() {
		postOrder(&dispose);
		root = nullptr;
		treeSize = 0;
	}
};

template<class T>
void (*linkedBinaryTree<T>::visit)(binaryTreeNode<T>*);
//
//int main() {
//	linkedBinaryTree<int> a, x, y, z;
//	y.makeTree(1, a, a);
//	z.makeTree(2, a, a);
//	x.makeTree(3, y, z);
//	y.makeTree(4, x, a);
//	y.inOrder([](binaryTreeNode<int>* t) {
//		if(t!=nullptr)
//		cout << t->element << " ";
//		});
//}