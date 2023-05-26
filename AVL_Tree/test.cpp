
// -------------------- LIBRARIES --------------------
#include <iostream>
#include <map>

// -------------------- DEBUG ON! --------------------
#define DEBUG_ON

// ------------------ INCLUDE FILES ------------------
#include "AVLTree.h"
#include "AVLTree.cpp"

// --------------------- DEFINES ---------------------
#define NUMBER_OF_NODES 500
#define NUMBER_OF_TREES 500

// ------------------ TEST FUNCTIONS ------------------
void checkInsertAndDelete() {
    std::cout << "Check if insert and delete are working: ";
    for (int j = 0; j < NUMBER_OF_TREES; j++) {
        Tree<int, int> avlTree;
        std::vector<int> vec;
        for (int i = 0; i < NUMBER_OF_NODES; i++) {
            int a = rand();
            int b = rand();
            vec.push_back(a);
            avlTree.insert(a, b);
            std::sort(vec.begin(), vec.end());
        }
        std::vector<int> avl_vec = avlTree.returnKeysVector();
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] != avl_vec[i]) {
                std::cout << "fail" << std::endl;
                return;
            }
        }
        for (int i = 0; i < vec.size(); i += j) {
            avlTree.remove(vec[i]);
            vec.erase(vec.begin() + i);
        }
        avl_vec = avlTree.returnKeysVector();

        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] != avl_vec[i]) {
                std::cout << "fail" << std::endl;
                return;
            }
        }
    }
    std::cout << "pass" << std::endl;
}

void checkNodeData() {
    std::cout << "Check if node data correct: ";
    for (int j = 0; j < NUMBER_OF_TREES; j++) {
        Tree<int, int> avlTree;
        std::vector<int> vec;
        std::map<int, int> map_t;
        for (int i = 0; i < NUMBER_OF_NODES; i++) {
            int a = rand();
            int b = rand();
            vec.push_back(a);
            map_t.insert(std::make_pair(a, b));
            avlTree.insert(a, b);
            std::sort(vec.begin(), vec.end());
        }
        for (int i = 0; i < vec.size(); i += j + 1) {
            avlTree.remove(vec[i]);
            vec.erase(vec.begin() + i);
        }
        std::vector<int> avl_vec = avlTree.returnKeysVector();

        for (int &i: vec) {
            if (map_t[i] != avlTree.findNodeData(i)) {
                std::cout << "fail" << std::endl;
                return;
            }
        }
    }
    std::cout << "pass" << std::endl;
}

void checkMemoryleak() {
    std::cout << "Check memory leak: ";
    for (int j = 1; j < NUMBER_OF_TREES; j++) {
        Tree<int, int> tree;
        std::vector<int> keys_vector;
        int before_deletion;
        int after_deletion;
        for (int i = 0; i < NUMBER_OF_NODES; i++) {
            int a = rand();
            keys_vector.push_back(a);
            tree.insert(a, i);
        }
        before_deletion = tree.getNodeCounter();
        for (int i = 0; i < NUMBER_OF_NODES; i += j) {
            tree.remove(keys_vector[i]);
        }
        int nodes_deleted;
        after_deletion = tree.getNodeCounter();
        if (before_deletion % j != 0)
            nodes_deleted = before_deletion / j + 1;
        else
            nodes_deleted = before_deletion / j;
        if (before_deletion - nodes_deleted != after_deletion) {
            std::cout << "fail" << std::endl;
            return;
        }
    }
    std::cout << "pass" << std::endl;
}

int main() {
    checkInsertAndDelete();
    checkNodeData();
    checkMemoryleak();
    return 0;
}


