#include<iostream>
#include<initializer_list>
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
	virtual ostream& output(ostream& os)const = 0;
};

template<class T>
struct chainNode {
	T element;
	chainNode<T>* next;
	chainNode<T>* pre;
	chainNode(const T& theElement = 0, chainNode<T>* pr = nullptr,chainNode<T>* ne=nullptr) {
		element = theElement;
		pre = pr;
		next = ne;
	}
};


template<class T>
class doublyLinkedList :public linear_list<T> {
public:
	doublyLinkedList(int i);
	doublyLinkedList(const doublyLinkedList<T>& list);
	doublyLinkedList(initializer_list<T> list);

	~doublyLinkedList();

	bool empty() const override;
	size_t size() const override;
	int indexof(const T& theElement)const override;
	T& get(int theIndex) override;
	void insert(int theIndex, const T& theElement) override;
	void erase(int theIndex) override;
	ostream& output(ostream& os)const override;

protected:
	chainNode<T>* firstNode;
	chainNode<T>* lastNode;
	size_t listSize;
	void check_index(int theIndex);
};

template<class T>
doublyLinkedList<T>::doublyLinkedList(int i) {
	firstNode = lastNode = nullptr;
}

template<class T>
doublyLinkedList<T>::doublyLinkedList(const doublyLinkedList<T>& list) {
	listSize = list.listSize;
	if (listSize == 0) {
		firstNode = lastNode = nullptr;
		return;
	}
	if (listSize == 1) {
		firstNode = lastNode = new chainNode<T>(list.firstNode->element);
		firstNode->next = firstNode;
		firstNode->pre = firstNode;
		return;
	}

	firstNode = new chainNode<T>(list.firstNode->element);
	lastNode = new chainNode<T>(list.lastNode->element,firstNode,firstNode);
	firstNode->pre = lastNode;


	chainNode<T>* des=firstNode;
	chainNode<T>* source = list.firstNode;

	while (source->next!=list.lastNode)
	{
		des->next = new chainNode<T>(source->next->element,des);
		des = des->next;
		source = source->next;
	}
	des->next = lastNode;
	return;
}
template<class T>
doublyLinkedList<T>::doublyLinkedList(initializer_list<T> list) {	
	listSize = list.size();
	const int *ini_ptr = list.begin();
	if (listSize == 0) {
		firstNode = lastNode = nullptr;
		return;
	}
	
	if (listSize == 1) {
		firstNode = lastNode = new chainNode<T>(ini_ptr[0]);
		firstNode->next = firstNode;
		firstNode->pre = firstNode;
		return;
	}

	firstNode = new chainNode<T>(ini_ptr[0]);
	auto des = firstNode;

	int i = 1;
	while (i != listSize) {
		des->next = new chainNode<T>(ini_ptr[i],des);
		des = des->next;
		i++;
	}
	lastNode = des;
	des->next = firstNode;
	firstNode->pre = lastNode;
}

template<class T>
doublyLinkedList<T>::~doublyLinkedList() {
	int i = 0;
	auto p = firstNode;
	auto del = firstNode->next;
	while (i + 1 != listSize) {
		delete p;
		p = del;
		del = del->next;
		i++;
	}delete lastNode;
}
template<class T>
void doublyLinkedList<T>::check_index(int i) {
	if (i<0 || i>(int)listSize) {
		clog << "访问因越界被阻止：error!";
		throw runtime_error{ "访问越界" };
	}
}
template<class T>
bool doublyLinkedList<T>::empty()const {
	return listSize == 0;
}

template<class T>
size_t doublyLinkedList<T>::size()const {
	return listSize;
}
template<class T>
void doublyLinkedList<T>::insert(int theIndex, const T& theElement) {
	check_index(theIndex);
	if (listSize == 0) {
		firstNode = lastNode = new chainNode<T>(theElement);
		firstNode->pre = firstNode;
		firstNode->next = firstNode;
		listSize++;
		return;
	}
	if (theIndex == 0) {
		chainNode<T>* p = new chainNode<T>(theElement, lastNode, firstNode);
		firstNode = p;
		listSize++;
		return;
	}
	int i = 0;
	auto p = firstNode;
	while (i + 1 != theIndex) {
		p = p->next;
		i++;
	}
	auto t = p->next;
	p->next = new chainNode<T>(theElement, p, t);
	t->pre = p->next;

	listSize++;
}
template<class T>
void doublyLinkedList<T>::erase(int theIndex) {
	check_index(theIndex);
	if (listSize == 0) {
		clog << "erase 函数出错，不能erase一个大小为0的对象";
		throw runtime_error{ "erase 函数出错" };
	}
	if (listSize == 1) {
		delete firstNode;
		firstNode = lastNode = nullptr;
		return;
	}
	if (theIndex <(int) listSize / 2) {
		int i = 0;
		auto p = firstNode;
		while (i+1!=theIndex)
		{
			p = p->next;
		}
		auto t = p->next->next;
		delete p->next;
		p->next = t;
		t->pre = p;
		listSize--;
		return;
	}
	else {
		int i = theIndex - 1;
		auto p = lastNode;
		if (theIndex == listSize - 1) {
			p->pre->next = firstNode;
			firstNode->pre = p->pre;
			lastNode = p->pre;
			delete p;
			listSize--;
			return;
		}
		while (i-1!= theIndex) {
			p = p->pre;
		}
		auto t = p->pre->pre;
		delete p->pre;
		p->pre = t;
		t->next = p;
		listSize--;
	}
}
template<class T>
T& doublyLinkedList<T>::get(int theIndex) {
	check_index(theIndex);
	if (theIndex < (int)listSize / 2) {
		auto p = firstNode;
		int i = 0;
		while (i != theIndex) {
			p = p->next;
		}
		return p->element;
	}
	else {
		auto p = lastNode;
		int i = listSize - 1;
		while (i != theIndex) {
			p = p->pre;
			i--;
		}
		return p->element;
	}
}

template<class T>
int doublyLinkedList<T>::indexof(const T& theElement)const {
	int i = 0;
	auto p = firstNode;
	while (i != listSize) {
		if (p->element == theElement) {
			return i;
		}
		p = p->next;
		i++;
	}
	return -1;
}
template<class T>
ostream& doublyLinkedList<T>::output(ostream& os)const {
	int i = 0;
	auto p = firstNode;
	while (i != listSize) {
		os << p->element << " " ;
		i++;
		p=p->next;
	}
	os << endl;
	return os;
}

int main() {
	doublyLinkedList<int> a({ 1,2,3,4,5 });
	a.output(clog);
	a.insert(0, 0);
	a.insert(0, -1);
	a.output(clog);
	a.erase(6);
	a.erase(5);
	a.output(clog);
	clog << a.indexof(3) << endl;
	clog << a.get(2)<<endl;
	clog << a.size()<<endl;
}
