#pragma once
#include<iostream>
using namespace std;

template<class T>
class stack {
public:
	virtual ~stack() {};
	virtual bool empty()const = 0;
	virtual int size()const = 0;
	virtual T& top() = 0;
	virtual void pop() = 0;
	virtual void push(const T& theElement) = 0;
};

template<class T>
class arrayStack :public stack<T> {
public:
	arrayStack(int initialNum = 10);
	arrayStack(const arrayStack& arr);
	arrayStack(initializer_list<T> t);
	~arrayStack();
	bool empty()const override;
	int size()const override;
	T& top() override;
	void pop() override;
	void push(const T& theElement) override;
	ostream& output(ostream& os);
private:
	int stackTop;
	int arrayLength;
	T* stack;
	void lengthen();
};
template<class T>
ostream& arrayStack<T>::output(ostream& os) {
	if (stackTop == -1) {
		clog << "不能output一个空栈" << endl;
		throw runtime_error{ "不能output一个空栈" };
	}
	int i = 0;
	while (i <= stackTop) {
		os << stack[i] << " ";
		i++;
	}
	os << endl;
	return os;
}
template<class T>
arrayStack<T>::arrayStack(const arrayStack<T>& arr) {
	stackTop = arr.stackTop;
	arrayLength = arr.stackTop;
	stack = new T[arrayLength]{};
	copy(arr.stack, arr.stack + arrayLength, stack);

}
template<class T>
arrayStack<T>::arrayStack(initializer_list<T> list) {
	arrayLength = list.size() * 2;
	stackTop = list.size() - 1;
	stack = new T[arrayLength]{};
	copy(list.begin(), list.end(), stack);
}
template<class T>
arrayStack<T>::arrayStack(int i) {
	if (i < 1) {
		clog << "参数错误，初始化容量太小" << endl;
		throw runtime_error{ "初始化容量太小" };
	}
	arrayLength = i;
	stack = new T[i]{};
	stackTop = -1;
}
template<class T>
bool arrayStack<T>::empty()const {
	return stackTop == -1;
}
template<class T>
int arrayStack<T>::size()const {
	return stackTop + 1;
}
template<class T>
void arrayStack<T>::pop() {
	if (stackTop == -1) {
		clog << "不能pop空栈";
		throw runtime_error{ "不能pop空栈" };
	}
	stack[stackTop--].~T();
}
template<class T>
void arrayStack<T>::lengthen() {
	T* p = new T[arrayLength * 2]{};
	copy(stack, stack + arrayLength, p);
	auto temp = stack;
	stack = p;
	delete[] temp;
	arrayLength *= 2;
}

template<class T>
void arrayStack<T>::push(const T& theElement) {
	if (stackTop + 1 == arrayLength) {
		lengthen();
	}
	stack[++stackTop] = theElement;
}
template<class T>
arrayStack<T>::~arrayStack() {
	delete[] stack;
}
template<class T>
T& arrayStack<T>::top() {
	if (stackTop == -1) {
		clog << "不能top()一个空栈" << endl;
		throw runtime_error{ "不能top()一个空栈" };
	}
	return stack[stackTop];
}