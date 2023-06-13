#include "UnionFind.h"

UnionFind::UnionFind(int* values, int numberOfNodes): m_numberOfNodes(numberOfNodes) {
    m_elements = new UnionFindNode[numberOfNodes];
    m_parent = new int[numberOfNodes];
    m_groupSize = new int[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++) {
        m_elements[i] = UnionFindNode(i, values[i]);
        m_parent[i] = EMPTY_KEY;
        m_groupSize[i] = NOT_GROUP_ROOT;
    }
}

UnionFind::UnionFind():  m_groupSize(nullptr), m_parent(nullptr), m_elements(nullptr), m_numberOfNodes(0) {};

UnionFind& UnionFind::operator=(const UnionFind& other) {
    this->clean();
    this->m_numberOfNodes = other.m_numberOfNodes;
    m_elements = new UnionFindNode[this->m_numberOfNodes];
    m_parent = new int[this->m_numberOfNodes];
    m_groupSize = new int[m_numberOfNodes];
    for (int i = 0; i < m_numberOfNodes; i++) {
        m_elements[i] = UnionFindNode(i, other.m_elements[i].getValue());
        m_parent[i] = EMPTY_KEY;
        m_groupSize[i] = NOT_GROUP_ROOT;
    }

    return *this;
}

void UnionFind::clean() {
    if (m_groupSize != nullptr) {
        delete[] m_groupSize;
    }

    if (m_parent != nullptr) {
        delete[] m_parent;
    }

    if (m_elements != nullptr) {
        delete[] m_elements;
    }
}

UnionFind::~UnionFind() {
    this->clean();
    
}

int UnionFind::getSize() {
    return m_numberOfNodes;
}

UnionFindNode& UnionFind::getElement(int key) {
    return m_elements[key];
}

void UnionFind::Union(int keyOne, int keyTwo) {
    int root, child;
    if (m_groupSize[keyOne] >= m_groupSize[keyTwo]) {
        root = keyOne;
        child = keyTwo;
    } else {
        root = keyTwo;
        child = keyOne;
    }
    
    m_parent[child] = root;
    m_groupSize[root] += m_groupSize[child];
    m_groupSize[child] = NOT_GROUP_ROOT;
}

UnionFindNode& UnionFind::Find(int key) {
    int heightSum = 0;
    int toSubstruct = 0;
    UnionFindNode rootNode;
    bool keepSearch = true;
    int searchKey = key;
    do
    {
        // key is a group's root -> key is the root of group
        if (m_parent[searchKey] == EMPTY_KEY) {
            rootNode = m_elements[searchKey];
            keepSearch = false;
            continue;
        }
        heightSum += m_elements[searchKey].getHeight();
        searchKey = m_parent[searchKey];

    } while (keepSearch);
    
    

    // second iteration, fix find route (shrink route)
    searchKey = key;
    while (searchKey != EMPTY_KEY && searchKey != rootNode.getKey()) {
        int currentNodeHeight = m_elements[searchKey].getHeight();
        m_parent[searchKey] = rootNode.getKey();
        m_elements[searchKey].setHeight(heightSum - toSubstruct);
        toSubstruct += currentNodeHeight;
        searchKey = m_parent[searchKey];
    }

    return m_elements[rootNode.getKey()];
}

int UnionFind::Height(int key) {
    int height = 0;
    int rootKey = key;
    while (m_parent[rootKey] != EMPTY_KEY) {
        height += m_elements[rootKey].getHeight();
        rootKey = m_parent[rootKey];
    }

    // after find, node should be connected directly to root
    return this->Find(key).getHeight() + height;
}

void UnionFind::putOnTop(int keyTop, int keyBottom) {
    // update values
    UnionFindNode& topRoot = this->Find(keyTop);
    UnionFindNode& bottomRoot = this->Find(keyBottom);

    topRoot.setBaseColumn(keyBottom);
    
    int bottomHeight = this->Find(keyBottom).getTowerHeight();
    int topHeight = this->Find(keyTop).getTowerHeight();

    int topRootKey = topRoot.getKey();
    int bottomRootKey = bottomRoot.getKey();

    int topHeightAuxValue = topRoot.getHeight();
    int bottomHeightAuxValue = bottomRoot.getHeight();
    if (m_groupSize[bottomRootKey] > m_groupSize[topRootKey]) {
        topRoot.setHeight(topHeightAuxValue + bottomHeight - bottomHeightAuxValue);
        this->Find(keyBottom).incrementTowerHeight(topHeight);
    } else {
        // in union, keyBottom will point to keyTop (opposite of what user asked)
        // need to update values accordingly
        topRoot.setHeight(topHeightAuxValue + bottomHeight);

        // bottom node will now count values of top node, need to reduce those values
        // accordingly
        bottomRoot.setHeight(bottomHeightAuxValue - topRoot.getHeight()); 

        // top will become the root in the UnionFind
        topRoot.incrementTowerHeight(bottomHeight);

    }

    // union
    this->Union(topRootKey, bottomRootKey);
}