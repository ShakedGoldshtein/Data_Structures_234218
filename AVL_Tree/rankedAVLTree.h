#ifndef AVL_RANKED_TREE_H
#define AVL_RANKED_TREE_H

// -------------------- LIBRARIES --------------------
#include <iostream>
#include <exception>
#include <cassert>


// ----------------- ROTATE EXCEPTION -----------------
class rotateError : public std::exception {
public:
    const char *what() const noexcept override {
        return "Rotate Error";
    }
};

// --------------------- READ ME ---------------------
// This templated AVL ranked tree.
// Functions:
// init, insert, remove, find, getRoot - get root node of the tree, getNodeRank.
// upgradeRank - upgrade whole keys between "keys_1 <= keys < keys_2" with amount of double.

// ------------------ AVL TREE CLASS ------------------
template<class keyType, class dataType>
class Tree {
private:

    class Node {
    public:
        keyType key;
        dataType data;
        double collector; // collector to calculate node rank from root to node
        double rank;
        int height;
        int balance; // positive if left higher
        Node *left_son;
        Node *right_son;

        explicit Node(const keyType key, const dataType data) : key(key), data(data), collector(0), rank(0),
                                                                height(0), balance(0), left_son(nullptr),
                                                                right_son(nullptr) {}

        void updateRank(double increase_rank) {
            this->rank += increase_rank;
        }

        void updateCollector(double increase_collector) {
            this->collector += increase_collector;
        }

        void resetCollector() {
            this->collector = 0;
        }

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

        int getSonHeight(const Node *child) const {
            return child == nullptr ? -1 : child->height;
        }
    };

    Node *root;
    int avl_nodes_counter;

    // ----------- TREE PRIVATE FUNCTIONS -----------
    void deleteTree(Node *node) {
        if (node == nullptr)
            return;
        deleteTree(node->right_son);
        deleteTree(node->left_son);
        delete node;
        avl_nodes_counter--;
    }

