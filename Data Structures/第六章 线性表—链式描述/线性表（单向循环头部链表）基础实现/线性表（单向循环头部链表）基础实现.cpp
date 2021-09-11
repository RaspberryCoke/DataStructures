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
class CircularListWithHeader :public linear_list<T> {
public:

	CircularListWithHeader(int initialNum = 10);
	CircularListWithHeader(const CircularListWithHeader<T>& list);
	CircularListWithHeader(initializer_list<T> list);
	~CircularListWithHeader();
	size_t size()const ;
	bool empty()  const override;
	int indexof(const T& theElement)  const override;
	T& get(int theIndex) override;
	void insert(int theIndex, const T& theElement)  override ;
	void erase(int theIndex)  override;
	void output(ostream& os) const override;

protected:
	chainNode<T>* headerNode;
	void check_index(int theIndex);
	int listSize;

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
			if (node->next == headerNode) {
				node = headerNode->next;
				return *this;
			}
			node = node->next;
			return *this;
		}
		iterator operator++(int) {
			iterator t = *this;
			++(*this);
			return t;
		}
		bool operator==(iterator& right) {
			return node == right.node;
		}
		bool operator!=(iterator& right) {
			return node != right.node;
		}
	};
	iterator begin() {
		return iterator(headerNode->next);
	}
	iterator end() {
		return iterator(nullptr);//不确定
	}
};
template<class T>
CircularListWithHeader<T>::~CircularListWithHeader() {
	auto temp = headerNode;
	chainNode<T>* t;
	auto p = headerNode;
	while (p != temp) {
		t = p->next;
		delete p;
		p = t;
	}
}
template<class T>
void CircularListWithHeader<T>::check_index(int theIndex) {
	if (theIndex < 0 || theIndex >= listSize)
		throw runtime_error{ "访问溢出" };
}
template<class T>
CircularListWithHeader<T>::CircularListWithHeader(int initialNum ) {
	headerNode = new chainNode<T>();
	headerNode->next = headerNode;
	listSize = 0;
}
template<class T>
CircularListWithHeader<T>::CircularListWithHeader(const CircularListWithHeader<T>& list) {
	headerNode = new chainNode<T>();
	headerNode->next = headerNode;
	if (list.listSize == 0) {
		listSize = 0;
		return;
	}

	listSize = list.listSize;

	chainNode<T>* source = list.headerNode->next;
	chainNode<T>* des = headerNode;
	while (source!=list.headerNode) {
		des->next = new chainNode<T>(source->element);
		des = des->next;
		source = source->next;
	}
	des->next = headerNode;
}
template<class T>
CircularListWithHeader<T>::CircularListWithHeader(initializer_list<T> list) {
	listSize = list.size();
	int i = 0;
	vector<chainNode<T>*> vec(listSize);
	headerNode = new chainNode<T>(0);
	for (auto& x : list) {
		vec[i] = new chainNode<T>(x,headerNode);
		i++;
	}
	headerNode->next = vec[0];

	for (auto t = 0; t + 1 != listSize;t++ ) {
		vec[t]->next = vec[t + 1];
	}
}
template<class T>
size_t CircularListWithHeader<T>::size()const {
	return listSize;
}
template<class T>
bool CircularListWithHeader<T>::empty()const {
	return listSize == 0;
}
template<class T>
int CircularListWithHeader<T>::indexof(const T & theElement)const {
	int i = 0;
	auto p = headerNode->next;
	while (i < listSize) {
		if (p->element == theElement) {
			return i;
		}
		p = p->next;
		i++;
	}
	return -1;
}
template<class T>
T& CircularListWithHeader<T>::get(int theIndex) {
	check_index(theIndex);
	int i = 0;
	auto p = headerNode->next;
	while (i != theIndex) {
		p = p->next;
	}
	return p->element;
}
template<class T>
void CircularListWithHeader<T>::insert(int theIndex, const T& theElement) {
	check_index(theIndex);

	if (theIndex == 0) {
		auto p = headerNode->next;
		headerNode->next = new chainNode<T>(theElement, p);
		listSize++;
		return;
	}

	int i = 0;
	auto p = headerNode->next;

	while (i + 1 != theIndex) {
		p = p->next;
	}

	auto temp = p->next;
	p->next = new chainNode<T>(theElement, temp);

	listSize++;
}
template<class T>
void CircularListWithHeader<T>::erase(int theIndex) {
	check_index(theIndex);
	int i = 0;
	auto p = headerNode->next;
	if (theIndex == 0) {
		headerNode->next = p->next;
		delete p;
		return;
	}
	

	while (i + 1 != theIndex) {
		p = p->next;
		i++;
	}

	auto temp = p->next;
	p->next = temp->next;
	delete temp;

	listSize--;
}

template<class T>
void CircularListWithHeader<T>::output(ostream & os)const {
	auto p = headerNode->next;
	while (p != headerNode) {
		os << p->element << " ";
		p = p->next;
	}
	os << endl;
}

int main() {
	CircularListWithHeader<int> a{ 1,2,3,4,5,6,7 };
	a.output(clog);
	a.insert(0, 0);
	a.insert(0, -1);
	cout << a.size()<<endl;
	a.output(clog);
	a.erase(8);
	a.erase(0);
	a.output(clog);

	CircularListWithHeader<int>::iterator m(a.begin());
	cout << *m;
}