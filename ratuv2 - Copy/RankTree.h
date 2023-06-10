#ifndef RATUV2_RankTree_H
#define RATUV2_RankTree_H

#include <iostream>
#include "utilesWet2.h"

template<class Key, class Value>
class RankNode {
public:
    Key key;
    Value value;
    RankNode *right_son;
    RankNode *left_son;
    RankNode *parent;
    int weight;
    int height;
    int extra;
    int privateExtra;

    RankNode(Key key, Value value)
            : key(key),
              value(value),
              right_son(nullptr),
              left_son(nullptr),
              parent(nullptr),
              weight(1),
              height(0),
              extra(0),
              privateExtra(0) {}

    RankNode(const RankNode<Key, Value> &other) {
        this->key = other.key;
        this->value = other.value;
        this->right_son = other.right_son;
        this->left_son = other.left_son;
        this->parent = other.parent;
        this->weight = other.weight;
        this->height = other.height;
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

    void deleteTree(RankNode<Key, Value> *node);

    int BF(RankNode<Key, Value> *node) const;

    RankNode<Key, Value> *RR(RankNode<Key, Value> *node);

    RankNode<Key, Value> *LL(RankNode<Key, Value> *node);

    RankNode<Key, Value> *RL(RankNode<Key, Value> *node);

    RankNode<Key, Value> *LR(RankNode<Key, Value> *node);

    RankNode<Key, Value> *rotate(RankNode<Key, Value> *node);

    StatusType insert(Key key, Value value);

    StatusType insertNode(RankNode<Key, Value> *root, RankNode<Key, Value> *parent, RankNode<Key, Value> *toInsert);

    RankNode<Key, Value> *deleteNode(RankNode<Key, Value> *node, RankNode<Key, Value> *nodeParent);

    void updateTreeBalance(RankNode<Key, Value> *node);

    StatusType remove(Key key);

    RankNode<Key, Value> *find(Key key);

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
        deleteTree(root);
    }
}

template<class Key, class Value>
RankTree<Key, Value>::RankTree(const RankTree<Key, Value> &other) {
    root = other.root;
    size = other.size;
}

template<class Key, class Value>
void RankTree<Key, Value>::deleteTree(RankNode<Key, Value> *node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->right_son);
    deleteTree(node->left_son);
    delete node;
}

//updates a nodes height
template<class Key, class Value>
void updateHeight(RankNode<Key, Value> *node) {
    int Rheight = 0;
    int Lheight = 0;
    int weight = 1;
    if (node->right_son != nullptr) {
        Rheight = node->right_son->height;
        weight += node->right_son->weight;
    }
    if (node->left_son != nullptr) {
        Lheight = node->left_son->height;
        weight += node->left_son->weight;
    }
    if (!node->left_son && !node->right_son) {
        node->height = 0;
    } else {
        node->height = 1 + (std::max(Rheight, Lheight));
    }
    node->weight = weight;
}


// Calculates Balance Factor
template<class Key, class Value>
int RankTree<Key, Value>::BF(RankNode<Key, Value> *node) const {
    if (node == nullptr) {
        return 0;
    }
    int Rheight = -1;
    int Lheight = -1;
    if (node->right_son != nullptr) {
        Rheight = node->right_son->height;
    }
    if (node->left_son != nullptr) {
        Lheight = node->left_son->height;
    }
    return Lheight - Rheight;
}

// Preforms Right Right rotation
template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::RR(RankNode<Key, Value> *node) {
    RankNode<Key, Value> *a;
    RankNode<Key, Value> *b;
    a = node;
    b = a->right_son;

    a->privateExtra += a->extra;
   if (a->left_son)
   {
        a->left_son->extra += a->extra; 
    }
    double bSum = a->extra +b->extra;
    a->extra = 0;
    b->privateExtra += bSum;
   if (b->left_son)
   {
        b->left_son->extra += bSum;
    }
   if (b->right_son)
   {
    b->right_son->extra += bSum;
    }
    b->extra = 0;

    a->right_son = b->left_son;
    if (b->left_son) {
        b->left_son->parent = a;
    }
    RankNode<Key, Value> *a_parent = a->parent;
    if (a_parent) {
        if (a_parent->right_son == a)
            a_parent->right_son = b;
        else if (a_parent->left_son == a)
            a_parent->left_son = b;
    }
    b->parent = a_parent;
    b->left_son = a;
    a->parent = b;
    updateHeight(a);
    updateHeight(b);
    if (root == node) {
        root = b;
    }
    return b;
}

