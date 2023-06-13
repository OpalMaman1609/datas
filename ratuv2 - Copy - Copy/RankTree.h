#ifndef RATUV2_RankTree_H
#define RATUV2_RankTree_H

#include <iostream>
#include "utilesWet2.h"

template<class Key, class Value>
class RankNode {
public:
    Key m_key;
    Value m_value;
    RankNode *m_right_son;
    RankNode *m_left_son;
    RankNode *m_parent;
    int m_weight;
    int m_height;
    double extra;
    double privateExtra;

    RankNode(Key key, Value value)
            : m_key(key),
              m_value(value),
              m_right_son(nullptr),
              m_left_son(nullptr),
              m_parent(nullptr),
              m_weight(1),
              m_height(0),
              extra(0),
              privateExtra(0) {}

    RankNode(const RankNode<Key, Value> &other) {
        this->m_key = other.m_key;
        this->m_value = other.m_value;
        this->m_right_son = other.m_right_son;
        this->m_left_son = other.m_left_son;
        this->m_parent = other.m_parent;
        this->m_weight = other.m_weight;
        this->m_height = other.m_height;
        this->extra = other.extra;
        this->privateExtra = other.privateExtra;

    }

    ~RankNode() = default;
};


template<class Key, class Value>
class RankTree {
public:
    int size;
    RankNode<Key, Value> *root;

    RankTree();

    ~RankTree();

    RankTree(const RankTree<Key, Value> &other);

    RankNode<Key, Value> *FindByKey(Key key);

    RankNode<Key, Value> *LeftLeft(RankNode<Key, Value> *node);

    RankNode<Key, Value> *LeftRight(RankNode<Key, Value> *node);

    RankNode<Key, Value> *RightRight(RankNode<Key, Value> *node);

    RankNode<Key, Value> *RightLeft(RankNode<Key, Value> *node);

    StatusType insertNode(RankNode<Key, Value> *root, RankNode<Key, Value> *parent, RankNode<Key, Value> *toInsert);

    StatusType insert(Key key, Value value);

    RankNode<Key, Value> *Rotate(RankNode<Key, Value> *node);

    RankNode<Key, Value> *DeleteNode(RankNode<Key, Value> *node, RankNode<Key, Value> *nodeParent);

    int CalcBalanceFactor(RankNode<Key, Value> *node) const;

    void UpdateBalance(RankNode<Key, Value> *node);

    StatusType Delete(Key key);

    void DeleteTree(RankNode<Key, Value> *node);

    void addToExtra(Key key, double  amount);

    double sumExtra(Key key);

    RankNode<Key, Value> *findClosestSmallerKey(Key target);

    RankNode<Key, Value> *NextInOrder(RankNode<Key, Value> *node);

};






// Constructor
template<class Key, class Value>
RankTree<Key, Value>::RankTree() : size(0), root(nullptr) {}

// Destructor
template<class Key, class Value>
RankTree<Key, Value>::~RankTree() {
    if (root != nullptr) {
        DeleteTree(root);
    }
}

template<class Key, class Value>
RankTree<Key, Value>::RankTree(const RankTree<Key, Value> &other) {
    root = other.root;
    size = other.size;
}

