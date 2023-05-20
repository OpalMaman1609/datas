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

    StatusType InsertNode(AvlNode<Key, Value> *root, AvlNode<Key, Value> *parent, AvlNode<Key, Value> *toInsert);

    StatusType Insert(Key key, Value value);

    AvlNode<Key, Value> *Rotate(AvlNode<Key, Value> *node);

    AvlNode<Key, Value> *DeleteNode(AvlNode<Key, Value> *node, AvlNode<Key, Value> *nodeParent);

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
AvlTree<Key, Value>::InsertNode(AvlNode<Key, Value> *rootNode, AvlNode<Key, Value> *newParent,
                                AvlNode<Key, Value> *toInsert) {
    if (!rootNode) {
        rootNode = toInsert;
        rootNode->m_parent = newParent;
        if (newParent) {
            if (newParent->m_key > toInsert->m_key) {
                newParent->m_left_son = rootNode;
            } else newParent->m_right_son = rootNode;
        }
        m_size++;
    } else if (rootNode->m_key == toInsert->m_key) {
        return StatusType::FAILURE;
    } 
    else if (rootNode->m_key > toInsert->m_key) {
        InsertNode(rootNode->m_left_son, rootNode, toInsert);
    } 
    else if (rootNode->m_key < toInsert->m_key) {
        InsertNode(rootNode->m_right_son, rootNode, toInsert);
    }
    int rHeight = 0;
    int lHeight = 0;
     if (newParent) {

        if (newParent->m_right_son)
            rHeight = newParent->m_right_son->m_height;
        if (newParent->m_left_son)
            lHeight = newParent->m_left_son->m_height;
        if (newParent->m_left_son || newParent->m_right_son)
            newParent->m_height = 1 + std::max(rHeight, lHeight);
        else
            newParent->m_height = 0;
        this->Rotate(newParent);
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
    int rHeight = 0, lHeight = 0;
    if (node->m_right_son)
        rHeight = node->m_right_son->m_height;
    if (node->m_left_son)
        lHeight = node->m_left_son->m_height;
    if (node->m_left_son || node->m_right_son)
        node->m_height = 1 + std::max(rHeight, lHeight);
    else
        node->m_height = 0;
    this->Rotate(node);
    UpdateBalance(node->m_parent);
}


template<class Key, class Value>
StatusType AvlTree<Key, Value>::Delete(Key key) {
    AvlNode<Key, Value> *node = FindByKey(key);
    if (!node)
        return StatusType::FAILURE;
    AvlNode<Key, Value> *new_node = DeleteNode(node, node->m_parent);
    UpdateBalance(new_node);
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
AvlNode<Key, Value> *AvlTree<Key, Value>::DeleteNode(AvlNode<Key, Value> *node, AvlNode<Key, Value> *nodeParent) {
    //if node is a leaf
    if (!node->m_left_son && !node->m_right_son) {
        if (nodeParent) {
            if (nodeParent->m_left_son == node) {
                nodeParent->m_left_son = nullptr;
            } else {
                nodeParent->m_right_son = nullptr;
            }
        }
        if(node == m_root)
            m_root = nullptr;
        delete node;
        m_size--;
        return nodeParent;
    }
        // if node has only left son
    else if (node->m_left_son && !node->m_right_son) {
        if (nodeParent) {
            if (nodeParent->m_left_son == node) {
                nodeParent->m_left_son = node->m_left_son;
            } else {
                nodeParent->m_right_son = node->m_left_son;
            }
        }
        node->m_left_son->m_parent = nodeParent;
        if(node == m_root)
            m_root = node->m_left_son;
        delete node;
        m_size--;
        return nodeParent;
    }
        // if node has only right son
    else if (!node->m_left_son && node->m_right_son) {
        if (nodeParent) {
            if (nodeParent->m_left_son == node) {
                nodeParent->m_left_son = node->m_right_son;
            } else {
                nodeParent->m_right_son = node->m_right_son;
            }
        }
        node->m_right_son->m_parent = nodeParent;
        if(node == m_root)
            m_root = node->m_right_son;
        delete node;
        m_size--;
        return nodeParent;
    }
        // if node has both sons
    else {
        AvlNode<Key, Value> *maxNode = findMaxNode(node->m_left_son);
        AvlNode<Key, Value> *maxNodeParent = maxNode->m_parent;
        //if maxNode is a direct son of node
        if (node->m_left_son == maxNode) {
            node->m_right_son->m_parent = maxNode;
            maxNode->m_right_son = node->m_right_son;
            maxNode->m_parent = nodeParent;
            if (nodeParent) {
                if (nodeParent->m_left_son == node) {
                    nodeParent->m_left_son = maxNode;
                } else {
                    nodeParent->m_right_son = maxNode;
                }
            }
            if(node == m_root)
                m_root = maxNode;
            delete node;
            m_size--;
            return maxNode;
            //if maxNode isn't a direct son of node
        } else {
            node->m_right_son->m_parent = maxNode;
            node->m_left_son->m_parent = maxNode;
            //if maxNode has a left son
            if (maxNode->m_left_son) {
                if (maxNodeParent->m_left_son == maxNode) {
                    maxNodeParent->m_left_son = maxNode->m_left_son;
                } else {
                    maxNodeParent->m_right_son = maxNode->m_left_son;
                }
                maxNode->m_left_son->m_parent = maxNodeParent;
                //if maxNode is a leaf
            } else {
                if (maxNodeParent->m_left_son == maxNode) {
                    maxNodeParent->m_left_son = nullptr;
                } else {
                    maxNodeParent->m_right_son = nullptr;
                }
            }
            maxNode->m_parent = nodeParent;
            maxNode->m_right_son = node->m_right_son;
            maxNode->m_left_son = node->m_left_son;
            if (nodeParent) {
                if (nodeParent->m_left_son == node) {
                    nodeParent->m_left_son = maxNode;
                } else {
                    nodeParent->m_right_son = maxNode;
                }
            }
            if(node == m_root)
                m_root = maxNode;
            delete node;
            m_size--;
            return maxNodeParent;
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
        return InsertNode(m_root, nullptr, node);
    }
}

#endif //AVLTREE_H