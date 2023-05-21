#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include "wet1util.h"
#include "Pair.h"


template<class Key, class Value, bool SHOULD_FREE>
class AvlNode {
public:
    int m_height{};
    Value m_value;
    Key m_key;
    bool m_should_free;
    AvlNode *m_parent;
    AvlNode *m_right_son;
    AvlNode *m_left_son;

    AvlNode(Key key, Value value): m_height(0),m_value(value),m_key(key),
    m_parent(nullptr),m_right_son(nullptr),m_left_son(nullptr), m_should_free(SHOULD_FREE) {}

    AvlNode(AvlNode<Key, Value, SHOULD_FREE> &other) {
        this->m_height = other.m_height;
        this->m_value = other.m_value;
        this->m_key = other.m_key;
        this->m_parent = other.m_parent;
        this->m_right_son = other.m_right_son;
        this->m_left_son = other.m_left_son;
    }

    ~AvlNode() = default;
};

template<class Key, class Value, bool SHOULD_FREE>
class AvlNode<Key, Value*, SHOULD_FREE> {
public:
    int m_height{};
    Value* m_value;
    Key m_key;
    bool m_should_free;
    AvlNode *m_parent;
    AvlNode *m_right_son;
    AvlNode *m_left_son;

    AvlNode(Key key, Value* value): m_height(0),m_value(value),m_key(key),
    m_parent(nullptr),m_right_son(nullptr),m_left_son(nullptr), m_should_free(SHOULD_FREE) {}

    AvlNode(AvlNode<Key, Value*, SHOULD_FREE> &other) {
        this->m_height = other.m_height;
        this->m_value = other.m_value;
        this->m_key = other.m_key;
        this->m_parent = other.m_parent;
        this->m_right_son = other.m_right_son;
        this->m_left_son = other.m_left_son;
    }

    ~AvlNode() {
        if (m_should_free) {
            delete m_value;
        }
    };
};


template<class Key, class Value, bool SHOULD_FREE>
class AvlTree {
public:
    AvlNode<Key, Value, SHOULD_FREE> *m_root;
    int m_size;

public:
    AvlTree();

    ~AvlTree();

    AvlTree(const AvlTree<Key, Value, SHOULD_FREE> &other);

    AvlNode<Key, Value, SHOULD_FREE> *FindByKey(Key key);

    AvlNode<Key, Value, SHOULD_FREE> *LeftLeft(AvlNode<Key, Value, SHOULD_FREE> *node);
        
    AvlNode<Key, Value, SHOULD_FREE> *LeftRight(AvlNode<Key, Value, SHOULD_FREE> *node);

    AvlNode<Key, Value, SHOULD_FREE> *RightRight(AvlNode<Key, Value, SHOULD_FREE> *node);

    AvlNode<Key, Value, SHOULD_FREE> *RightLeft(AvlNode<Key, Value, SHOULD_FREE> *node);

    StatusType InsertNode(AvlNode<Key, Value, SHOULD_FREE> *root, AvlNode<Key, Value, SHOULD_FREE> *parent, AvlNode<Key, Value, SHOULD_FREE> *toInsert);

    StatusType Insert(Key key, Value value);

    AvlNode<Key, Value, SHOULD_FREE> *Rotate(AvlNode<Key, Value, SHOULD_FREE> *node);

    AvlNode<Key, Value, SHOULD_FREE> *DeleteNode(AvlNode<Key, Value, SHOULD_FREE> *node, AvlNode<Key, Value, SHOULD_FREE> *nodeParent);

    int CalcBalanceFactor(AvlNode<Key, Value, SHOULD_FREE> *node) const;

    void UpdateBalance(AvlNode<Key, Value, SHOULD_FREE> *node);

    StatusType Delete(Key key);

    void DeleteTree(AvlNode<Key, Value, SHOULD_FREE> *node);

    AvlNode<Key, Value, SHOULD_FREE> *NextInOrder(AvlNode<Key, Value, SHOULD_FREE> *node);

    AvlNode<Key, Value, SHOULD_FREE> *FindMax();

    void ReverseInOrder(Key arr[]);
};


template<class Key, class Value, bool SHOULD_FREE>
AvlTree<Key, Value, SHOULD_FREE>::AvlTree() : m_root(nullptr), m_size(0) {}


template<class Key, class Value, bool SHOULD_FREE>
AvlTree<Key, Value, SHOULD_FREE>::~AvlTree() {
    if (m_root != nullptr) {
        DeleteTree(m_root);
    }
}

template<class Key, class Value, bool SHOULD_FREE>
AvlTree<Key, Value, SHOULD_FREE>::AvlTree(const AvlTree<Key, Value, SHOULD_FREE> &other) {
    m_size = other.m_size;
    m_root = other.m_root;
}

