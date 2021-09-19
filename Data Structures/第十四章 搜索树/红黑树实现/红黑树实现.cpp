#include<iostream>
#include<stack>
#include<queue>
#define black 1
#define red 0
using namespace std;
template<class K, class E>
struct RBTNode {
	pair<K, E> element;
	bool color;
	RBTNode* left, * right, * parent;
	RBTNode(pair<K, E> ele, RBTNode<K, E>* l = nullptr, RBTNode<K, E>* r = nullptr, RBTNode<K, E>* p = nullptr) : element(ele), color(red), left(l), right(r), parent(p) { }

	RBTNode* grandparent() {
		if (parent == nullptr) { return nullptr; }
		return parent->parent;
	}
	RBTNode* uncle() {
		if (grandparent() == nullptr) {
			return nullptr;
		}
		if (parent == grandparent()->left) {
			return grandparent()->right;
		}
		else {
			return grandparent()->left;
		}
	}
	RBTNode* sibling() {
		if (parent == nullptr) {
			return nullptr;
		}
		if (this == parent->left) {
			return parent->right;
		}
		else {
			return parent->left;
		}
	}
};
template<class K, class E>
class RBTree {
private:
	RBTNode<K, E>* root;
	RBTNode<K, E>* NIL;
	int treeSize;
private:
	RBTNode<K, E>* rightRotation(RBTNode<K, E>* p);
	RBTNode<K, E>* leftRotation(RBTNode<K, E>* p);
	void insert(RBTNode<K, E>* p, pair<K, E> theElement);
	void erase(RBTNode<K, E>* p, K theKey);
	RBTNode<K, E>* nearest_node(RBTNode<K, E>* p);
	void erase_node(RBTNode<K, E>* p);
	void check_after_delete(RBTNode<K, E>* p);
	void check_after_insert(RBTNode<K, E>* p);
	void toBlack(RBTNode<K, E>* p);
	void toRed(RBTNode<K, E>* p);
	bool isRed(RBTNode<K, E>* p);
	bool isBlack(RBTNode<K, E>* p);
public:
	RBTree();
	~RBTree();
	int size()const { return treeSize; }
	int empty()const { return treeSize == 0; }
	void insert(pair<K, E> theElement);
	void erase(int data);
	void inOrder(void (*visit)(RBTNode<K, E>*));
	void preOrder(void (*visit)(RBTNode<K, E>*));
	void postOrder(void (*visit)(RBTNode<K, E>*));
	void levelOrder(void (*visit)(RBTNode<K, E>*));
};
template<class K, class E>
void RBTree<K, E>::levelOrder(void (*visit)(RBTNode<K, E>*)) {
	if (root == nullptr) {
		return;
	}
	RBTNode<K, E>* p = root;
	queue<RBTNode<K, E>*> que;
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

template<class K, class E>
void RBTree<K, E>::postOrder(void (*visit)(RBTNode<K, E>*)) {
	if (root == nullptr) {
		return;
	}
	RBTNode<K, E>* p = root;
	stack<RBTNode<K, E>*> q1;
	stack<RBTNode<K, E>*> q2;
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

template<class K, class E>
void RBTree<K, E>::preOrder(void (*visit)(RBTNode<K, E>*)) {
	if (root == nullptr) {
		return;
	}
	RBTNode<K, E>* p = root;
	stack<RBTNode<K, E>*> s;
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

template<class K, class E>
void  RBTree<K, E>::erase(int theKey) {
	erase(root, theKey);
}

template<class K, class E>
void RBTree<K, E>::insert(pair<K, E> theElement) {
	if (root == nullptr) {
		root = new RBTNode<K, E>(theElement);
		root->color = black;
		treeSize++;
	}
	else {
		insert(root, theElement);
	}
}

template<class K, class E>
void RBTree<K, E>::inOrder(void (*visit)(RBTNode<K, E>*)) {
	if (root == nullptr)
		return;
	RBTNode<K, E>* p = root;
	stack<RBTNode<K, E>*> s;
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

template<class K, class E>
RBTree<K, E>::~RBTree() {
	if (root)
		postOrder([](RBTNode<K, E>* p) {
		delete p;
			});
	delete NIL;
}

template<class K, class E>
RBTree<K, E>::RBTree() {
	NIL = new RBTNode<K, E>({ 0,0 });
	NIL->color = black;
	root = nullptr;
	treeSize = 0;
}

template<class K, class E>
bool RBTree<K, E>::isBlack(RBTNode<K, E>* p) {
	if (p == nullptr) {
		return true;
	}
	if (p->color == red) {
		return false;
	}
	else {
		return true;
	}
}

template<class K, class E>
bool RBTree<K, E>::isRed(RBTNode<K, E>* p) {
	if (p == nullptr)
		return false;
	if (p->color == red) {
		return true;
	}
	else {
		return false;
	}
}

template<class K, class E>
void RBTree<K, E>::toRed(RBTNode<K, E>* p) {
	if (p == nullptr) {
		return;
	}
	p->color = red;
}

template<class K, class E>
void RBTree<K, E>::toBlack(RBTNode<K, E>* p) {
	if (p == nullptr)
		return;
	p->color = black;
}

template<class K, class E>
RBTNode<K, E>* RBTree<K, E>::rightRotation(RBTNode<K, E>* p) {

	RBTNode<K, E>* par = p->parent;
	auto l = p->left;
	p->left = l->right;
	if (l->right != nullptr) {
		l->right->parent = p;
	}

	p->parent = l;
	l->right = p;
	l->parent = par;
	if (par != nullptr) {
		if (p == par->right) {
			par->right = l;
		}
		else {
			par->left = l;
		}
	}
	return l;
}

template<class K, class E>
RBTNode<K, E>* RBTree<K, E>::leftRotation(RBTNode<K, E>* p) {
	RBTNode<K, E>* par = p->parent;
	auto l = p->right;
	p->right = l->left;
	if (l->left != nullptr) {
		l->left->parent = p;
	}

	p->parent = l;
	l->parent = par;
	l->left = p;
	if (par != nullptr) {
		if (p == par->right) {
			par->right = l;
		}
		else {
			par->left = l;
		}
	}
	return l;
}

template<class K, class E>
void RBTree<K, E>::insert(RBTNode<K, E>* p, pair<K, E> theElement) {

	if (p->element.first > theElement.first) {
		if (p->left != nullptr) {
			insert(p->left, theElement);
		}
		else {
			RBTNode<K, E>* temp = new RBTNode<K, E>(theElement);
			treeSize++;
			temp->parent = p;
			p->left = temp;
			check_after_insert(temp);
		}
	}
	else if (p->element.first < theElement.first) {
		if (p->right != nullptr) {
			insert(p->right, theElement);
		}
		else {
			RBTNode<K, E>* temp = new RBTNode<K, E>(theElement);
			treeSize++;
			temp->parent = p;
			p->right = temp;
			check_after_insert(temp);
		}
	}
	else {//覆盖
		p->element = theElement;
	}
}

template<class K, class E>
void RBTree<K, E>::erase(RBTNode<K, E>* p, K theKey) {
	if (p->element.first > theKey) {//从左子树中删除
		if (p->left == nullptr) { return; }
		erase(p->left, theKey);
		return;
	}
	else if (p->element.first < theKey) {//从右子树中删除
		if (p->right == nullptr) { return; }
		erase(p->right, theKey);
		return;
	}
	else if (p->element.first == theKey) {//找到该节点
		RBTNode<K, E>* temp = nearest_node(p);
		swap(p->element, temp->element);
		erase_node(temp);
		return;
	}
	else {//没有这个节点
		return;
	}
}

template<class K, class E>
RBTNode<K, E>* RBTree<K, E>::nearest_node(RBTNode<K, E>* p) {
	if (p->left != nullptr) {//左子树中最大的
		p = p->left;
		while (p->right != nullptr) {
			p = p->right;
		}
		return p;
	}
	else if (p->right != nullptr) {//右子树中最小的
		p = p->right;
		while (p->left != nullptr) {
			p = p->left;
		}
		return p;
	}
	return p;//没有孩子，返回自己
}

template<class K, class E>
void RBTree<K, E>::erase_node(RBTNode<K, E>* p) {
	//这个时候还是可能有子树的，但最多有一个
	RBTNode<K, E>* child = p->left == nullptr ? p->right : p->left;
	if (p == root) {
		if (p->left == nullptr && p->right == nullptr) {
			root = p = nullptr;
			delete p;
			treeSize--;
		}
		else {
			delete p;
			treeSize--;
			child->parent = nullptr;
			root = child;
			toBlack(root);
		}
		return;
	}
	//无孩子
	if (p->left == nullptr && p->right == nullptr) {
		if (isRed(p)) {
			if (p = p->parent->right) {
				p->parent->right = nullptr;
			}
			else {
				p->parent->left = nullptr;
			}
			delete p;
			treeSize--;
			return;
		}
		auto par = p->parent;
		if (p == par->left) {
			par->left = NIL;
			NIL->parent = par;
		}
		else {
			par->right = NIL;
			NIL->parent = par;
		}

		delete p;
		treeSize--;
		p = NIL;
		check_after_delete(p);

		if (p->parent->left == p) {
			p->parent->left = nullptr;
			p->parent = nullptr;
		}
		else {
			p->parent->right = nullptr;
			p->parent = nullptr;
		}
		return;
	}
	//有一个孩子
	if (p->parent->left == p) {
		p->parent->left = child;
	}
	else {
		p->parent->left = child;
	}
	child->parent = p->parent;

	if (p->color == black) {
		if (isRed(child)) {
			toBlack(child);
			delete p;
			treeSize--;
		}
		else {
			delete p;
			treeSize--;
			check_after_delete(child);
		}
	}
}

template<class K, class E>
void RBTree<K, E>::check_after_delete(RBTNode<K, E>* p) {
	RBTNode<K, E>* par = p->parent;
	if (par == nullptr) {
		toBlack(root);
		return;
	}

	if (isRed(p->sibling())) {
		toRed(par);
		toBlack(p->sibling());
		if (p == par->left) {
			if (par == root) {
				root = leftRotation(par);
			}
			else {
				leftRotation(par);
			}
		}
		else {
			if (par == root) {
				root = leftRotation(par);
			}
			else {
				rightRotation(par);
			}
		}
	}
	if (isBlack(par) && isBlack(p->sibling()) && isBlack(p->sibling()->left) && isBlack(p->sibling()->right)) {
		toRed(p->sibling());
		check_after_delete(par);//递归从头开始，以parent作为调整的基准
	}
	else if (isRed(par) && isBlack(p->sibling()) && isBlack(p->sibling()->left) && isBlack(p->sibling()->right)) {
		toRed(p->sibling());
		toBlack(par);
	}
	else {
		if (isBlack(p->sibling())) {
			if (p == par->left && isRed(p->sibling()->left) && isBlack(p->sibling()->right)) {
				toRed(p->sibling());
				toBlack(p->sibling()->left);
				rightRotation(p->sibling());
			}
			else if (p == par->right && isBlack(p->sibling()->left) && isRed(p->sibling()->right)) {
				toRed(p->sibling());
				toBlack(p->sibling()->right);
				leftRotation(p->sibling());
			}
		}
		p->sibling()->color = par->color;
		toBlack(par);
		if (p == par->left) {
			toBlack(p->sibling()->right);
			if (par == root) {
				root = leftRotation(par);
				return;
			}
			leftRotation(par);
		}
		else {
			toBlack(p->sibling()->left);
			if (par == root) {
				root = rightRotation(par);
				return;
			}
			rightRotation(p->parent);
		}
	}
}

template<class K, class E>
void  RBTree<K, E>::check_after_insert(RBTNode<K, E>* p) {
	//第一种情况：要么是父节点是树根，要么是black，不需要考虑
	if (isBlack(p->parent)) { return; }
	//剩下的情况：必定至少是三阶高的红黑树
	//第一种情况，uncle是红色，进行简单的染色处理
	//并未区分祖宗节点的颜色，故递归检查
	if (isRed(p->uncle())) {
		toBlack(p->parent);
		toBlack(p->uncle());
		toRed(p->grandparent());
		check_after_insert(p->grandparent());
	}
	//剩下的情况：uncle节点不是红色
	else {
		if (p->parent->right == p && p->grandparent()->left == p->parent) {//LR
			p = leftRotation(p->parent);
			toBlack(p);
			toRed(p->parent);
			rightRotation(p->parent);
		}
		else if (p->parent->left == p && p->grandparent()->left == p->parent) {//LL
			toBlack(p->parent);
			toRed(p->grandparent());
			if (root == p->grandparent()) {
				root = rightRotation(p->grandparent());
			}
			rightRotation(p->grandparent());
		}
		else if (p->parent->left == p && p->grandparent()->right == p->parent) {//RL
			p = rightRotation(p->parent);
			toBlack(p);
			toRed(p->parent);
			leftRotation(p->parent);
		}
		else if (p->parent->right == p && p->grandparent()->right == p->parent) {//RR
			toBlack(p->parent);
			toRed(p->grandparent());
			if (p->grandparent() == root) {
				root = leftRotation(p->grandparent());
				return;
			}leftRotation(p->grandparent());
		}
	}toBlack(root);
}
//
//int main() {
//	RBTree<int, int> a;
//	for (int i = 0; i < 1000; i++) {
//		a.insert({ i,i });
//	}
//
//	a.inOrder([](RBTNode<int, int>* p) {
//		cout << p->element.first << " ";
//		});
//	cout << endl;
//
//	cout << a.size();
//
//	cout << endl;
//	for (int i = 0; i < 1000; i += 2) {
//		a.erase(i);
//	}
//	a.inOrder([](RBTNode<int, int>* p) {
//		cout << p->element.first << " ";
//		});
//
//	cout << endl;
//
//	cout << a.size();
//}