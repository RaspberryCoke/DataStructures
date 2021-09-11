#pragma once

#include<iostream>
using namespace std;
/*
当使用临时变量的指针，可以用pair<K,E>*
其他情况，统一使用pair<const K,E>*

字典由字典节点组成
字典节点由pair<> 和next指针组成。

构造函数不设置参数。
*/
//字典
template<class K, class E>
class dictionary {
public:
	virtual ~dictionary() {}
	virtual bool empty()const = 0;
	virtual int size()const = 0;
	virtual std::pair<K, E>* find(const K&)const = 0;
	virtual void erase(const K&) = 0;
	virtual void insert(const pair<K, E>&) = 0;
};
//字典节点
template<class K,class E>
struct pairNode {
	using pairType = pair<K, E>;
	pairType element;
	pairNode<K, E>* next;

	pairNode(const pairType node, pairNode<K, E>* n=nullptr) :element(node){
		next = n;
	}
};
//用链表描述的已排序字典
template<class K,class E>
class sortedChain :public dictionary<K, E> {
private:
	pairNode<K, E>* firstNode;
	int listSize;
public:
	sortedChain() {
		firstNode = nullptr;
		listSize=0;
	}
	bool empty()const override {
		return listSize == 0;
	}
	int size()const override {
		return listSize;
	}
	std::pair<K, E>* find(const K& key)const override {
		pairNode<K, E>* p = firstNode;
		while (p != nullptr && p->element.first != key) {
			p = p->next;
		}
		if (p == nullptr) {
			return nullptr;
		}
		else {
			return &p->element;
		}
	}
	void insert(const pair<K, E>& thePair)override {
		pairNode<K, E>* currentNode = firstNode;
		pairNode<K, E>* preNode = nullptr;

		while (currentNode != nullptr && thePair.first > currentNode->element.first) {
			preNode = currentNode;
			currentNode = currentNode->next;
		}

		if (currentNode != nullptr && currentNode->element.first == thePair.first) {
			currentNode->element.second = thePair.second;
			return;
		}

		pairNode<K, E>* newPair = new pairNode<K, E>(thePair, currentNode);

		if (preNode == nullptr) {
			firstNode = newPair;
		}
		else {
			preNode->next = newPair;
		}
		listSize++;
	}
	void erase(const K& theKey)override {
		if (listSize == 0) {
			clog << "无法erase一个空链表" << endl;
			throw runtime_error{ "无法erase一个空链表" };
		}
		pairNode<K, E>* currentNode = firstNode;
		pairNode<K, E>* preNode = nullptr;

		while (currentNode != nullptr && theKey > currentNode->element.first) {
			preNode = currentNode;
			currentNode = currentNode->next;
		}
		if (currentNode == nullptr || currentNode->element.first < theKey) {
			clog << "erase失败，没有这个key" << endl;
			return;
		}
		if (preNode == nullptr) {
			firstNode = firstNode->next;
			delete currentNode;
			listSize--;
			return;
		}
		preNode ->next= currentNode->next;
		delete currentNode;
		listSize--;
	}
	void output(ostream& os) const{
		pairNode<K,E>* p = firstNode;
		while (p != nullptr) {
			os << p->element.first << " ";
			p = p->next;
		}
		os << endl;
	}
};
//int main() {
//	sortedChain<int, int> chain{};//不能写chain()
//	chain.insert({ 1,1 });
//	chain.output(clog);
//	chain.erase(1);
//	chain.output(clog);
//}