template<class Key, class Value>
StatusType
RankTree<Key, Value>::insertNode(RankNode<Key, Value> *rootNode, RankNode<Key, Value> *newParent,
                                 RankNode<Key, Value> *toInsert) {
    if (!rootNode) {
        rootNode = toInsert;
        rootNode->m_parent = newParent;
        if (newParent) {
            if (newParent->m_key > toInsert->m_key) {
                newParent->m_left_son = rootNode;
            } else newParent->m_right_son = rootNode;
        }
        size++;
    } else if (rootNode->m_key == toInsert->m_key) {
        return StatusType::FAILURE;

    } else if (rootNode->m_key > toInsert->m_key) {
        insertNode(rootNode->m_left_son, rootNode, toInsert);
    } else if (rootNode->m_key < toInsert->m_key) {
        insertNode(rootNode->m_right_son, rootNode, toInsert);
    }
    int rHeight = 0, lHeight = 0;
    int weight = 1;
    if (newParent) {
        if (newParent->m_right_son) {
            rHeight = newParent->m_right_son->m_height;
            weight += newParent->m_right_son->m_weight;
        }
        if (newParent->m_left_son) {
            lHeight = newParent->m_left_son->m_height;
            weight += newParent->m_left_son->m_weight;
        }
        if (newParent->m_left_son || newParent->m_right_son)
            newParent->m_height = 1 + std::max(rHeight, lHeight);
        else
            newParent->m_height = 0;
        newParent->m_weight = weight;
        this->Rotate(newParent);
    }
    return StatusType::SUCCESS;
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::FindByKey(Key key) {
    return findNode(root, key);
}


template<class Key, class Value>
RankNode<Key, Value> *findNode(RankNode<Key, Value> *rootNode, Key key) {
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
RankNode<Key, Value> *RankTree<Key, Value>::LeftRight(RankNode<Key, Value> *node) {
    node->m_left_son = RightRight(node->m_left_son);
    return LeftLeft(node);
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::LeftLeft(RankNode<Key, Value> *node) {
    RankNode<Key, Value> *a;
    RankNode<Key, Value> *b;
    a = node;
    b = a->m_left_son;
    a->privateExtra += a->extra;
    if (a->m_right_son)
    {
        a->m_right_son->extra += a->extra;
    }
    double bSum = a->extra +b->extra;
    a->extra = 0;
    b->privateExtra += bSum;
    if (b->m_left_son)
    {
       b->m_left_son->extra += bSum;
    }
    if (b->m_right_son)
    {
        b->m_right_son->extra += bSum;
    }
    b->extra = 0;
    a->m_left_son = b->m_right_son;
    if (b->m_right_son) {
        b->m_right_son->m_parent = a;
    }
    RankNode<Key, Value> *a_parent = a->m_parent;
    if (a_parent) {
        if (a_parent->m_right_son == a)
            a_parent->m_right_son = b;
        else if (a_parent->m_left_son == a)
            a_parent->m_left_son = b;
    }
    b->m_parent = a_parent;
    b->m_right_son = a;
    a->m_parent = b;
    updateHeight(a);
    updateHeight(b);
    if (root == node) {
        root = b;
    }
    return b;
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::RightRight(RankNode<Key, Value> *node) {
    RankNode<Key, Value> *a;
    RankNode<Key, Value> *b;
    a = node;
    b = a->m_right_son;

    a->privateExtra += a->extra;
   if (a->m_left_son)
   {
        a->m_left_son->extra += a->extra; 
    }
    double bSum = a->extra +b->extra;
    a->extra = 0;
    b->privateExtra += bSum;
   if (b->m_left_son)
   {
        b->m_left_son->extra += bSum;
    }
   if (b->m_right_son)
   {
    b->m_right_son->extra += bSum;
    }
    b->extra = 0;

    a->m_right_son = b->m_left_son;
    if (b->m_left_son) {
        b->m_left_son->m_parent = a;
    }
    RankNode<Key, Value> *a_parent = a->m_parent;
    if (a_parent) {
        if (a_parent->m_right_son == a)
            a_parent->m_right_son = b;
        else if (a_parent->m_left_son == a)
            a_parent->m_left_son = b;
    }
    b->m_parent = a_parent;
    b->m_left_son = a;
    a->m_parent = b;
    updateHeight(a);
    updateHeight(b);
    if (root == node) {
        root = b;
    }
    return b;
}


template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::RightLeft(RankNode<Key, Value> *node) {
    node->m_right_son = LeftLeft(node->m_right_son);
    return RightRight(node);
}


template<class Key, class Value>
int RankTree<Key, Value>::CalcBalanceFactor(RankNode<Key, Value> *node) const {
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
void updateHeight(RankNode<Key, Value> *node) {
    int Rheight = 0;
    int Lheight = 0;
    int weight = 1;
    if (node->m_right_son != nullptr) {
        Rheight = node->m_right_son->m_height;
        weight += node->m_right_son->m_weight;
    }
    if (node->m_left_son != nullptr) {
        Lheight = node->m_left_son->m_height;
        weight += node->m_left_son->m_weight;
    }
    if (!node->m_left_son && !node->m_right_son) {
        node->m_height = 0;
    } else {
        node->m_height = 1 + (std::max(Rheight, Lheight));
    }
    node->m_weight = weight;
}


template<class Key, class Value>
void RankTree<Key, Value>::DeleteTree(RankNode<Key, Value> *node) {
    if (node == nullptr) {
        return;
    }
    DeleteTree(node->m_right_son);
    DeleteTree(node->m_left_son);
    delete node;
}


template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::Rotate(RankNode<Key, Value> *node) {
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
RankNode<Key, Value> *RankTree<Key, Value>::findClosestSmallerKey(Key target) {
    Key result = -1;
    RankNode<Key, Value> * curRoot = root;
    while (curRoot) {
        if (curRoot->m_key < target) {
            result = result > curRoot->m_key ? result : curRoot->m_key;
            curRoot = curRoot->m_right_son;
        } else {
            curRoot = curRoot->m_left_son;
        }
    }
    return FindByKey(result);
}

template<class Key, class Value>
StatusType RankTree<Key, Value>::insert(Key key, Value value) {
    RankNode<Key, Value> *node;
    try {
        node = new RankNode<Key, Value>(key, value);
    } catch (std::bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (!root) {
        root = node;
        size++;
        return StatusType::SUCCESS;
    } else {
        return insertNode(root, nullptr, node);
    }
}


template<class Key, class Value>
void RankTree<Key, Value>::addToExtra(Key key, double  amount){
    bool isRightSequnce = false;
    RankNode<Key, Value> * curRoot = root;
    while (curRoot != NULL) {
        if (key > curRoot->m_key){
            if (isRightSequnce == false){
                curRoot->extra += amount;
                isRightSequnce = true;
            }
            curRoot = curRoot->m_right_son;
        }
        else if (key < curRoot->m_key){
            if (isRightSequnce == true){
                curRoot->extra -= amount;
                isRightSequnce = false;
            }
            curRoot = curRoot->m_left_son;
            }
        else {
            if (isRightSequnce == false){
                curRoot->extra += amount;
            }
            if (curRoot->m_right_son){
                curRoot->m_right_son->extra -= amount;
            }
            return; 
        }
    }
    return;
}

template<class Key, class Value>
double RankTree<Key, Value>::sumExtra(Key key){
    //bool isRightSequnce = false;
    RankNode<Key, Value> * curRoot = root;
    double sum = 0;
    while (curRoot != NULL) {
        if (key > curRoot->m_key){
            sum += curRoot->extra;
            curRoot = curRoot->m_right_son;
        }
        else if (key < curRoot->m_key){
            sum += curRoot->extra;
            curRoot = curRoot->m_left_son;
        }
        else {
            sum += curRoot->privateExtra;
            sum += curRoot->extra;
            return sum; 
        }
    }
    return 0;
}



template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::NextInOrder(RankNode<Key, Value> *node) {
    if (!node) {
        return nullptr;
    }
    if (node->m_right_son) {
        return findMinNode(node->m_right_son);
    }
    RankNode<Key, Value> *parent = node->m_parent;
    while (parent != nullptr && node == parent->m_right_son) {
        node = parent;
        parent = parent->m_parent;
    }
    return parent;
}



template<class Key, class Value>
RankNode<Key, Value> *findMinNode(RankNode<Key, Value> *root) {
    RankNode<Key, Value> *node = root;
    while (node->m_left_son != nullptr) {
        node = node->m_left_son;
    }
    return node;
}

template<class Key, class Value>
RankNode<Key, Value> *findMaxNode(RankNode<Key, Value> *root) {
    RankNode<Key, Value> *node = root;
    while (node->m_right_son != nullptr) {
        node = node->m_right_son;
    }
    return node;
}

template<class Key, class Value>
StatusType RankTree<Key, Value>::Delete(Key key) {
    RankNode<Key, Value> *node = FindByKey(key);
    if (!node)
        return StatusType::FAILURE;
    RankNode<Key, Value> *new_node = DeleteNode(node, node->parent);
    UpdateBalance(new_node);
    return StatusType::SUCCESS;
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::DeleteNode(RankNode<Key, Value> *node, RankNode<Key, Value> *nodeParent) {
    //if node is a leaf
    if (!node->m_left_son && !node->m_right_son) {
        if (nodeParent) {
            if (nodeParent->m_left_son == node) {
                nodeParent->m_left_son = nullptr;
            } else {
                nodeParent->m_right_son = nullptr;
            }
        }
        if (node == root)
            root = nullptr;
        delete node;
        size--;
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
        if (node == root)
            root = node->m_right_son;
        delete node;
        size--;
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
        if (node == root)
            root = node->m_left_son;
        delete node;
        size--;
        return nodeParent;
    }
        // if node has both sons
    else {
        RankNode<Key, Value> *maxNode = findMaxNode(node->m_left_son);
        RankNode<Key, Value> *maxNodeParent = maxNode->m_parent;
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
            if (node == root)
                root = maxNode;
            delete node;
            size--;
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
            if (node == root)
                root = maxNode;
            delete node;
            size--;
            return maxNodeParent;
        }
    }
}

template<class Key, class Value>
void RankTree<Key, Value>::UpdateBalance(RankNode<Key, Value> *node) {
    if (!node)
        return;
    int rHeight = 0, lHeight = 0;
    int weight = 1;
    if (node->m_right_son) {
        rHeight = node->m_right_son->m_height;
        weight += node->m_right_son->m_weight;
    }
    if (node->m_left_son) {
        lHeight = node->m_left_son->m_height;
        weight += node->m_left_son->m_weight;
    }
    if (node->m_left_son || node->m_right_son)
        node->m_height = 1 + std::max(rHeight, lHeight);
    else
        node->m_height = 0;
    node->m_weight = weight;
    this->Rotate(node);
    UpdateBalance(node->m_parent);
}





template<class Key, class Value>
int BalanceFactor(RankNode<Key, Value> *node) {
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

#endif //RATUV2_RankTree_H