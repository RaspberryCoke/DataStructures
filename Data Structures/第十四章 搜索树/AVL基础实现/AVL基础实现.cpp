#include<iostream>
#include<stack>
#include<queue>
using namespace std;
/*
取消visit函数值指针，统一使用lambda表达式

使用stl

*/
template<class K, class E>
struct AVLNode {
	pair<K, E> element;
	AVLNode<K, E>* left;
	AVLNode<K, E>* right;
	int height;

	AVLNode(const pair<K, E>& ele = make_pair(0, 0), AVLNode<K, E>* l = nullptr, AVLNode<K, E>* r = nullptr) {
		element = ele;
		left = l;
		right = r;
		updateHeight(this);
	}
	int updateHeight(AVLNode<K, E>* t) {
		if (t == nullptr)
			return 0;
		else {//同时更新子节点的高度信息。
			t->height = max(updateHeight(t->left), updateHeight(t->right)) + 1;
			return t->height;
		}
	}
};

template<class K, class E>
class AVLTree {
private:
	AVLNode<K, E>* root;
	int treeSize;

	//为TREE提供一个计算高度的方法
	int height(AVLNode<K, E>* t) {
		if (t == nullptr)
			return 0;
		else {
			t->height = max(height(t->left), height(t->right)) + 1;
			return t->height;
		}
	}
	//插入
	AVLNode<K, E>* insert(AVLNode<K, E>* node, const pair<K, E>& theElement) {
		if (node == nullptr) {//函数出口，实际插入点
			node = new AVLNode<K, E>(theElement);
			treeSize++;
			return node;
		}

		if (theElement.first < node->element.first) {//插入到左子树
			node->left = insert(node->left, theElement);

			int t = height(node->left) - height(node->right);//检查node节点的平衡状态
			if (t == 2) {
				if (theElement.first > node->left->element.first) {//左右
					node = leftRightRotation(node);
				}
				else {//左左
					node = rightRotation(node);
				}
			}
		}
		else if (theElement.first > node->element.first) {//插入到右子树
			node->right = insert(node->right, theElement);
			int t = height(node->right) - height(node->left);//检查node节点的平衡状态
			if (t == 2) {
				if (theElement.first > node->right->element.first) {//右右
					node = leftRotation(node);
				}
				else {//右左
					node = rightLeftRotation(node);
				}
			}
		}
		else {//相等的情况
			node->element.second = theElement.second;//覆盖
			return node;
		}
		node->height = max(height(node->left), height(node->right)) + 1;
		return node;
	}
	//删除
	AVLNode<K, E>* erase(AVLNode<K, E>* node, const K& theKey) {
		if (node == nullptr)//没有找到该节点
			return nullptr;
		if (node->element.first == theKey) {//找到
			if (node->left && node->right) {//两个分支
				if (node->left->height >= node->right->height) {
					AVLNode<K, E>* tmp = node->left;
					while (tmp->right) {//找到左子树中的最大元素
						tmp = tmp->right;
					}
					node->element = tmp->element;//覆盖，向下递归删除
					node->left = erase(node->left, tmp->element.first);//递归的删除，因为要考虑平衡问题
					return node;
				}
				else {//找到左子树中的最大元素
					AVLNode<K, E>* tmp = node->right;
					while (tmp->left) {
						tmp = tmp->left;
					}
					node->element = tmp->element;//覆盖，向下递归删除
					node->right = erase(node->right, tmp->element.first);
					return node;
				}
			}
			//函数的真正的出口
			else {//没有两个分支时
				if (node->left) {//左子树嫁接
					AVLNode<K, E>* tmp2 = node;
					node = node->left;
					delete tmp2;
					treeSize--;
					return node;
				}
				else if (node->right) {//右子树嫁接
					AVLNode<K, E>* tmp3 = node;
					node = node->right;
					delete tmp3;
					treeSize--;
					return node;
				}
				else {//删除自己
					if (node == root) {
						root = nullptr;
						delete node;
						treeSize--;
						return root;
					}
					delete node;
					treeSize--;
					return nullptr;
				}
			}
		}
		//向左子树中删除
		else if (node->element.first > theKey) {
			node->left = erase(node->left, theKey);
			//检查平衡状态
			if (height(node->right) - height(node->left) == 2) {
				if (height(node->right->right) >= height(node->right->left)) {
					return leftRotation(node);
				}
				else {
					return rightLeftRotation(node);
				}
			}return node;
		}
		else {//向右子树中删除
			node->right = erase(node->right, theKey);
			//检查平衡状态
			if (height(node->left) - height(node->right) == 2) {
				if (height(node->left->right) > height(node->left->left)) {
					return leftRightRotation(node);
				}
				else {
					return rightRotation(node);
				}
			}return node;
		}
	}
	//右旋
	AVLNode<K, E>* rightRotation(AVLNode<K, E>* node) {
		auto l = node->left;
		node->left = l->right;
		l->left = node;
		return l;
	}
	//左旋
	AVLNode<K, E>* leftRotation(AVLNode<K, E>* node) {
		auto r = node->right;
		node->right = r->left;
		r->left = node;
		return r;
	}
	//右左
	AVLNode<K, E>* rightLeftRotation(AVLNode<K, E>* node) {
		node->right = rightRotation(node->right);
		return leftRotation(node);
	}
	//左右
	AVLNode<K, E>* leftRightRotation(AVLNode<K, E>* node) {
		node->left = leftRotation(node->left);
		return rightRotation(node);
	}

public:
	AVLTree() {
		root = nullptr;
		treeSize = 0;
	}
	~AVLTree() {
		postOrder([](AVLNode<K, E>* p) {
			if (p)
				delete p;
			});
	}

