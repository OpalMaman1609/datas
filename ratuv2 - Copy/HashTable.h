#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "utilesWet2.h"
#include <iostream>
#include "AvlTree.h"

// Hashtable with chain hashing based on an AVL tree with load factor of 2


// ----Hash Table----
template<class Value>
class HashTable {

public:
    int size;
    int Values_amount;
    int load_factor;
    AvlTree<int, Value> *array;

    explicit HashTable();

    ~HashTable();

    AvlNode<int, Value> *find_HT(int key);

    AvlNode<int, Value> *operator[](int place);

    const AvlNode<int, Value> *operator[](int place) const;

    void clear();

    void resize(int to_change);

    StatusType insert_HT(int id, Value value);

    bool remove_HT(int id);
};


// Constructor
template<class Value>
HashTable<Value>::HashTable()
        : size(10), Values_amount(0), load_factor(2)  {
            array = new AvlTree<int, Value>[size]();
        }

// Dtor
template<class Value>
HashTable<Value>::~HashTable() {
    this->clear();
}

template<class Value>
AvlNode<int, Value> *HashTable<Value>::find_HT(int key) {
    if (array[key % size].FindByKey(key) == nullptr) {
        return nullptr;
    } else {
        return array[key % size].FindByKey(key);
    }
}

template<class Value>
AvlNode<int, Value> *HashTable<Value>::operator[](int place) {
    if (place >= size || place < 0) {
        return nullptr;
    }
    return array[place];
}

template<class Value>
const AvlNode<int, Value> *HashTable<Value>::operator[](int place) const {
    if (place >= size || place < 0) {
        return nullptr;
    }
    return array[place];
}

template<class Value>
void HashTable<Value>::clear() {
    delete[] array;
}

template<class Value>
void HashTable<Value>::resize(int to_change) {
    if (to_change == 1) {
        size *= 2;
    } else {
        size /= 2;
    }
}

template<class Value>
StatusType HashTable<Value>::insert_HT(int id, Value value) {
    if (this->find_HT(id) != nullptr) {
        return StatusType::ALREADY_EXISTS;
    }
    if ((this->load_factor * this->size) < this->Values_amount) {
        AvlNode<int, Value> *current;
        //try {
        auto bigger_array = new AvlTree<int, Value>[2 * size]();
         //} catch (std::bad_alloc &e) {
         //   return StatusType::ALLOCATION_ERROR;
         // }
        for (int i = 0; i < this->size; i++) {
            if (array[i].m_root != nullptr) {
                current = findMinNode(array[i].m_root);
                while (current != nullptr) {
                    bigger_array[current->m_key % (2 * this->size)].Insert(current->m_key, current->m_value);
                    current = array[i].NextInOrder(current);
                }
            }
        }
        this->clear();
        array = bigger_array;
        this->resize(1);
    }
    array[id % this->size].Insert(id, value);
    this->Values_amount++;
    return StatusType::SUCCESS;
}


template<class Value>
bool HashTable<Value>::remove_HT(int id) {
    if (this->find_HT(id) == nullptr) {
        return false;
    }
    if ((this->load_factor * this->Values_amount) < this->size) {
        AvlNode<int, Value> *current;
        auto smaller_array = new AvlTree<int, Value>[this->size / 2]();
        for (int i = 0; i < this->size; i++) {
            if (array[i].m_root != nullptr) {
                current = findMinNode(array[i].m_root);
                while (current != nullptr) {
                    smaller_array[current->m_key % (this->size / 2)].Insert(current->id, current->value);
                    current = array[i].NextInOrder(current);
                }
            }
        }
        this->clear();
        array = smaller_array;
        this->resize(2);
    }
    array[id % this->size].Delete(id);
    this->Values_amount--;
    return true;
}


#endif /* HASH_TABLE_H */