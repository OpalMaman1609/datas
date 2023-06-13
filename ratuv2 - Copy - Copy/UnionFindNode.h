#ifndef UNION_FIND_NODE_H
#define UNION_FIND_NODE_H

#include "utilesWet2.h"

#define EMPTY_KEY -1
#define EMPTY_VALUE -1

class UnionFindNode {
private:
    int m_key;
    int m_value;
    int m_height;
    int m_sales;
    int m_recordsTowerHeight;
    // a value that represent the place of the Node on the shelf.
    // even if the putOnTop method will be called with a big stack put on a small stack,
    // the order of the UnionFindNodes will be not as expectedm, but the m_baseColumn field will save order.
    int m_baseColumn;
public:
    UnionFindNode() = default;
    UnionFindNode(int key, int value);
    UnionFindNode(const UnionFindNode& other);
    int getKey();
    int getValue();
    int getHeight();
    int getBaseColumn();
    int getSales();
    int getTowerHeight();
    void incrementTowerHeight(int addHeight);
    void setHeight(int height);
    void setBaseColumn(int column);
    void incrementSales();
    bool operator==(const UnionFindNode& other);

};

#endif