	void inOrder(void (*visit)(AVLNode<K, E>*)) {
		if (root == nullptr)
			return;
		AVLNode<K, E>* p = root;
		stack<AVLNode<K, E>*> s;
		while (p != nullptr || !s.empty()) {
			if (p != nullptr) {
				s.push(p);
				p = p->left;
			}
			else {
				p = s.top();
				s.pop();
				visit(p);
				p = p->right;
			}
		}
	}
	void preOrder(void (*visit)(AVLNode<K, E>*)) {
		if (root == nullptr) {
			return;
		}
		AVLNode<K, E>* p = root;
		stack<AVLNode<K, E>*> s;
		s.push(p);
		while (!s.empty()) {
			p = s.top();
			s.pop();
			visit(p);
			if (p->right != nullptr) {
				s.push(p->right);
			}
			if (p->left != nullptr) {
				s.push(p->left);
			}
		}
	}
	void postOrder(void (*visit)(AVLNode<K, E>*)) {
		if (root == nullptr) {
			return;
		}
		AVLNode<K, E>* p = root;
		stack<AVLNode<K, E>*> q1;
		stack<AVLNode<K, E>*> q2;
		q1.push(p);
		while (!q1.empty()) {
			p = q1.top();
			q1.pop();
			q2.push(p);
			if (p->left != nullptr) {
				q1.push(p->left);
			}
			if (p->right != nullptr) {
				q1.push(p->right);
			}
		}while (!q2.empty()) {
			p = q2.top();
			q2.pop();
			visit(p);
		}
	}
	void levelOrder(void (*visit)(AVLNode<K, E>*)) {
		if (root == nullptr) {
			return;
		}
		AVLNode<K, E>* p = root;
		queue<AVLNode<K, E>*> que;
		que.push(p);
		while (!que.empty()) {
			p = que.front();
			que.pop();
			visit(p);
			if (p->left != nullptr) {
				que.push(p->left);
			}if (p->right != nullptr) {
				que.push(p->right);
			}
		}
	}


	bool empty()const { return treeSize == 0; }
	int size()const { return treeSize; }

	//提供公有接口
	void insert(const pair<K, E>& theElement) {
		root = insert(root, theElement);
	}
	void erase(const K &theKey) {
		root = erase(root, theKey);
	}
};


int main() {
	AVLTree<int, int> a;
	for (int i = 0; i < 100; i++) {
		a.insert({ i,i });
	}
	clog << a.size() << endl;
	for (int i = 0; i < 100; i += 2) {
		a.erase(i);
	}
	clog << a.size() << endl;
	a.inOrder([](AVLNode<int, int>* p) {
		cout << p->element.first << " ";
		});

}
