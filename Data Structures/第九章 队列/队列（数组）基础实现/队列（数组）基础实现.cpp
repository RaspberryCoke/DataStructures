#include<iostream>

using namespace std;

template<class T>
class queue {
public:
	virtual ~queue() {}
	virtual bool empty()const =0;
	virtual int size()const = 0;
	virtual T& front() = 0;
	virtual T& back() = 0;
	virtual void pop() = 0;
	virtual void push(const T& theElement) = 0;
	virtual void output(ostream& os)const = 0;
};
template<class T>
class arrayQueue :public queue<T> {
private:
	int queueFront;
	int queueBack;
	int arrayLength;
	T* queue;
	void lengthen() {
		T* q = new T[arrayLength]{};
		if (queueBack > queueFront) {
			copy(queue + queueFront + 1, queue + queueBack + 1, q + 1);
			queueFront = 0;
			queueBack = arrayLength;
			delete[]queue;
			queue = q;
		}
		else {
			copy(queue + queueFront + 1, queue + arrayLength, q + 1);
			copy(queue, queue + queueBack + 1, q + arrayLength - queueFront);
			delete[]queue;
			queueFront = 0;
			queueBack = arrayLength;
			queue = q;
		}
		
	}

public:
	arrayQueue(int initialNum = 10) {
		if (initialNum < 1) {
			clog << "构造函数参数太小" << endl;
			throw runtime_error{ "构造函数参数太小" };
		}
		arrayLength = initialNum;
		queue = new T[arrayLength]{};
		queueBack=queueFront = 0;
	}
	~arrayQueue() {
		delete[]queue;
	}
	bool empty()const override{
		return queueFront == queueBack;
	}
	int size()const override {
		return (queueBack - queueFront + arrayLength) % arrayLength;
	}
	T& front()override {
		if (empty()) {
			clog << "队列空，不能使用front函数" << endl;
			throw runtime_error{ "队列空，不能使用front函数" };
		}
		return queue[(queueFront + 1) % arrayLength];
	}
	T& back()override {
		if (empty()) {
			clog << "队列空，不能使用back函数" << endl;
			throw runtime_error{ "队列空，不能使用back函数" };
		}
		return queue[queueBack];
	}
	void pop() override{
		if (empty()) {
			clog << "队列空，不能使用pop函数" << endl;
			throw runtime_error{ "队列空，不能使用pop函数" };
		}
		queueFront = (queueFront + 1) % arrayLength;
		queue[queueFront].~T();
	}
	void push(const T& theElement)override {
		if ((queueBack + 1) % arrayLength == queueFront) {
			lengthen();
		}
		queueBack = (queueBack + 1) % arrayLength;
		queue[queueBack] = theElement;
	}
	void output(ostream& os) const{
		auto p = queue;
		int i = (queueFront+1)%arrayLength;
		while (i <= queueBack) {
			os << queue[i] << " ";
			i = (i + 1) % arrayLength;
		}
		os << endl;
	}
};
int main() {
	arrayQueue<int> a(100);
	a.push(1);
	a.push(2);
	a.push(3);
	a.output(clog);
	a.pop();
	a.pop();
	a.output(clog);
}