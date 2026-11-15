#ifndef LEVELAVLTREE_HPP
#define LEVELAVLTREE_HPP
#include <vector>

// АВЛ-дерево как в ЛР1, но с методом getLevels() - значения узлов по уровням (обход в ширину).
// В классе AVLTree из ЛР1 корень и узлы закрыты (private), поэтому для задания 5 дерево
// с доступом к уровням сделано отдельным классом.
class LevelAVLTree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;

    int height(Node* n) const;
    int getBalance(Node* n) const;
    void updateHeight(Node* n);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* balance(Node* n);
    Node* insert(Node* n, int key);
    void clear(Node* n);
    void printTreeRec(Node* n, int space) const;

public:
    LevelAVLTree();
    ~LevelAVLTree();
    LevelAVLTree(const LevelAVLTree&) = delete;
    LevelAVLTree& operator=(const LevelAVLTree&) = delete;

    void insert(int key);
    std::vector<std::vector<int>> getLevels() const;
    void printTree() const;
};
#endif