// Preforms Left Left rotation
template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::LL(RankNode<Key, Value> *node) {
    RankNode<Key, Value> *a;
    RankNode<Key, Value> *b;
    a = node;
    b = a->left_son;
    a->privateExtra += a->extra;
    if (a->right_son)
    {
        a->right_son->extra += a->extra;
    }
    double bSum = a->extra +b->extra;
    a->extra = 0;
    b->privateExtra += bSum;
    if (b->left_son)
    {
       b->left_son->extra += bSum;
    }
    if (b->right_son)
    {
        b->right_son->extra += bSum;
    }
    b->extra = 0;
    a->left_son = b->right_son;
    if (b->right_son) {
        b->right_son->parent = a;
    }
    RankNode<Key, Value> *a_parent = a->parent;
    if (a_parent) {
        if (a_parent->right_son == a)
            a_parent->right_son = b;
        else if (a_parent->left_son == a)
            a_parent->left_son = b;
    }
    b->parent = a_parent;
    b->right_son = a;
    a->parent = b;
    updateHeight(a);
    updateHeight(b);
    if (root == node) {
        root = b;
    }
    return b;
}

// Preforms Right Left rotation
template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::RL(RankNode<Key, Value> *node) {
    node->right_son = LL(node->right_son);
    return RR(node);
}

// Preforms Left Right rotation
template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::LR(RankNode<Key, Value> *node) {
    node->left_son = RR(node->left_son);
    return LL(node);
}

// Preforms the rotation needed
template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::rotate(RankNode<Key, Value> *node) {
    int bf = BF(node);
    if (bf >= 2) {
        if (BF(node->left_son) >= 0) {
            return LL(node);
        } else {
            return LR(node);
        }
    } else if (bf <= -2) {
        if (BF(node->right_son) > 0) {
            return RL(node);
        } else {
            return RR(node);
        }
    } else return node;
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
        if (key > curRoot->key){
            if (isRightSequnce == false){
                curRoot->extra += amount;
                isRightSequnce = true;
            }
            curRoot = curRoot->right_son;
        }
        else if (key < curRoot->key){
            if (isRightSequnce == true){
                curRoot->extra -= amount;
                isRightSequnce = false;
            }
            curRoot = curRoot->left_son;
            }
        else {
            if (isRightSequnce == false){
                curRoot->extra += amount;
            }
            if (curRoot->right_son){
                curRoot->right_son->extra -= amount;
            }
            return; 
        }
    }
    return;
}

