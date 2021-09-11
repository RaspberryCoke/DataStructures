#include<algorithm>
#include<iostream>
#include<initializer_list>
#include<vector>
using namespace std;

template<class T>
class linear_list {
public:
	virtual ~linear_list() {};
	virtual bool empty() const = 0;
	virtual size_t size() const = 0;
	virtual int indexof(const T& theElement)const = 0;
	virtual T& get(int theIndex) = 0;
	virtual void insert(int theIndex, const T& theElement) = 0;
	virtual void erase(int theIndex) = 0;
	virtual void output(ostream& os)const = 0;
};

template<class T>
struct chainNode {
	T element;
	chainNode<T>* next;

	chainNode(const T& theElement = 0, chainNode<T>* p = nullptr) :element(theElement), next(p) {};
};

template<class T>
class chain :public linear_list<T> {
protected:
	chainNode<T>* firstNode;
	int listSize;
	void check_index(int i) {
		if (i<0 || i>listSize)
			throw runtime_error{ "数组访问溢出" };
	}
public:
	class iterator {
	protected:
		chainNode<T>* node;
	public:
		iterator(chainNode<T>* n) {
			node = n;
		}

		T& operator*() {
			return node->element;
		}
		T* operator->() {
			return &node->element;
		}
		iterator& operator++() {
			node = node->next;
			return *this;
		}
		iterator operator++(int) {
			auto p = node;
			node = node->next;
			return iterator(p);
		}
		bool operator==(const iterator& right) {
			return node == right.node;
		}
		bool operator!=(const iterator& right) {
			return(!(node == right.node));
		}
	};
	iterator begin() {
		return iterator(firstNode);
	}
	iterator end() {
		return iterator(nullptr);
	}
	chain(int initialnum = 0) {
		if (initialnum < 0)
			throw runtime_error{ "初始化出错" };
		firstNode = nullptr;
		listSize = 0;
	}
	chain(const chain<T>& t) {
		listSize = t.listSize;
		if (listSize == 0) {
			firstNode = nullptr;
			return;
		}
		chainNode<T>* des = firstNode = new chainNode<T>(t.firstNode->element);
		chainNode<T>* source = t.firstNode->next;
		while (source->next != nullptr) {
			des->next = new chainNode<T>(source->element);
			des = des->next;
			source = source->next;
		}
		des->next = nullptr;
	}
	chain(const chain<T>&& t) {
		listSize = t.listSize;
		firstNode = t.firstNode;
		t.listSize = 0;
		t.firstNode = nullptr;
	}
	chain(initializer_list<T> t) {
		listSize = t.size();
		vector<chainNode<T>*>  p(listSize);
		int i = 0;
		for (auto& x : t) {
			p[i++] = new chainNode<T>(x);
		}
		for (int e = 0; e < listSize - 1; e++) {
			p[e]->next = p[e + 1];
		}
		firstNode = p[0];
	}
	~chain() {

		chainNode<T>* q{};
		while (firstNode != nullptr) {
			q = firstNode;
			firstNode = firstNode->next;
			delete q;
		}
	}

	bool empty() const override { return listSize == 0; }
	size_t size() const override { return listSize; }
	T& get(int theIndex) {
		check_index(theIndex);
		int i = 0;
		auto p = firstNode;
		while (i != theIndex) {
			p = p->next;
			i++;
		}
		return p->element;
	}
	int indexof(const T& theElement)const override {
		int i = 0;
		chainNode<T>* temp{ firstNode };
		while (temp && temp->element != theElement) {
			temp = temp->next;
			i++;
		}
		return listSize == i ? -1 : i;
	}
	void erase(int theIndex)override {
		check_index(theIndex);
		if (listSize == 0)
			throw runtime_error{ "不能erase空链表" };
		if (theIndex == 0) {
			auto p = firstNode;
			firstNode = firstNode->next;
			delete p;
			listSize--;
			return;
		}
		int i = 0;
		chainNode<T>* deleteNode{ firstNode };
		while (i + 1 != theIndex) {
			deleteNode = deleteNode->next;
			i++;
		}
		auto p = deleteNode->next;
		if (p) {
			deleteNode->next = p->next;
			delete p;
		}
		listSize--;
	}
	void insert(int theIndex, const T& theElement) override {
		check_index(theIndex);
		if (theIndex == 0) {
			firstNode = new chainNode<T>(theElement, firstNode);
			listSize++;
			return;
		}
		int i = 0;
		auto p{ firstNode };
		while (i + 1 != theIndex) {
			p = p->next;
			i++;
		}
		chainNode<T>* insertNode = new chainNode<T>(theElement);
		auto t = p->next;
		p->next = insertNode;
		insertNode->next = t;
		listSize++;
	}
	void output(ostream& os)const override {
		for (auto p = firstNode; p != nullptr; p = p->next) {
			os << p->element << " ";
		}os << endl;
	}

};
template<class T>
ostream& operator<<(ostream& os, chain<T>& c) {
	c.output(os);
	return os;
}

int main() {
	chain<int> b{ 1,2,3,4,5,6 };
	b.insert(0, 0);
	b.insert(0, -1);
	clog << b;
	clog << b.size();
	for (int i = 0; i < 8; i++) {
		b.erase(0);
		clog << b;
	}
	clog << b.size() << endl;
	chain<int>::iterator a(b.begin());
	//clog << *a;
}