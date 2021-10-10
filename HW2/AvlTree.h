#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>

using namespace std;

/* AVL Tree implementation was copied and slightly modified from 5-Trees3.ppt */

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
    Comparable element;
    AvlNode* left;
    AvlNode* right;
    int        		height;

    AvlNode(const Comparable& theElement,
        AvlNode* lt, AvlNode* rt, int h = 0)
        : element(theElement), left(lt), right(rt), height(h) { }

    friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree
{
public:
    explicit AvlTree();
    explicit AvlTree(const Comparable& notFound);
    AvlTree(const AvlTree& rhs);
    ~AvlTree();

    const Comparable& findMin() const;
    const Comparable& findMax() const;
    const Comparable& find(const Comparable& x) const;
    void findAndUpdate(const Comparable& toBeFound, const Comparable& updated);
    bool isEmpty() const;
    void printTree() const;

    void makeEmpty();
    void insert(const Comparable& x);
    void remove(const Comparable& x);

    const AvlTree& operator=(const AvlTree& rhs);

private:
    AvlNode<Comparable>* root;
    const Comparable ITEM_NOT_FOUND;

    const Comparable& elementAt(AvlNode<Comparable>* t) const;

    void insert(const Comparable& x, AvlNode<Comparable>*& t) const;
    void remove(const Comparable& x, AvlNode<Comparable>*& t) const;

    AvlNode<Comparable>* findMin(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* findMax(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* find(const Comparable& x, AvlNode<Comparable>* t) const;
    void makeEmpty(AvlNode<Comparable>*& t) const;
    void printTree(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* clone(AvlNode<Comparable>* t) const;

    // Avl manipulations
    int height(AvlNode<Comparable>* t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<Comparable>*& k2) const;
    void rotateWithRightChild(AvlNode<Comparable>*& k1) const;
    void doubleWithLeftChild(AvlNode<Comparable>*& k3) const;
    void doubleWithRightChild(AvlNode<Comparable>*& k1) const;
};

#include "AvlTree.cpp"  
#endif