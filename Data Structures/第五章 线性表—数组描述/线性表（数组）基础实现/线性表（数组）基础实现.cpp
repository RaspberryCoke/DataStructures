#include<iostream>
#include <initializer_list>
using namespace std;
class linear_list {
	int* ptr;
	int size;
	int capacity;
public:
	linear_list() {
		ptr = nullptr;
		size = 0;
		capacity = 0;
	}
	linear_list(initializer_list<int>&& t) {
		if (t.size() == 0) {
			ptr = nullptr;
			size = 0;
			capacity = 0;
		}

		capacity = 2 * t.size();
		ptr = new int[capacity]();
		for (auto i : t) {
			ptr[size] = i;
			size++;
		}
	}
	linear_list(const linear_list& t) {
		size = t.size;
		capacity = t.capacity;
		ptr = new int[capacity]();
		for (int i = 0; i < size; i++) {
			ptr[i] = t.ptr[i];
		}
	}
	linear_list(linear_list&& t) {
		ptr = t.ptr;
		size = t.size;
		capacity = t.capacity;
		t.ptr = nullptr;
		t.capacity = 0;
	}
	linear_list(size_t s) {
		ptr = new int[2 * s]();
		size = 0;
		capacity = 2 * s;
	}
	linear_list& operator=(const linear_list& t) {
		size = t.size;

		if (capacity > 1)
			delete[] ptr;
		if (capacity == 1)
			delete ptr;

		capacity = t.capacity;
		ptr = new int[capacity]();
		for (int i = 0; i < size; i++) {
			ptr[i] = t.ptr[i];
		}
	}
	linear_list& operator=(linear_list&& t) {
		if (capacity > 1)
			delete[] ptr;
		if (capacity == 1)
			delete ptr;
		t.capacity = 0;
		ptr = t.ptr;
		size = t.size;
		capacity = t.capacity;

		t.ptr = nullptr;
	}
	~linear_list() {
		if (capacity > 1)
			delete[] ptr;
		if (capacity == 1)
			delete ptr;
	}
	void enlarge() {
		capacity *= 2;
		int* n = new int[capacity]();
		for (int i = 0; i < size; i++) {
			n[i] = ptr[i];
		}
		if (size == 1)
			delete ptr;
		else
			delete[]ptr;
		ptr = n;

	}

	bool empty() {
		return size == 0;
	}

	bool full() {
		return size == capacity;
	}

	int& get(int index) {
		return ptr[index];
	}

	void erase(int index) {
		if (index == size) {
			ptr[index] = 0;
			return;
		}
		for (int i = index; i < size - 1; i++) {
			ptr[i] = ptr[i + 1];
		}
		ptr[size - 1] = 0;
		size--;
	}

	void insert(int index, const int& element) {
		if ((size + 1) >= capacity) {
			enlarge();
		}
		for (int i = size; i > index; i--) {
			ptr[i] = ptr[i - 1];
		}
		ptr[index] = element;
		size++;
	}
	void reverse() {
		int temp;
		for (int i = 0; i < size / 2; i++) {
			temp = ptr[i];
			ptr[i] = ptr[size - i - 1];
			ptr[size - i - 1] = temp;
		}
	}

	void print() {
		for (int i = 0; i < size; i++) {
			cout << ptr[i] << " ";
		}
		cout << endl;
	}

	void sort() {
		for (int i = 0; i < size - 1; i++) {
			for (int t = 0; t < size - i - 1; t++) {
				if (ptr[t] > ptr[t + 1]) {
					int temp = ptr[t];
					ptr[t] = ptr[t + 1];
					ptr[t + 1] = temp;
				}
			}
		}
	}

	int index_of(int temp) {
		for (int i = 0; i < size; i++) {
			if (ptr[i] == temp)
				return i;
		}
	}
};

int main() {

	linear_list l{ 5,4,3,2,1 };

	l.print();

	l.insert(0, 0);
	l.print();
	l.erase(0);
	l.print();
	l.sort();
	l.print();
	cout << l.index_of(2);
	l.reverse();
	l.print();
}