#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* x){
        
        AVLNode<Key, Value>* p = x->getParent();
        AVLNode<Key, Value>* y = x->getRight();
        AVLNode<Key, Value>* B = y->getLeft();

        x->setRight(B);
        if(B != NULL){
            B->setParent(x);    
        }
        y->setParent(p);
        if(p == NULL){
            this->root_ = y;
        }else if(x == p->getLeft()){
            p->setLeft(y);
        }else{
            p->setRight(y);     
        }

        y->setLeft(x);
        x->setParent(y);
    }
    void rotateRight(AVLNode<Key, Value>* x){
        AVLNode<Key, Value>* p = x->getParent();
        AVLNode<Key, Value>* y = x->getLeft();
        AVLNode<Key, Value>* B = y->getRight();

        x->setLeft(B);
        if(B != NULL){
            B->setParent(x);    
        }
        y->setParent(p);
        if(p == NULL){
            this->root_ = y;
        }else if(x == p->getLeft()){
            p->setLeft(y);      
        } else{
            p->setRight(y);     
        }
        y->setRight(x);
        x->setParent(y);
    }





void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
    if(parent == NULL){
        return;
    }
    if(node == parent->getLeft()){
        parent->updateBalance(-1);
    }else{
        parent->updateBalance(1);
    }

    if(parent->getBalance() == 0){
        return;
    }
    if(abs(parent->getBalance()) == 2){
        if(parent->getBalance() == -2){
            if(node->getBalance() == -1){
                rotateRight(parent);
                parent->setBalance(0);
                node->setBalance(0);
            }else{
                AVLNode<Key, Value>* y = node->getRight();
                rotateLeft(node);
                rotateRight(parent);
                if(y->getBalance() == -1){
                    parent->setBalance(1);
                    node->setBalance(0);
                    y->setBalance(0);
                }else if(y->getBalance() == 0){
                    parent->setBalance(0);
                    node->setBalance(0);
                    y->setBalance(0);
                }else{
                    parent->setBalance(0);
                    node->setBalance(-1);
                    y->setBalance(0);
                }
            }
        }else{
            if(node->getBalance() == 1){
                rotateLeft(parent);
                parent->setBalance(0);
                node->setBalance(0);
            }else{
                AVLNode<Key, Value>* y = node->getLeft();
                rotateRight(node);
                rotateLeft(parent);
                if(y->getBalance() == 1){
                    parent->setBalance(-1);
                    node->setBalance(0);
                    y->setBalance(0);
                }else if(y->getBalance() == 0){
                    parent->setBalance(0);
                    node->setBalance(0);
                    y->setBalance(0);
                }else{
                    parent->setBalance(0);
                    node->setBalance(1);
                    y->setBalance(0);    
                }
            }
        }
        return;
    }
    insertFix(parent->getParent(), parent);
}

void removeFix(AVLNode<Key, Value>* parent, int diff){
    if(parent == NULL){
        return;
    }
    AVLNode<Key, Value>* gp = parent->getParent();
    int ndiff = 0;
    if(gp != NULL){
        if(parent == gp->getLeft()){
            ndiff = 1;
        }else{
            ndiff = -1;
        }
    }

    parent->updateBalance(diff);
    if(abs(parent->getBalance()) == 1){
        return;
    }
    if(parent->getBalance() == 0){
        removeFix(gp, ndiff);
        return;
    }

    if(parent->getBalance() == 2){
        AVLNode<Key, Value>* child = parent->getRight();
        if(child->getBalance() >= 0){
            rotateLeft(parent);
            if(child->getBalance() == 0){
                parent->setBalance(1);
                child->setBalance(-1);
                return;
            }else{
                parent->setBalance(0);
                child->setBalance(0);
            }
        }else{
            AVLNode<Key, Value>* gchild = child->getLeft();
            rotateRight(child);
            rotateLeft(parent);
            if(gchild->getBalance() == 1){
                parent->setBalance(-1);
                child->setBalance(0);
            }else if(gchild->getBalance() == 0){
                parent->setBalance(0);
                child->setBalance(0);
            }else{
                parent->setBalance(0);
                child->setBalance(1);
            }
            gchild->setBalance(0);
        }
    }else{
        AVLNode<Key, Value>* child = parent->getLeft();
        if(child->getBalance() <= 0){
            rotateRight(parent);
            if(child->getBalance() == 0){
                parent->setBalance(-1);
                child->setBalance(1);
                return;
            }else{
                parent->setBalance(0);
                child->setBalance(0);
            }
        }else{
            AVLNode<Key, Value>* gchild = child->getRight();
            rotateLeft(child);
            rotateRight(parent);
            if(gchild->getBalance() == -1){
                parent->setBalance(1);
                child->setBalance(0);
            }else if(gchild->getBalance() == 0){
                parent->setBalance(0);
                child->setBalance(0);
            }else{
                parent->setBalance(0);
                child->setBalance(-1);
            }
            gchild->setBalance(0);
        }
    }
    removeFix(gp, ndiff);
}
};
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;

    while(current!= NULL){
        if(new_item.first == current->getKey()){
            current->setValue(new_item.second);
            return;
        }else if(new_item.first < current->getKey()){
            parent = current;
            current = current->getLeft();
        }else{
            parent = current;
            current = current->getRight();  
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(new_item.first < parent->getKey()){
        parent->setLeft(newNode);
    }else{
        parent->setRight(newNode);
    }

    insertFix(parent, newNode);

}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(node == NULL){
        return;
    }

    if(node->getLeft() != NULL && node->getRight() != NULL){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
        nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int diff = 0;
    if(parent != NULL){
        if(node == parent->getLeft()){
            diff = 1;
        }else{
            diff = -1;
        }
    }

    AVLNode<Key, Value>* child = NULL;
    if(node->getLeft() != NULL){
        child = node->getLeft();
    }else{
        child = node->getRight();
    }
    if(parent != NULL){
        if(node == parent->getLeft()){
            parent->setLeft(child);
        }else{
            parent->setRight(child);
        }
    }else{
        this->root_ = child;    
    }

    if(child != NULL){
        child->setParent(parent);
    }
    delete node;
    removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
