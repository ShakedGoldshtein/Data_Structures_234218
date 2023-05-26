// -------------------- LIBRARIES --------------------
#include <iostream>
#include <exception>
#include <cassert>

// ------------------ INCLUDE FILES ------------------
#ifndef AVL_TEST_H
#define AVL_TEST_H

// ----------------- ROTATE EXCEPTION -----------------
class rotateError : public std::exception {
public:
    const char *what() const noexcept override {
        return "Rotate error";
    }
};

// ------------------ AVL TREE CLASS ------------------
template<class T, class V>
class Tree {
private:

    class Node {
    public:
        T key;
        V data;
        int height;
        int balance; // positive if left higher
        Node *left_son;
        Node *right_son;

        explicit Node(const T key, const V data) : key(key), data(data), height(0), balance(0), left_son(nullptr),
                                                   right_son(nullptr) {}

        void updateBalance() {
            int left_son_height = getSonHeight(left_son);
            int right_son_height = getSonHeight(right_son);
            balance = left_son_height - right_son_height;
        }

        void updateHeight() {
            int left_son_height = getSonHeight(left_son);
            int right_son_height = getSonHeight(right_son);
            if (left_son_height > right_son_height)
                height = left_son_height + 1;
            else
                height = right_son_height + 1;
        }

    private:
        int getSonHeight(const Node *child) const {
            return child == nullptr ? -1 : child->height;
        }
    };

    Node *root;
    int nodes_counter;

    // ----------- TREE PRIVATE FUNCTIONS -----------
    void deleteTree(Node *root);

    Node *insertNodeRecursion(Node *current, Node *const new_node);

    static Node *rotate(Node *sub_root);

    static Node *LLrotate(Node *father);

    static Node *RRrotate(Node *father);

    Node *removeNodeRecursion(Node *current, const int key);

    bool nodeExist(Node *root, const T key) const;

public:
    // ----------- TREE PUBLIC FUNCTIONS -----------
    Tree();

    ~Tree();

    bool nodeExist(const T key) const;

    void insert(const T key, const V data);

    void remove(const int key);

    // ----------- TREE DEBUG FUNCTIONS -----------
#ifdef DEBUG_ON

    void printTree();

    void printTreeInorder(Node *root);

    int getNodeCounter();

    V findNodeData(const int key);

    T findNodeDataRecursion(const Node *root, const int key);

    std::vector<T> returnKeysVector() const;

    void buildKeysVector(const Node *root_t, std::vector<T> &vec) const;

    void getNodesRecursion(std::vector<Node *> &nodes_vector, Node *root);

#endif /* DEBUG_ON */
};

#endif /* AVL_TEST_H */

