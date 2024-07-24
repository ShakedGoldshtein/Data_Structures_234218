#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// -------------------- DEFINES --------------------
#define INITIAL_HASH_SIZE 3
#define INCREASE_HASH_SIZE_MULTIPLES 2

// -------------------- LIBRARIES --------------------
#include "AVLTree/rankedAVLTree.h"

// --------------------- READ ME ---------------------
// This templated chain hash table that every bucket point to AVL tree.
// Amortized analysis on average input: O(1)
// Functions: init, insert, getData, nodeExist.

template<class dataType>
class HashTable {

private:
    Tree<int, dataType> *buckets;
    int hash_size;
    int hash_nodes_counter;

    int hashFunction(int key) const {
        return key % hash_size;
    }

public:
    HashTable() : hash_size(INITIAL_HASH_SIZE), hash_nodes_counter(0) {
        buckets = new Tree<int, dataType>[hash_size];
    }

    void insert(int new_key, dataType new_data) {
        int index = hashFunction(new_key);
        if (nodeExist(new_key))
            return;

        buckets[index].insert(new_key, new_data);
        hash_nodes_counter += 1;

        if (hash_nodes_counter / hash_size == 1) {
            int old_hash_size = hash_size;
            hash_size = hash_size * INCREASE_HASH_SIZE_MULTIPLES - 1;
            try {
                auto *new_buckets = new Tree<int, dataType>[hash_size];

                for (int i = 0; i < old_hash_size; i++) {
                    while (buckets[i].getRoot() != nullptr) {
                        int move_key = buckets[i].getRoot()->key;
                        int new_index = hashFunction(move_key);
                        dataType move_data =  buckets[i].getRoot()->data;
                        new_buckets[new_index].insert(move_key, move_data);
                        buckets[i].remove(move_key);
                    }
                }
                delete[] buckets;
                buckets = new_buckets;
            }
            catch (std::bad_alloc &e) {
                hash_size = old_hash_size;
                throw;
            }
        }
    }

    bool nodeExist(int key) {
        int index = hashFunction(key);
        if (buckets[index].find(key))
            return true;
        return false;
    }

    dataType& getData(int key) {
        int index = hashFunction(key);
        return buckets[index].find(key)->data;
    }
};

#endif /* HASH_TABLE_H */