template<class Key, class Value, bool SHOULD_FREE>
StatusType
AvlTree<Key, Value, SHOULD_FREE>::InsertNode(AvlNode<Key, Value, SHOULD_FREE> *rootNode, AvlNode<Key, Value, SHOULD_FREE> *newParent,
                                AvlNode<Key, Value, SHOULD_FREE> *toInsert) {
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

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::FindByKey(Key key) {
    return findNode(m_root, key);
}

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *findNode(AvlNode<Key, Value, SHOULD_FREE> *rootNode, Key key) {
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


template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::LeftRight(AvlNode<Key, Value, SHOULD_FREE> *node) {
    node->m_left_son = RightRight(node->m_left_son);
    return LeftLeft(node);
}




template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::LeftLeft(AvlNode<Key, Value, SHOULD_FREE> *node) {
    AvlNode<Key, Value, SHOULD_FREE> *aGroup;
    AvlNode<Key, Value, SHOULD_FREE> *bGroup;
    aGroup = node;
    bGroup = aGroup->m_left_son;
    aGroup->m_left_son = bGroup->m_right_son;
    if (bGroup->m_right_son) {
        bGroup->m_right_son->m_parent = aGroup;
    }
    AvlNode<Key, Value, SHOULD_FREE> *aGroupParent = aGroup->m_parent;
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

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::RightRight(AvlNode<Key, Value, SHOULD_FREE> *node) {
    AvlNode<Key, Value, SHOULD_FREE> *aGroup;
    AvlNode<Key, Value, SHOULD_FREE> *bGroup;
    aGroup = node;
    bGroup = aGroup->m_right_son;
    aGroup->m_right_son = bGroup->m_left_son;
    if (bGroup->m_left_son) {
        bGroup->m_left_son->m_parent = aGroup;
    }
    AvlNode<Key, Value, SHOULD_FREE> *aGroupParent = aGroup->m_parent;
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



template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::RightLeft(AvlNode<Key, Value, SHOULD_FREE> *node) {
    node->m_right_son = LeftLeft(node->m_right_son);
    return RightRight(node);
}





template<class Key, class Value, bool SHOULD_FREE>
int AvlTree<Key, Value, SHOULD_FREE>::CalcBalanceFactor(AvlNode<Key, Value, SHOULD_FREE> *node) const {
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



template<class Key, class Value, bool SHOULD_FREE>
void AvlTree<Key, Value, SHOULD_FREE>::UpdateBalance(AvlNode<Key, Value, SHOULD_FREE> *node) {
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


template<class Key, class Value, bool SHOULD_FREE>
StatusType AvlTree<Key, Value, SHOULD_FREE>::Delete(Key key) {
    AvlNode<Key, Value, SHOULD_FREE> *node = FindByKey(key);
    if (!node)
        return StatusType::FAILURE;
    AvlNode<Key, Value, SHOULD_FREE> *new_node = DeleteNode(node, node->m_parent);
    UpdateBalance(new_node);
    return StatusType::SUCCESS;
}



template<class Key, class Value, bool SHOULD_FREE>
void AvlTree<Key, Value, SHOULD_FREE>::DeleteTree(AvlNode<Key, Value, SHOULD_FREE> *node) {
    if (node == nullptr) {
        return;
    }
    DeleteTree(node->m_right_son);
    DeleteTree(node->m_left_son);
    delete node;
}


// Preforms the rotation needed
template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::Rotate(AvlNode<Key, Value, SHOULD_FREE> *node) {
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

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *findMinNode(AvlNode<Key, Value, SHOULD_FREE> *root) {
    AvlNode<Key, Value, SHOULD_FREE> *node = root;
    while (node->m_left_son != nullptr) {
        node = node->m_left_son;
    }
    return node;
}

template<class Key, class Value, bool SHOULD_FREE>
void updateHeight(AvlNode<Key, Value, SHOULD_FREE> *node) {
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





template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::NextInOrder(AvlNode<Key, Value, SHOULD_FREE> *node) {
    if (!node) {
        return nullptr;
    }
    if (node->m_right_son) {
        return findMinNode(node->m_right_son);
    }
    AvlNode<Key, Value, SHOULD_FREE> *parent = node->m_parent;
    while (parent != nullptr && node == parent->m_right_son) {
        node = parent;
        parent = parent->m_parent;
    }
    return parent;
}

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE>* AvlTree<Key, Value, SHOULD_FREE>::FindMax() {
    return findMaxNode(m_root);
}

template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *findMaxNode(AvlNode<Key, Value, SHOULD_FREE> *root) {
    AvlNode<Key, Value, SHOULD_FREE> *node = root;
    while (node->m_right_son != nullptr) {
        node = node->m_right_son;
    }
    return node;
}




template<class Key, class Value, bool SHOULD_FREE>
void ReverseInOrderUtil(AvlNode<Key, Value, SHOULD_FREE>* root, Key arr[], int * index) {
    if (!root) {
        return;
    }

    ReverseInOrderUtil(root->m_right_son, arr, index);
    arr[*index] = root->m_key;
//    arr[*index].m_value = root->m_value;
    *index = *index + 1;
    ReverseInOrderUtil(root->m_left_son, arr, index);
}

template<class Key, class Value, bool SHOULD_FREE>
void AvlTree<Key, Value, SHOULD_FREE>::ReverseInOrder(Key arr[]) {
    int i = 0;
    ReverseInOrderUtil(m_root, arr, &i);
}


template<class Key, class Value, bool SHOULD_FREE>
AvlNode<Key, Value, SHOULD_FREE> *AvlTree<Key, Value, SHOULD_FREE>::DeleteNode(AvlNode<Key, Value, SHOULD_FREE> *node, AvlNode<Key, Value, SHOULD_FREE> *nodeParent) {
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
        AvlNode<Key, Value, SHOULD_FREE> *maxNode = findMaxNode(node->m_left_son);
        AvlNode<Key, Value, SHOULD_FREE> *maxNodeParent = maxNode->m_parent;
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



template<class Key, class Value, bool SHOULD_FREE>
StatusType AvlTree<Key, Value, SHOULD_FREE>::Insert(Key key, Value value) {
    AvlNode<Key, Value, SHOULD_FREE> *node;
    try {
        node = new AvlNode<Key, Value, SHOULD_FREE>(key, value);
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