    Node *insertNode(Node *current, Node *const new_node, double current_collector) {
        Node *new_sub_root_after_rotate;
        // stop conditions
        if (current == nullptr)
            return nullptr;

        // recursion move
        if (new_node->key < current->key) {
            new_sub_root_after_rotate = insertNode(current->left_son, new_node,
                                                   current_collector + current->collector);
            if (new_sub_root_after_rotate != nullptr) {
                current->left_son = new_sub_root_after_rotate;
                assert(current->left_son != nullptr);
            }
            if (current->left_son == nullptr) {
                current->left_son = new_node;
                new_node->collector -= current_collector + current->collector;
            }
            current->updateHeight();
        }
        else if (new_node->key > current->key) {
            new_sub_root_after_rotate = insertNode(current->right_son, new_node,
                                                   current_collector + current->collector);
            if (new_sub_root_after_rotate != nullptr) {
                current->right_son = new_sub_root_after_rotate;
                assert(current->right_son != nullptr);
            }
            if (current->right_son == nullptr) {
                current->right_son = new_node;
                new_node->collector -= current_collector + current->collector;
            }
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

    static Node *rotate(Node *sub_root) {
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

    static void updateRanksBeforeRotate(Node *father, Node *son) {
        if (son != nullptr) {
            son->updateCollector(father->collector);
            Node *left_grandson = son->left_son;
            if (left_grandson != nullptr)
                left_grandson->updateCollector(son->collector);
            Node *right_grandson = son->right_son;
            if (right_grandson != nullptr)
                right_grandson->updateCollector(son->collector);
            son->updateRank(son->collector);
            son->resetCollector();
        }
    }

    static Node *LLrotate(Node *father) {
        assert(father->left_son != nullptr);
        Node *old_left_son = father->left_son;

        updateRanksBeforeRotate(father, father->left_son);
        updateRanksBeforeRotate(father, father->right_son);
        father->updateRank(father->collector);
        father->resetCollector();

        father->left_son = old_left_son->right_son;
        old_left_son->right_son = father;
        father->updateHeight();
        old_left_son->updateHeight();
        father->updateBalance();
        old_left_son->updateBalance();
        return old_left_son;
    }

    static Node *RRrotate(Node *father) {
        assert (father->right_son != nullptr);
        Node *old_right_son = father->right_son;

        updateRanksBeforeRotate(father, father->left_son);
        updateRanksBeforeRotate(father, father->right_son);
        father->updateRank(father->collector);
        father->resetCollector();

        father->right_son = old_right_son->left_son;
        old_right_son->left_son = father;
        father->updateHeight();
        old_right_son->updateHeight();
        father->updateBalance();
        old_right_son->updateBalance();
        return old_right_son;
    }

    Node *removeNode(Node *current, const int key) {
        Node *new_sub_root_after_rotate;
        // stop conditions
        if (current == nullptr)
            return nullptr;

        // recursion move
        if (key < current->key) {
            new_sub_root_after_rotate = removeNode(current->left_son, key);
            if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == key) {
                delete new_sub_root_after_rotate;
                avl_nodes_counter--;
                new_sub_root_after_rotate = nullptr;
                current->left_son = nullptr;
            }
            if (new_sub_root_after_rotate != nullptr) {
                current->left_son = new_sub_root_after_rotate;
                assert(current->left_son != nullptr);
            }
        }
        else if (key > current->key) {
            new_sub_root_after_rotate = removeNode(current->right_son, key);
            if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == key) {
                delete new_sub_root_after_rotate;
                avl_nodes_counter--;
                new_sub_root_after_rotate = nullptr;
                current->right_son = nullptr;
            }
            if (new_sub_root_after_rotate != nullptr) {
                current->right_son = new_sub_root_after_rotate;
            }
        }
        else if (key == current->key) { // node to delete found:

            if (current->left_son == nullptr && current->right_son == nullptr) { // node has no children
                Node *node_pointer_to_be_deleted = new Node(key, dataType{});
                avl_nodes_counter++;
                delete current;
                avl_nodes_counter--;
                return node_pointer_to_be_deleted;

            }
            else if (current->left_son == nullptr) { // node has only right child

                Node *right_son = current->right_son;
                delete current;
                avl_nodes_counter--;
                return right_son;
            }
            else if (current->right_son == nullptr) { // node has only left child

                Node *left_son = current->left_son;
                delete current;
                avl_nodes_counter--;
                return left_son;
            }
            else {

                // node has two children:
                // 1. take the leftmost node of the right child (named "successor").
                // 2. copy successor in place of the old right child.
                // 3. make recursion with root: "successor" and key: "successor_key" to delete the leaf copied from.
                // 4. the return of the recursion will update the whole race heights and balances.

                // section "1":
                Node *successor = current->right_son;
                while (successor->left_son != nullptr) {
                    successor = successor->left_son;
                }

                // section "2":
                current->key = successor->key;
                current->data = successor->data;

                // section "3":
                int successor_key = successor->key;
                new_sub_root_after_rotate = removeNode(current->right_son, successor->key);

                // section "4":
                if (new_sub_root_after_rotate != nullptr && new_sub_root_after_rotate->key == successor_key) {
                    delete new_sub_root_after_rotate;
                    avl_nodes_counter--;
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

    Node *find(Node *node, const keyType key) const {
        if (node == nullptr || node->key == key)
            return node;

        Node *leftResult = find(node->left_son, key);
        if (leftResult != nullptr)
            return leftResult;
        Node *rightResult = find(node->right_son, key);
        if (rightResult != nullptr)
            return rightResult;

        return nullptr;
    }

    void updateLeftBranchCollectors(int key_1, Node *node, double amount, bool moved_right) {
        if (node == nullptr)
            return;

        if (key_1 <= node->key) {
            if (moved_right) {
                node->collector += amount;
                moved_right = false;
            }
            updateLeftBranchCollectors(key_1, node->left_son, amount, moved_right);
        }

        if (key_1 > node->key) {
            node->collector -= amount;
            moved_right = true;
            updateLeftBranchCollectors(key_1, node->right_son, amount, moved_right);
        }
    }

    void updateRightBranchCollectors(int key_2, Node *node, double amount, bool moved_left) {
        if (node == nullptr)
            return;

        if (key_2 >= node->key) {
            if (moved_left) {
                node->collector += amount;
                moved_left = false;
            }
            updateRightBranchCollectors(key_2, node->right_son, amount, moved_left);
        }

        if (key_2 < node->key) {
            node->collector -= amount;
            moved_left = true;
            updateLeftBranchCollectors(key_2, node->left_son, amount, moved_left);
        }
    }

public:

    // ----------- TREE PUBLIC FUNCTIONS -----------
    Tree() : root(nullptr), avl_nodes_counter(0) {}

    ~Tree() {
        deleteTree(root);
    }

    Node *find(const keyType key) const {
        return find(root, key);
    }

    void insert(const keyType key, const dataType data) {
        if (find(root, key) != nullptr) {
            return;
        }
        try {
            Node *new_node = new Node(key, data);
            avl_nodes_counter++;
            if (root == nullptr)
                root = new_node;
            else {
                Node *new_root_after_rotate = insertNode(root, new_node, 0);
                if (new_root_after_rotate != nullptr) {
                    root = new_root_after_rotate;
                }
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void upgradeRank(int key_1, int key_2, double amount) {
        if (key_1 >= key_2)
            return;
        Node *ancestor = root;
        bool ancestor_found = false;
        while (!ancestor_found) {
            if (ancestor == nullptr)
                return;
            if (key_1 < ancestor->key && key_2 < ancestor->key)
                ancestor = ancestor->left_son;
            if (key_1 > ancestor->key && key_2 > ancestor->key)
                ancestor = ancestor->right_son;
            else
                ancestor_found = true;
        }
        updateLeftBranchCollectors(key_1, ancestor, amount, true);
        updateRightBranchCollectors(key_2, ancestor, amount, false);
    }

    void remove(const int key) {
        if (find(root, key) == nullptr)
            return;
        if (root != nullptr && root->key == key && root->right_son == nullptr && root->left_son == nullptr) {
            Node *node_to_delete = root;
            root = nullptr;
            delete node_to_delete;
            avl_nodes_counter--;
        }
        else {
            Node *new_root_after_rotate = removeNode(root, key);
            if (new_root_after_rotate != nullptr) {
                root = new_root_after_rotate;
            }
        }
    }

    Node *getRoot() const {
        return root;
    }

    double getNodeRank(const Node *node, keyType key, double current_collector = 0) const {
        if (node == nullptr)
            return 0.0;
        if (node->key == key)
            return node->rank + node->collector + current_collector;

        if (key < node->key)
            getNodeRank(node->left_son, key, node->collector + current_collector);
        if (key > node->key)
            getNodeRank(node->right_son, key, node->collector + current_collector);
        return 0.0;
    }

    // -------------- DEBUG FUNCTIONS --------------

#ifdef DEBUG_ON

    void printTreeInorder(const Node *node) const {
        if (node == nullptr)
            return;
        printTreeInorder(node->left_son);
        std::cout << node->key << " ";
        printTreeInorder(node->right_son);
    }

    int getNodeCounter() const {
        return avl_nodes_counter;
    }

    std::vector<dataType> KeysVector() const {
        std::vector<dataType> vec;
        buildKeysVector(root, vec);
        return vec;
    }

    void buildKeysVector(const Node *root_t, std::vector<dataType> &vec) const {
        if (root_t == nullptr)
            return;
        buildKeysVector(root_t->left_son, vec);
        vec.push_back(root_t->key);
        buildKeysVector(root_t->right_son, vec);
    }

    void printCollectorsInorder(const Node *node) const {
        if (node == nullptr)
            return;
        printCollectorsInorder(node->left_son);
        std::cout << node->key << ": " << node->collector << std::endl;
        printCollectorsInorder(node->right_son);
    }

    void printRanksInorder(const Node *node, double current_collector = 0) {
        if (node == nullptr)
            return;
        printRanksInorder(node->left_son, node->collector + current_collector);
        std::cout << node->key << ": " << node->rank + current_collector + node->collector << std::endl;
        printRanksInorder(node->right_son, node->collector + current_collector);
    }

#endif /* DEBUG_ON */

};


#endif /* AVL_RANKED_TREE_H */
