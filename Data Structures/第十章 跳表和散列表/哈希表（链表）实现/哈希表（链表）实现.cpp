#include<iostream>
#include"sortedChain.h"
using namespace std;
/*
为了完善链表，把每个链表的末尾设置为某个非常大的数字。
这里默认为LONG_MAX。但可能会存在问题。

*/
template<class K,class E>
class hashChains :public dictionary<K, E> {
private:
	sortedChain<K, E>* table;
	int dSize;
	int divisor;
	std::hash<K>hash;
public:

	hashChains(int Thedivisor = 11) {
		divisor = Thedivisor;
		table = new sortedChain<K, E>[divisor];
		dSize = 0;
	}
	~hashChains() { delete[]table;}
	bool empty()const override {return dSize == 0;}
	int size()const override { return dSize; }
	std::pair<K, E>* find(const K& theValue) const{ return table[hash(theValue)%divisor].find(theValue); }
	void insert(const pair<K, E>& thePair) override{
		int index = hash(thePair.first) % divisor;
		int tempSize = table[index].size();
		table[hash(thePair.first)%divisor].insert(thePair);
		if (tempSize < table[index].size())
			dSize++;
	}
	void erase(const K& theElement)override {
		int index = hash(theElement) % divisor;
		int tempSize = table[index].size();
		table[index].erase(theElement);
		if (tempSize > table[index].size())
			dSize--;
	}
	void output(ostream& os)const {
		int i = 0;
		while (i < divisor) {
			cout << i + 1 << "行：  ";
			if (table[i].size() == 0) {
				cout << "null" << endl;
			} else{
				table[i].output(os);
			}
			i++;
		}
	}
};

int main() {
	hashChains<int,int> a(11);
	clog << "测试1： " << endl;
	a.insert({ 1,1 });
	a.insert({ 2,2 });
	a.output(clog);
	a.erase(1);
	clog <<endl<< "测试2： " << endl;
	a.output(clog);

	clog << endl << "测试3： " << endl;
	for (int i = 0; i < 100; i++) {
		a.insert({ i ,i});
	}
	a.output(clog);
}