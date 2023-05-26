// -------------------- LIBRARIES --------------------
#include <iostream>
#include <cassert>

// ------------------ INCLUDE FILES ------------------
#include "AVLTree.h"


template<class T, class V>
Tree<T, V>::Tree() : root(nullptr), nodes_counter(0) {}

template<class T, class V>
Tree<T, V>::~Tree() {
    deleteTree(root);
}

template<class T, class V>
void Tree<T, V>::deleteTree(Node *root) {
    if (root == nullptr)
        return;
    deleteTree(root->left_son);
    deleteTree(root->right_son);
    delete root;
    nodes_counter--;
}

template<class T, class V>
bool Tree<T, V>::nodeExist(const T key) const {
    return nodeExist(root, key);
}

template<class T, class V>
bool Tree<T, V>::nodeExist(Node *root, const T key) const {
    if (root == nullptr)
        return false;
    if (root->key == key)
        return true;
    return nodeExist(root->left_son, key) || nodeExist(root->right_son, key);
}

template<class T, class V>
void Tree<T, V>::insert(const T key, const V data) {
    if (nodeExist(root, key)) {
        return;
    }
    try {
        Node *new_node = new Node(key, data);
        nodes_counter++;
        if (root == nullptr)
            root = new_node;
        else {
            Node *new_root_after_rotate = insertNodeRecursion(root, new_node);
            if (new_root_after_rotate != nullptr) {
                root = new_root_after_rotate;
            }
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

template<class T, class V>
typename Tree<T, V>::Node *Tree<T, V>::insertNodeRecursion(Node *current, Node *const new_node) {
    Node *new_sub_root_after_rotate;
    // stop conditions
    if (current == nullptr)
        return nullptr;

    // recursion move
    if (new_node->key < current->key) {
        new_sub_root_after_rotate = insertNodeRecursion(current->left_son, new_node);
        if (new_sub_root_after_rotate != nullptr) {
            current->left_son = new_sub_root_after_rotate;
            assert(current->left_son != nullptr);
        }
        if (current->left_son == nullptr)
            current->left_son = new_node;
        current->updateHeight();
    }
    else if (new_node->key > current->key) {
        new_sub_root_after_rotate = insertNodeRecursion(current->right_son, new_node);
        if (new_sub_root_after_rotate != nullptr) {
            current->right_son = new_sub_root_after_rotate;
            assert(current->right_son != nullptr);
        }
        if (current->right_son == nullptr)
            current->right_son = new_node;
        current->updateHeight();
    }
    current->updateBalance();
    if (abs(current->balance) > 1) {
        try {
            new_sub_root_after_rotate = rotate(current);
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        assert(current->balance <= 1 && current->balance >= -1);
        return new_sub_root_after_rotate;
    }
    return nullptr;
}

template<class T, class V>
typename Tree<T, V>::Node *Tree<T, V>::rotate(Node *sub_root) {
    Node *new_sub_root;
    if (sub_root->balance == 2 && sub_root->left_son->balance >= 0) { // LL ROTATE
        new_sub_root = LLrotate(sub_root);
        return new_sub_root;
    }
    else if (sub_root->balance == -2 && sub_root->right_son->balance <= 0) { // RR ROTATE
        new_sub_root = RRrotate(sub_root);
        return new_sub_root;
    }
    else if (sub_root->balance == 2 && sub_root->left_son->balance == -1) { // LR ROTATE
        Node *ll_sub_root = RRrotate(sub_root->left_son);
        sub_root->left_son = ll_sub_root;
        new_sub_root = LLrotate(sub_root);
        return new_sub_root;
    }
    else if (sub_root->balance == -2 && sub_root->right_son->balance == 1) { //RL ROTATE
        Node *rr_sub_root = LLrotate(sub_root->right_son);
        sub_root->right_son = rr_sub_root;
        new_sub_root = RRrotate(sub_root);
        return new_sub_root;
    }
    else {
        throw rotateError(); // exception
    }
}

template<class T, class V>
typename Tree<T, V>::Node *Tree<T, V>::LLrotate(Node *father) {
    assert(father->left_son != nullptr);
    Node *old_left_son = father->left_son;
    father->left_son = old_left_son->right_son;
    old_left_son->right_son = father;
    father->updateHeight();
    old_left_son->updateHeight();
    father->updateBalance();
    old_left_son->updateBalance();
    return old_left_son;
}

template<class T, class V>
typename Tree<T, V>::Node *Tree<T, V>::RRrotate(Node *father) {
    assert (father->right_son != nullptr);
    Node *old_right_son = father->right_son;
    father->right_son = old_right_son->left_son;
    old_right_son->left_son = father;
    old_right_son->left_son->updateHeight();
    old_right_son->updateHeight();
    old_right_son->left_son->updateBalance();
    old_right_son->updateBalance();
    return old_right_son;
}

template<class T, class V>
void Tree<T, V>::remove(const int key) {
    if (root == nullptr || !nodeExist(root, key))
        return;
    if (root != nullptr && root->key == key && root->right_son == nullptr && root->left_son == nullptr) {
        Node *node_to_delete = root;
        root = nullptr;
        delete node_to_delete;
        nodes_counter--;
    }
    else {
        Node *new_root_after_rotate = removeNodeRecursion(root, key);
        if (new_root_after_rotate != nullptr) {
            root = new_root_after_rotate;
        }
    }
}

template<class T, class V>
typename Tree<T, V>::Node *Tree<T, V>::removeNodeRecursion(Node *current, const int key) {
    Node *new_sub_root_after_rotate;
    // stop conditions
    if (current == nullptr)
        return nullptr;

    // recursion move
    if (key < current->key) {
        new_sub_root_after_rotate = removeNodeRecursion(current->left_son, key);
        if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == key) {
            delete new_sub_root_after_rotate;
            nodes_counter--;
            new_sub_root_after_rotate = nullptr;
            current->left_son = nullptr;
        }
        if (new_sub_root_after_rotate != nullptr) {
            current->left_son = new_sub_root_after_rotate;
            assert(current->left_son != nullptr);
        }
    }
    else if (key > current->key) {
        new_sub_root_after_rotate = removeNodeRecursion(current->right_son, key);
        if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == key) {
            delete new_sub_root_after_rotate;
            nodes_counter--;
            new_sub_root_after_rotate = nullptr;
            current->right_son = nullptr;
        }
        if (new_sub_root_after_rotate != nullptr) {
            current->right_son = new_sub_root_after_rotate;
        }
    }
    else if (key == current->key) {

        // node to delete found:
        if (current->left_son == nullptr && current->right_son == nullptr) { // node has no children

            Node *node_pointer_to_be_deleted = new Node(key, current->data);
            nodes_counter++;
            delete current;
            nodes_counter--;
            return node_pointer_to_be_deleted;

        }
        else if (current->left_son == nullptr) { // node has only right child

            Node *right_son = current->right_son;
            delete current;
            nodes_counter--;
            return right_son;
        }
        else if (current->right_son == nullptr) { // node has only left child

            Node *left_son = current->left_son;
            delete current;
            nodes_counter--;
            return left_son;
        }
        else {

            // father has two children:
            // 1. take the leftmost node of the right child: "successor".
            // 2. copy successor in place of the old right child.
            // 3. make recursion with root: "successor" and key: "successor_key" to delete the leaf copied from.
            // 4. the return of the recursion will update the whole race heights and balances.
            Node *successor = current->right_son;
            while (successor->left_son != nullptr) {
                successor = successor->left_son;
            }
            current->key = successor->key;
            current->data = successor->data;
            int successor_key = successor->key;
            new_sub_root_after_rotate = removeNodeRecursion(current->right_son, successor->key);
            if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == successor_key) {
                delete new_sub_root_after_rotate;
                nodes_counter--;
                current->right_son = nullptr;
                new_sub_root_after_rotate = nullptr;
            }
            if (new_sub_root_after_rotate != nullptr) {
                current->right_son = new_sub_root_after_rotate;
            }
        }
    }
    current->updateHeight();
    current->updateBalance();
    if (abs(current->balance) > 1) {
        try {
            new_sub_root_after_rotate = rotate(current);
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        assert(abs(current->balance) <= 1);
        return new_sub_root_after_rotate;
    }
    return nullptr;
}

// ------------------ debug functions ------------------
#ifdef DEBUG_ON

template<class T, class V>
void Tree<T, V>::printTree() {
    printTreeInorder(root);
    std::cout << std::endl;
}

template<class T, class V>
void Tree<T, V>::printTreeInorder(Node *root) {
    if (root == nullptr)
        return;
    printTreeInorder(root->left_son);
    std::cout << root->key << " ";
    printTreeInorder(root->right_son);
}

template<class T, class V>
int Tree<T, V>::getNodeCounter() {
    return nodes_counter;
}

template<class T, class V>
V Tree<T, V>::findNodeData(const int key) {
    V data = findNodeDataRecursion(root, key);
    return data;
}

template<class T, class V>
T Tree<T, V>::findNodeDataRecursion(const Node *root, const int key) {
    if (root != nullptr) {
        if (root->key == key) {
            return root->data;
        }
        T data = findNodeDataRecursion(root->left_son, key);
        if (data != T{}) {
            return data;
        }
        data = findNodeDataRecursion(root->right_son, key);
        if (data != T{}) {
            return data;
        }
    }
    return T{};
}

template<class T, class V>
std::vector<T> Tree<T, V>::returnKeysVector() const {
    std::vector<T> vec;
    buildKeysVector(root, vec);
    return vec;
}

template<class T, class V>
void Tree<T, V>::buildKeysVector(const Node *root_t, std::vector<T> &vec) const {
    if (root_t == nullptr)
        return;
    buildKeysVector(root_t->left_son, vec);
    vec.push_back(root_t->key);
    buildKeysVector(root_t->right_son, vec);
}

template<class T, class V>
void Tree<T, V>::getNodesRecursion(std::vector<Node *> &nodes_vector, Node *root) {
    if (root == nullptr)
        return;
    getNodesRecursion(nodes_vector, root->left_son);
    nodes_vector.push_back(root);
    getNodesRecursion(nodes_vector, root->right_son);
}

#endif /* DEBUG_ON */


