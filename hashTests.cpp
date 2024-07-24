#include <iostream>
#include "hashTable.h"

#define NUMBER_OF_NODES 500

void checkHash() {
    HashTable<int> hash;
    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        hash.insert(i,i);
    }
}

int main() {
    checkHash();
    return 0;
}