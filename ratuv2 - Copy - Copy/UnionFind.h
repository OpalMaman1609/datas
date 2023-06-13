#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "utilesWet2.h"
#include "UnionFindNode.h"

#define NOT_GROUP_ROOT 1

class UnionFind {
private:
    int* m_groupSize;
    int* m_parent;
    UnionFindNode* m_elements;
    int m_numberOfNodes;
    void clean();
public:
    UnionFind(int *values, int numberOfNodes);
    UnionFind();
    UnionFind& operator=(const UnionFind& other);
    virtual ~UnionFind();
    UnionFindNode& Find(int key);
    void Union(int keyOne, int keyTwo);
    void putOnTop(int keyTop, int keyBottom);
    int Height(int key);
    int getSize();
    UnionFindNode& getElement(int key);
};

#endif