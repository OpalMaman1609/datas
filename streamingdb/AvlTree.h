#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include "wet1util.h"
#include "Pair.h"


template<class Key, class Value>
class AvlNode {
public:
    int m_height{};
    Value m_value;
    Key m_key;
    AvlNode *m_parent;
    AvlNode *m_right_son;
    AvlNode *m_left_son;

    AvlNode(Key key, Value value): m_height(0),m_value(value),m_key(key),
    m_parent(nullptr),m_right_son(nullptr),m_left_son(nullptr) {}

    AvlNode(AvlNode<Key, Value> &other) {
        this->m_height = other.m_height;
        this->m_value = other.m_value;
        this->m_key = other.m_key;
        this->m_parent = other.m_parent;
        this->m_right_son = other.m_right_son;
        this->m_left_son = other.m_left_son;
    }

    ~AvlNode() = default;
};


template<class Key, class Value>
class AvlTree {
public:
    AvlNode<Key, Value> *m_root;
    int m_size;

public:
    AvlTree();

    ~AvlTree();

    AvlTree(const AvlTree<Key, Value> &other);

    AvlNode<Key, Value> *FindByKey(Key key);

    AvlNode<Key, Value> *LeftLeft(AvlNode<Key, Value> *node);
        
    AvlNode<Key, Value> *LeftRight(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *RightRight(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *RightLeft(AvlNode<Key, Value> *node);

    StatusType InsertAux(AvlNode<Key, Value> *root,AvlNode<Key, Value> *toInsert, AvlNode<Key, Value> *parent);

    StatusType Insert(Key key, Value value);

    AvlNode<Key, Value> *Rotate(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *DeleteNode(AvlNode<Key, Value> *node, AvlNode<Key, Value> *parentNode);

    int CalcBalanceFactor(AvlNode<Key, Value> *node) const;

    void UpdateBalance(AvlNode<Key, Value> *node);

    StatusType Delete(Key key);

    void DeleteTree(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *NextInOrder(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *FindMax();

    void ReverseInOrder(Key arr[]);
};


template<class Key, class Value>
AvlTree<Key, Value>::AvlTree() : m_root(nullptr), m_size(0) {}


template<class Key, class Value>
AvlTree<Key, Value>::~AvlTree() {
    if (m_root != nullptr) {
        DeleteTree(m_root);
    }
}

template<class Key, class Value>
AvlTree<Key, Value>::AvlTree(const AvlTree<Key, Value> &other) {
    m_size = other.m_size;
    m_root = other.m_root;
}

template<class Key, class Value>
StatusType
AvlTree<Key, Value>::InsertAux(AvlNode<Key, Value> *root, AvlNode<Key, Value> *insert, AvlNode<Key, Value> *parent) {
    if (!root) {
        root = insert;
        root->m_parent = parent;
        if (parent) {
            if (parent->m_key > insert->m_key) {
                parent->m_left_son = root;
            } else parent->m_right_son = root;
        }
        m_size++;
    } else if (root->m_key == insert->m_key) {
        return StatusType::FAILURE;
    } 
    else if (root->m_key > insert->m_key) {
        InsertAux(root->m_left_son, insert, root );
    } 
    else if (root->m_key < insert->m_key) {
        InsertAux(root->m_right_son,insert, root);
    }
    int rHeight = 0, lHeight = 0;
    if (parent) {
        if (parent->m_left_son || parent->m_right_son)
            parent->m_height = 1 + std::max(rHeight, lHeight);
        if (parent->m_left_son)
            lHeight = parent->m_left_son->m_height;
        if (parent->m_right_son)
            rHeight = parent->m_right_son->m_height;
        
        else
            parent->m_height = 0;
        this->Rotate(parent);
    }
    return StatusType::SUCCESS;
}

template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::FindByKey(Key key) {
    return findNode(m_root, key);
}

template<class Key, class Value>
AvlNode<Key, Value> *findNode(AvlNode<Key, Value> *rootNode, Key key) {
    if (!rootNode) {
        return nullptr;
    } else if (rootNode->m_key == key) {
        return rootNode;
    } else if (rootNode->m_key > key)
        return findNode(rootNode->m_left_son, key);
    else {
        return findNode(rootNode->m_right_son, key);
    }
}


template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::LeftRight(AvlNode<Key, Value> *node) {
    node->m_left_son = RightRight(node->m_left_son);
    return LeftLeft(node);
}




template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::LeftLeft(AvlNode<Key, Value> *node) {
    AvlNode<Key, Value> *aGroup;
    AvlNode<Key, Value> *bGroup;
    aGroup = node;
    bGroup = aGroup->m_left_son;
    aGroup->m_left_son = bGroup->m_right_son;
    if (bGroup->m_right_son) {
        bGroup->m_right_son->m_parent = aGroup;
    }
    AvlNode<Key, Value> *aGroupParent = aGroup->m_parent;
    if (aGroupParent) {
        if (aGroupParent->m_right_son == aGroup)
            aGroupParent->m_right_son = bGroup;
        else if (aGroupParent->m_left_son == aGroup)
            aGroupParent->m_left_son = bGroup;
    }
    bGroup->m_parent = aGroupParent;
    bGroup->m_right_son = aGroup;
    aGroup->m_parent = bGroup;
    updateHeight(aGroup);
    updateHeight(bGroup);
    if (m_root == node) {
        m_root = bGroup;
    }
    return bGroup;
}

template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::RightRight(AvlNode<Key, Value> *node) {
    AvlNode<Key, Value> *aGroup;
    AvlNode<Key, Value> *bGroup;
    aGroup = node;
    bGroup = aGroup->m_right_son;
    aGroup->m_right_son = bGroup->m_left_son;
    if (bGroup->m_left_son) {
        bGroup->m_left_son->m_parent = aGroup;
    }
    AvlNode<Key, Value> *aGroupParent = aGroup->m_parent;
    if (aGroupParent) {
        if (aGroupParent->m_right_son == aGroup)
            aGroupParent->m_right_son = bGroup;
        else if (aGroupParent->m_left_son == aGroup)
            aGroupParent->m_left_son = bGroup;
    }
    bGroup->m_parent = aGroupParent;
    bGroup->m_left_son = aGroup;
    aGroup->m_parent = bGroup;
    updateHeight(aGroup);
    updateHeight(bGroup);
    if (m_root == node) {
        m_root = bGroup;
    }
    return bGroup;
}



template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::RightLeft(AvlNode<Key, Value> *node) {
    node->m_right_son = LeftLeft(node->m_right_son);
    return RightRight(node);
}





template<class Key, class Value>
int AvlTree<Key, Value>::CalcBalanceFactor(AvlNode<Key, Value> *node) const {
    if (node == nullptr) {
        return 0;
    }
    int Rheight = -1;
    int Lheight = -1;
    if (node->m_right_son != nullptr) {
        Rheight = node->m_right_son->m_height;
    }
    if (node->m_left_son != nullptr) {
        Lheight = node->m_left_son->m_height;
    }
    return Lheight - Rheight;
}



template<class Key, class Value>
void AvlTree<Key, Value>::UpdateBalance(AvlNode<Key, Value> *node) {
    if (!node)
        return;
    int heightRight = 0, heightLeft = 0;
    if (node->m_left_son || node->m_right_son){
        node->m_height = 1 + std::max(heightRight, heightLeft);
        }
    if (node->m_left_son){
        heightLeft = node->m_left_son->m_height;
    }
    if (node->m_right_son){
        heightRight = node->m_right_son->m_height;
    }
    else {
        node->m_height = 0;
        this->Rotate(node);
        UpdateBalance(node->m_parent);
    }
}


template<class Key, class Value>
StatusType AvlTree<Key, Value>::Delete(Key key) {
    AvlNode<Key, Value> *node = FindByKey(key);
    if (!node)
        return StatusType::FAILURE;
    AvlNode<Key, Value> *newNode = DeleteNode(node, node->m_parent);
    UpdateBalance(newNode);
    return StatusType::SUCCESS;
}



template<class Key, class Value>
void AvlTree<Key, Value>::DeleteTree(AvlNode<Key, Value> *node) {
    if (node == nullptr) {
        return;
    }
    DeleteTree(node->m_right_son);
    DeleteTree(node->m_left_son);
    delete node;
}


// Preforms the rotation needed
template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::Rotate(AvlNode<Key, Value> *node) {
    int bf = CalcBalanceFactor(node);
    if (bf >= 2) {
        if (CalcBalanceFactor(node->m_left_son) >= 0) {
            return LeftLeft(node);
        } else {
            return LeftRight(node);
        }
    } else if (bf <= -2) {
        if (CalcBalanceFactor(node->m_right_son) > 0) {
            return RightLeft(node);
        } else {
            return RightRight(node);
        }
    } else return node;
}

template<class Key, class Value>
AvlNode<Key, Value> *findMinNode(AvlNode<Key, Value> *root) {
    AvlNode<Key, Value> *node = root;
    while (node->m_left_son != nullptr) {
        node = node->m_left_son;
    }
    return node;
}

template<class Key, class Value>
void updateHeight(AvlNode<Key, Value> *node) {
    int Rheight = 0;
    int Lheight = 0;
    if (node->m_right_son != nullptr) {
        Rheight = node->m_right_son->m_height;
    }
    if (node->m_left_son != nullptr) {
        Lheight = node->m_left_son->m_height;
    }
    if (!node->m_left_son && !node->m_right_son) {
        node->m_height = 0;
    } else {
        node->m_height = 1 + (std::max(Rheight, Lheight));
    }
}





template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::NextInOrder(AvlNode<Key, Value> *node) {
    if (!node) {
        return nullptr;
    }
    if (node->m_right_son) {
        return findMinNode(node->m_right_son);
    }
    AvlNode<Key, Value> *parent = node->m_parent;
    while (parent != nullptr && node == parent->m_right_son) {
        node = parent;
        parent = parent->m_parent;
    }
    return parent;
}

template<class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::FindMax() {
    return findMaxNode(m_root);
}

template<class Key, class Value>
AvlNode<Key, Value> *findMaxNode(AvlNode<Key, Value> *root) {
    AvlNode<Key, Value> *node = root;
    while (node->m_right_son != nullptr) {
        node = node->m_right_son;
    }
    return node;
}




template<class Key, class Value>
void ReverseInOrderUtil(AvlNode<Key, Value>* root, Key arr[], int * index) {
    if (!root) {
        return;
    }

    ReverseInOrderUtil(root->m_right_son, arr, index);
    arr[*index] = root->m_key;
//    arr[*index].m_value = root->m_value;
    *index = *index + 1;
    ReverseInOrderUtil(root->m_left_son, arr, index);
}

template<class Key, class Value>
void AvlTree<Key, Value>::ReverseInOrder(Key arr[]) {
    int i = 0;
    ReverseInOrderUtil(m_root, arr, &i);
}


template<class Key, class Value>
AvlNode<Key, Value> *AvlTree<Key, Value>::DeleteNode(AvlNode<Key, Value> *node, AvlNode<Key, Value> *parentNode) {
    if (!parentNode->m_left_son && !node->m_right_son) {
        if (parentNode) {
            if (parentNode->m_left_son == node) {
                parentNode->m_left_son = nullptr;
            } else {
                parentNode->m_right_son = nullptr;
            }
        }
        if(node == m_root)
            m_root = nullptr;
        delete node;
        m_size--;
        return parentNode;
    }
    else if (node->m_left_son && !node->m_right_son) {
        if (parentNode) {
            if (parentNode->m_left_son == node) {
                parentNode->m_left_son = node->m_left_son;
            } else {
                parentNode->m_right_son = node->m_left_son;
            }
        }
        node->m_left_son->m_parent = parentNode;
        if(node == m_root)
            m_root = node->m_left_son;
        delete node;
        m_size--;
        return parentNode;
    }
    else if (!node->m_left_son && node->m_right_son) {
        if (parentNode) {
            if (parentNode->m_left_son == node) {
                parentNode->m_left_son = node->m_right_son;
            } else {
                parentNode->m_right_son = node->m_right_son;
            }
        }
        node->m_right_son->m_parent = parentNode;
        if(node == m_root)
            m_root = node->m_right_son;
        delete node;
        m_size--;
        return parentNode;
    }
    else {
        AvlNode<Key, Value> *maxNode = findMaxNode(node->m_left_son);
        AvlNode<Key, Value> *maxParentNode = maxNode->m_parent;
        if (node->m_left_son == maxNode) {
            node->m_right_son->m_parent = maxNode;
            maxNode->m_right_son = node->m_right_son;
            maxNode->m_parent = parentNode;
            if (parentNode) {
                if (parentNode->m_left_son == node) {
                    parentNode->m_left_son = maxNode;
                } else {
                    parentNode->m_right_son = maxNode;
                }
            }
            if(node == m_root)
                m_root = maxNode;
            delete node;
            m_size--;
            return maxNode;
        } else {
            node->m_right_son->m_parent = maxNode;
            node->m_left_son->m_parent = maxNode;
            if (maxNode->m_left_son) {
                if (maxParentNode->m_left_son == maxNode) {
                    maxParentNode->m_left_son = maxNode->m_left_son;
                } else {
                    maxParentNode->m_right_son = maxNode->m_left_son;
                }
                maxNode->m_left_son->m_parent = maxParentNode;
            } else {
                if (maxParentNode->m_left_son == maxNode) {
                    maxParentNode->m_left_son = nullptr;
                } else {
                    maxParentNode->m_right_son = nullptr;
                }
            }
            maxNode->m_parent = parentNode;
            maxNode->m_right_son = node->m_right_son;
            maxNode->m_left_son = node->m_left_son;
            if (parentNode) {
                if (parentNode->m_left_son == node) {
                    parentNode->m_left_son = maxNode;
                } else {
                    parentNode->m_right_son = maxNode;
                }
            }
            if(node == m_root)
                m_root = maxNode;
            delete node;
            m_size--;
            return maxParentNode;
        }
    }
}



template<class Key, class Value>
StatusType AvlTree<Key, Value>::Insert(Key key, Value value) {
    AvlNode<Key, Value> *node;
    try {
        node = new AvlNode<Key, Value>(key, value);
    } catch (std::bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (!m_root) {
        m_root = node;
        m_size++;
        return StatusType::SUCCESS;
    } else {
        return InsertAux(m_root, node, nullptr);
    }
}

#endif //AVLTREE_H