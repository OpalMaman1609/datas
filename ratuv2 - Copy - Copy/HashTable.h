#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "utilesWet2.h"
#include <iostream>
#include "AvlTree.h"

/*Chain hashing hash table, LF is 2*/

template<class Value>
class HashTable {

public:
    int m_size;
    int m_valueSize;
    int m_loadFactor;
    AvlTree<int, Value> *array;

    explicit HashTable();

    ~HashTable();

    void changeSize(int toBigger);

    StatusType insertH(int id, Value value);

    AvlNode<int, Value> *findH(int key);

    AvlNode<int, Value> *operator[](int index);

    const AvlNode<int, Value> *operator[](int index) const;


};


template<class Value>
HashTable<Value>::HashTable(): m_size(10), m_valueSize(0), m_loadFactor(2)  
    {
    array = new AvlTree<int, Value>[m_size]();
    }


template<class Value>
HashTable<Value>::~HashTable() {
    AvlNode<int, Value> *current;
    for (int i = 0; i < this->m_size; i++) {
        if (array[i].m_root != nullptr) {
            current = findMinNode(array[i].m_root);
                while (current != nullptr) {
                    delete current->m_value;
                    current = array[i].NextInOrder(current);
                }
            }
        }
    delete[] array;
}

template<class Value>
AvlNode<int, Value> *HashTable<Value>::findH(int key) {
    if (array[key % m_size].FindByKey(key) == nullptr) {
        return nullptr;
    } else {
        return array[key % m_size].FindByKey(key);
    }
}

template<class Value>
AvlNode<int, Value> *HashTable<Value>::operator[](int index) {
    if (index >= m_size || index < 0) {
        return nullptr;
    }
    return array[index];
}

template<class Value>
const AvlNode<int, Value> *HashTable<Value>::operator[](int index) const {
    if (index >= m_size || index < 0) {
        return nullptr;
    }
    return array[index];
}


template<class Value>
void HashTable<Value>::changeSize(int toBigger) {
    if (toBigger == 1) {
        m_size *= 2;
    } else {
        m_size /= 2;
    }
}

template<class Value>
StatusType HashTable<Value>::insertH(int id, Value value) {
    if (this->findH(id) != nullptr) {
        return StatusType::ALREADY_EXISTS;
    }
    if ((this->m_loadFactor * this->m_size) < this->m_valueSize) {
        AvlNode<int, Value> *current;
        //try {
        auto bigArray = new AvlTree<int, Value>[2 * m_size]();
         //} catch (std::bad_alloc &e) {
         //   return StatusType::ALLOCATION_ERROR;
         // }
        for (int i = 0; i < this->m_size; i++) {
            if (array[i].m_root != nullptr) {
                current = findMinNode(array[i].m_root);
                while (current != nullptr) {
                    bigArray[current->m_key % (2 * this->m_size)].Insert(current->m_key, current->m_value);
                    current = array[i].NextInOrder(current);
                }
            }
        }
        delete[] array;
        array = bigArray;
        this->changeSize(1);
    }
    array[id % this->m_size].Insert(id, value);
    this->m_valueSize++;
    return StatusType::SUCCESS;
}



#endif /* HASHTABLE_H */