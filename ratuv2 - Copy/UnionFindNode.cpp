#include "UnionFindNode.h"

int UnionFindNode::getKey() {
    return m_key;
}

int UnionFindNode::getValue() {
    return m_value;
}

int UnionFindNode::getHeight() {
    return m_height;
}

int UnionFindNode::getBaseColumn() {
    return m_baseColumn;
}

int UnionFindNode::getSales() {
    return m_sales;
}

int UnionFindNode::getTowerHeight() {
    return m_recordsTowerHeight;
}

void UnionFindNode::incrementTowerHeight(int addHeight) {
    m_recordsTowerHeight += addHeight;
}

void UnionFindNode::setHeight(int height) {
    m_height = height;
}

void UnionFindNode::setBaseColumn(int column) {
    m_baseColumn = column;
}

void UnionFindNode::incrementSales() {
    m_sales++;
}

// return equal if both Nodes are in the same column
bool UnionFindNode::operator==(const UnionFindNode& other) {
    return this->m_baseColumn == other.m_baseColumn;
}

UnionFindNode::UnionFindNode(int key, int value): m_key(key), 
    m_value(value), m_height(0), m_sales(0), m_baseColumn(key), m_recordsTowerHeight(value) {};

UnionFindNode::UnionFindNode(const UnionFindNode& other) {
    m_key = other.m_key;
    m_value = other.m_value;
    m_height = other.m_height;
    m_sales = other.m_sales;
    m_baseColumn = other.m_baseColumn;
    m_recordsTowerHeight = other.m_recordsTowerHeight;
}