template<class Key, class Value>
double RankTree<Key, Value>::sumExtra(Key key){
    bool isRightSequnce = false;
    RankNode<Key, Value> * curRoot = root;
    double sum = 0;
    while (curRoot != NULL) {
        if (key > curRoot->key){
            sum += curRoot->extra;
            curRoot = curRoot->right_son;
        }
        else if (key < curRoot->key){
            sum += curRoot->extra;
            curRoot = curRoot->left_son;
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
RankNode<Key, Value> *RankTree<Key, Value>::findClosestSmallerKey(Key target) {
    Key result = -1;
    RankNode<Key, Value> * curRoot = root;
    while (curRoot) {
        if (curRoot->key < target) {
            result = result > curRoot->key ? result : curRoot->key;
            curRoot = curRoot->right_son;
        } else {
            curRoot = curRoot->left_son;
        }
    }
    return find(result);
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::NextInOrder(RankNode<Key, Value> *node) {
    if (!node) {
        return nullptr;
    }
    if (node->right_son) {
        return findMinNode(node->right_son);
    }
    RankNode<Key, Value> *parent = node->parent;
    while (parent != nullptr && node == parent->right_son) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

template<class Key, class Value>
StatusType
RankTree<Key, Value>::insertNode(RankNode<Key, Value> *rootNode, RankNode<Key, Value> *newParent,
                                 RankNode<Key, Value> *toInsert) {
    if (!rootNode) {
        rootNode = toInsert;
        rootNode->parent = newParent;
        if (newParent) {
            if (newParent->key > toInsert->key) {
                newParent->left_son = rootNode;
            } else newParent->right_son = rootNode;
        }
        size++;
    } else if (rootNode->key == toInsert->key) {
        return StatusType::FAILURE;

    } else if (rootNode->key > toInsert->key) {
        insertNode(rootNode->left_son, rootNode, toInsert);
    } else if (rootNode->key < toInsert->key) {
        insertNode(rootNode->right_son, rootNode, toInsert);
    }
    int rHeight = 0, lHeight = 0;
    int weight = 1;
    if (newParent) {
        if (newParent->right_son) {
            rHeight = newParent->right_son->height;
            weight += newParent->right_son->weight;
        }
        if (newParent->left_son) {
            lHeight = newParent->left_son->height;
            weight += newParent->left_son->weight;
        }
        if (newParent->left_son || newParent->right_son)
            newParent->height = 1 + std::max(rHeight, lHeight);
        else
            newParent->height = 0;
        newParent->weight = weight;
        this->rotate(newParent);
    }
    return StatusType::SUCCESS;
}

template<class Key, class Value>
RankNode<Key, Value> *findMinNode(RankNode<Key, Value> *root) {
    RankNode<Key, Value> *node = root;
    while (node->left_son != nullptr) {
        node = node->left_son;
    }
    return node;
}

template<class Key, class Value>
RankNode<Key, Value> *findMaxNode(RankNode<Key, Value> *root) {
    RankNode<Key, Value> *node = root;
    while (node->right_son != nullptr) {
        node = node->right_son;
    }
    return node;
}

template<class Key, class Value>
StatusType RankTree<Key, Value>::remove(Key key) {
    RankNode<Key, Value> *node = find(key);
    if (!node)
        return StatusType::FAILURE;
    RankNode<Key, Value> *new_node = deleteNode(node, node->parent);
    updateTreeBalance(new_node);
    return StatusType::SUCCESS;
}

template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::deleteNode(RankNode<Key, Value> *node, RankNode<Key, Value> *nodeParent) {
    //if node is a leaf
    if (!node->left_son && !node->right_son) {
        if (nodeParent) {
            if (nodeParent->left_son == node) {
                nodeParent->left_son = nullptr;
            } else {
                nodeParent->right_son = nullptr;
            }
        }
        if (node == root)
            root = nullptr;
        delete node;
        size--;
        return nodeParent;
    }
        // if node has only right son
    else if (!node->left_son && node->right_son) {
        if (nodeParent) {
            if (nodeParent->left_son == node) {
                nodeParent->left_son = node->right_son;
            } else {
                nodeParent->right_son = node->right_son;
            }
        }
        node->right_son->parent = nodeParent;
        if (node == root)
            root = node->right_son;
        delete node;
        size--;
        return nodeParent;
    }
        // if node has only left son
    else if (node->left_son && !node->right_son) {
        if (nodeParent) {
            if (nodeParent->left_son == node) {
                nodeParent->left_son = node->left_son;
            } else {
                nodeParent->right_son = node->left_son;
            }
        }
        node->left_son->parent = nodeParent;
        if (node == root)
            root = node->left_son;
        delete node;
        size--;
        return nodeParent;
    }
        // if node has both sons
    else {
        RankNode<Key, Value> *maxNode = findMaxNode(node->left_son);
        RankNode<Key, Value> *maxNodeParent = maxNode->parent;
        //if maxNode is a direct son of node
        if (node->left_son == maxNode) {
            node->right_son->parent = maxNode;
            maxNode->right_son = node->right_son;
            maxNode->parent = nodeParent;
            if (nodeParent) {
                if (nodeParent->left_son == node) {
                    nodeParent->left_son = maxNode;
                } else {
                    nodeParent->right_son = maxNode;
                }
            }
            if (node == root)
                root = maxNode;
            delete node;
            size--;
            return maxNode;
            //if maxNode isn't a direct son of node
        } else {
            node->right_son->parent = maxNode;
            node->left_son->parent = maxNode;
            //if maxNode has a left son
            if (maxNode->left_son) {
                if (maxNodeParent->left_son == maxNode) {
                    maxNodeParent->left_son = maxNode->left_son;
                } else {
                    maxNodeParent->right_son = maxNode->left_son;
                }
                maxNode->left_son->parent = maxNodeParent;
                //if maxNode is a leaf
            } else {
                if (maxNodeParent->left_son == maxNode) {
                    maxNodeParent->left_son = nullptr;
                } else {
                    maxNodeParent->right_son = nullptr;
                }
            }
            maxNode->parent = nodeParent;
            maxNode->right_son = node->right_son;
            maxNode->left_son = node->left_son;
            if (nodeParent) {
                if (nodeParent->left_son == node) {
                    nodeParent->left_son = maxNode;
                } else {
                    nodeParent->right_son = maxNode;
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
void RankTree<Key, Value>::updateTreeBalance(RankNode<Key, Value> *node) {
    if (!node)
        return;
    int rHeight = 0, lHeight = 0;
    int weight = 1;
    if (node->right_son) {
        rHeight = node->right_son->height;
        weight += node->right_son->weight;
    }
    if (node->left_son) {
        lHeight = node->left_son->height;
        weight += node->left_son->weight;
    }
    if (node->left_son || node->right_son)
        node->height = 1 + std::max(rHeight, lHeight);
    else
        node->height = 0;
    node->weight = weight;
    this->rotate(node);
    updateTreeBalance(node->parent);
}


template<class Key, class Value>
RankNode<Key, Value> *RankTree<Key, Value>::find(Key key) {
    return findNode(root, key);
}


template<class Key, class Value>
RankNode<Key, Value> *findNode(RankNode<Key, Value> *rootNode, Key key) {
    if (!rootNode) {
        return nullptr;
    } else if (rootNode->key == key) {
        return rootNode;
    } else if (rootNode->key > key)
        return findNode(rootNode->left_son, key);
    else {
        return findNode(rootNode->right_son, key);
    }
}


template<class Key, class Value>
int BalanceFactor(RankNode<Key, Value> *node) {
    if (node == nullptr) {
        return 0;
    }
    int Rheight = -1;
    int Lheight = -1;
    if (node->right_son != nullptr) {
        Rheight = node->right_son->height;
    }
    if (node->left_son != nullptr) {
        Lheight = node->left_son->height;
    }
    return Lheight - Rheight;
}

#endif //RATUV2_RankTree_H