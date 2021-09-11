#include<iostream>
#include"linkedBinaryTree.h"
using namespace std;

//基类
template <class K, class E>
class bsTree {
public:
    virtual ~bsTree() {}

    //字典为空时返回true
    virtual bool empty() const = 0;

    //返回元素个数
    virtual int size() const = 0;

    //返回匹配数对的指针
    virtual std::pair<K, E>* find(const K&) const = 0;

    //删除匹配的数对
    virtual void erase(const K&) = 0;

    //在字典中插入一个数对
    virtual void insert(const std::pair<K, E>&) = 0;

};

//派生
template<class K,class E>
class binarySearchTree :public linkedBinaryTree<pair<K, E>> ,public bsTree<K, E>{
    //显式地引入基类的成员
    using linkedBinaryTree<pair<K, E>>::treeSize;
    using linkedBinaryTree<pair<K, E>>::root;

public:

    typedef std::pair<K, E> pairType;

    bool empty() const { return treeSize==0; }

    int size()const { return treeSize; }

    std::pair<K, E>* find(const K& theKey)const {
        auto p = root;
        while (p != nullptr) {
            if (theKey == p->element.first) {
                return &p->element;
            }
            else if (theKey > p->element.first) {
                p = p->right;
            }
            else {
                p = p->left;
            }
        }return nullptr;
    }

    void erase(const K& theKey) {
        binaryTreeNode<pair<K, E>>* p=root, * pp=nullptr;
        
        while (p != nullptr && p->element.first != theKey) {
            pp = p;
            if (theKey < p->element.first) {
                p = p->left;
            }
            else {
                p = p->right;
            }
        }
        if (p == nullptr) {
            return;//没找到
        }

        if (p->left != nullptr && p->right != nullptr) {
            binaryTreeNode<pair<K, E>>* s = p->left,
                * ps = p;
            while (s->right != nullptr) {
                ps = s;
                s = s->right;
            }

            binaryTreeNode<pair<K, E>>* q = new  binaryTreeNode<pair<K, E>>(s->element,p->right,p->left);

            if (pp == nullptr) {
                root = q;
            }
            else if (p == pp->left) {
                pp->left = q;
            }
            else {
                pp->right = q;
            }

            if (ps == p) {
                pp = q;
            }
            else {
                pp = ps;
            }
            delete p;
            p = s;
            
        }


        //p最多有一个孩子
        binaryTreeNode<pair<K, E>>* c;
        if (p->left != nullptr) {
            c = p -> left;
        }
        else {
            c = p->right;
        }

        if (p == root) {
            root = c;
        }
        else {
            if (p == pp->left) {
                pp->left = c;
            }
            else {
                pp->right = c;
            }
        }treeSize--;
        delete p;
    }

    void insert(const pair<K, E>& thePair) {
        binaryTreeNode<pair<K, E>>* p, * pp;
        p = root;
        pp = p;
        while (p != nullptr) {
            pp = p;
            if (thePair.first > p->element.first) {
                p = p->right;
            }
            else {
                p = p->left;
            }
        }
        binaryTreeNode<pair<K, E>>* newNode = new binaryTreeNode<pair<K, E>>(thePair);
        if (root==nullptr) {
            root = newNode;
            treeSize++;
            return;
        }
        if (thePair.first < pp->element.first) {
            pp->left = newNode;
        }
        else {
            pp->right = newNode;
        }
        treeSize++;
        
    }

};
//
//int main() {
//    binarySearchTree<int, int> a;
//    
//    for (int i = 1; i < 100; i++) {
//        a.insert(pair<int, int>{i, i});
//    }
//    
//    for (int i = 1; i < 100; i+=2) {
//        a.erase(i);
//    }
//
//    a.inOrder([](binaryTreeNode<pair<int, int>>* p) {
//        if (p != nullptr)
//            cout << p->element.first << "  ";
//        });
//}