#include<iostream>
#include<stack>
#include<queue>
using namespace std;

template<class K,class E>
struct AVLNode {
	pair<K, E> element;
	AVLNode<K, E>* left;
	AVLNode<K, E>* right;
	int height;

	AVLNode(const pair<K, E>& ele = make_pair(0, 0), AVLNode<K, E>* l = nullptr, AVLNode<K, E>* r = nullptr) {
		element = ele;
		left = l;
		right = r;
		updateheight(this);
	}
	int updateheight(AVLNode<K, E>* t) {
		if (t == nullptr)
			return 0;
		else {
			t->height = max(updateheight(t->left), updateheight(t->right)) + 1;
			return t->height;
		}

	}
};

template<class K,class E>
class AVLTree {
private:
	AVLNode<K, E>* root;
	int treeSize;

	int height(AVLNode<K, E>* t) {
		if (t == nullptr)
			return 0;
		else {
			t->height = max(height(t->left), height(t->right) )+ 1;
			return t->height;
		}
			
	}
	AVLNode<K, E>* insert(AVLNode<K, E>* node, const pair<K, E>& theElement) {
		if (node == nullptr) {
			node = new AVLNode<K, E>(theElement);
			treeSize++;
			return node;
		}

		if (theElement.first < node->element.first) {//插入到左子树
			node->left=insert(node->left, theElement);

			int t = height(node->left) - height(node->right);//检查node节点的平衡状态
			if (t == 2) {
				if (theElement.first > node->left->element.first) {//左右
					node=leftRightRotation(node);
				}
				else {//左左
					node = rightRotation(node);
				}
			}
		}
		else if(theElement.first > node->element.first){//插入到右子树
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
			node->element.second = theElement.second;
			return node;
		}
		node->height = max(height(node->left), height(node->right)) + 1;
		return node;
	}
	AVLNode<K, E>* erase(AVLNode<K, E>* node, const pair<K, E>& theElement) {
		if (node == nullptr)
			return nullptr;
		if (node->element.first == theElement.first) {
			if (node->left && node->right) {//两个分支
				if (node->left->height >= node->right->height) {
					AVLNode<K, E>* tmp = node->left;
					while (tmp->right) {//找到左子树中的最大元素
						tmp = tmp->right;
					}
					node->element = tmp->element;
					node->left=erase(node->left, tmp->element);//递归的删除，因为要考虑平衡问题
					return node;
				}
				else {
					AVLNode<K, E>* tmp = node->right;
					while (tmp->left) {
						tmp = tmp->left;
					}
					node->element = tmp->element;
					node->right = erase(node->right, tmp->element);
					return node;
				}
			}
			else {
				if (node->left) {
					AVLNode<K, E>* tmp2 = node;
					node = node->left;
					delete tmp2;
					treeSize--;
					return node;
				}
				else if (node->right) {
					AVLNode<K, E>* tmp3 = node;
					node = node->right;
					delete tmp3;
					treeSize--;
					return node;
				}
				else {
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
		else if (node->element.first > theElement.first) {
			node->left=erase(node->left, theElement);
			if (height(node->right) - height(node->left) == 2) {
				if (height(node->right->right) >=height(node->right->left) ){
					return leftRotation(node);
				}
				else {
					return rightLeftRotation(node);
				}

			}return node;
		}
		else {
			node->right=erase(node->right,theElement);
				if (height(node->left) - height(node->right) == 2) {
					if (height(node->left->right) > height(node->left->left)){
						return leftRightRotation(node);
					}
					else {
						return rightRotation(node);
					}

				}return node;
		}

	}
	AVLNode<K, E>* rightRotation(AVLNode<K, E>* node) {
		auto l = node->left;
		node->left = l->right;
		l->left = node;
		return l;
	}
	AVLNode<K, E>* leftRotation(AVLNode<K, E>* node) {
		auto r = node->right;
		node->right = r->left;
		r->left = node;
		return r;
	}
	AVLNode<K, E>* rightLeftRotation(AVLNode<K, E>* node) {
		node->right=rightRotation(node->right);
		return leftRotation(node);
	}
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

	void inOrder(void (*visit)(AVLNode<K,E>*)) {
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
	void preOrder(void (*visit)(AVLNode<K,E>*)) {
		if (root == nullptr) {
			return;
		}
		AVLNode<K,E>* p = root;
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
	void postOrder(void (*visit)(AVLNode<K,E>*)) {
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
	void insert(const pair<K, E>& theElement) {
		root=insert(root, theElement);
	}
	void erase(const pair<K, E>& theElement) {
		root=erase(root, theElement);
	}
};


int main() {
	AVLTree<int, int> a;
	for (int i = 0; i < 100; i++) {
		a.insert({ i,i });
	}
	for (int i = 0; i < 100; i+=2) {
		a.erase({ i,i });
	}
	a.inOrder([](AVLNode<int, int>* p) {
		cout << p->element.first << " ";
		});


}