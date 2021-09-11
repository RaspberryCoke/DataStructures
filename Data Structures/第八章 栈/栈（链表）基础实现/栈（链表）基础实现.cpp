#include<iostream>
using namespace std;

template<class T>
class stack {
	virtual void pop()=0;
	virtual void push(const T& theElement) = 0;
	virtual int size()const = 0;
	virtual bool empty()const = 0;
	virtual T& top() = 0;
};
template<class T>
struct chainNode {
	T element;
	chainNode<T>* pre;
};

template<class T>
class linkedStack :public stack<T> {
private:
	chainNode<T>* stackTop;
	int stackSize;
public:
	linkedStack(int initialNum = 10) {
		if (initialNum < 1) {
			clog << "构造函数传入的参数过小" << endl;
			throw runtime_error{ "构造函数传入的参数过小" };
		}
		stackTop = nullptr;
		stackSize = 0;
	}
	~linkedStack() {
		while (stackTop != nullptr) {
			auto ptr = stackTop->pre;////
			delete stackTop;
			stackTop = ptr;
		}
	}
	bool empty()const override {
		return stackSize == 0;
	}
	int size()const override {
		reurn stackSize;
	}
	T& top() {
		if (stackSize == 0) {
			clog << "不能top()一个空栈" << endl;
			throw runtime_error{ "不能top()一个空栈" };
		}
		return stackTop->element;
	}
	void pop()override {
		if (stackSize == 0) {
			clog << "不能pop()一个空栈" << endl;
			throw runtime_error{ "不能pop()一个空栈" };
		}
		auto p = stackTop->pre;
		delete stackTop;
		stackTop = p;
		stackSize--;
	}
};