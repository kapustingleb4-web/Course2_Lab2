#include "LevelAVLTree.hpp"
#include <algorithm>
#include <iostream>

LevelAVLTree::LevelAVLTree() : root(nullptr) {}
LevelAVLTree::~LevelAVLTree() { clear(root); }

void LevelAVLTree::clear(Node* n) {
    if (!n) return;
    clear(n->left);
    clear(n->right);
    delete n;
}

int LevelAVLTree::height(Node* n) const { return n ? n->height : 0; }

int LevelAVLTree::getBalance(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }

void LevelAVLTree::updateHeight(Node* n) { n->height = 1 + std::max(height(n->left), height(n->right)); }

LevelAVLTree::Node* LevelAVLTree::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

LevelAVLTree::Node* LevelAVLTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

LevelAVLTree::Node* LevelAVLTree::balance(Node* n) {
    updateHeight(n);
    int bf = getBalance(n);
    if (bf > 1) {
        if (getBalance(n->left) < 0) n->left = leftRotate(n->left);    // LR
        return rightRotate(n);                                           // LL
    }
    if (bf < -1) {
        if (getBalance(n->right) > 0) n->right = rightRotate(n->right); // RL
        return leftRotate(n);                                            // RR
    }
    return n;
}

LevelAVLTree::Node* LevelAVLTree::insert(Node* n, int key) {
    if (!n) return new Node(key);
    if (key < n->key) n->left = insert(n->left, key);
    else if (key > n->key) n->right = insert(n->right, key);
    else return n;  // дубликаты не вставляем
    return balance(n);
}

void LevelAVLTree::insert(int key) { root = insert(root, key); }

// Обход в ширину: берём текущий уровень, собираем детей в следующий уровень
std::vector<std::vector<int>> LevelAVLTree::getLevels() const {
    std::vector<std::vector<int>> res;
    std::vector<Node*> level;
    if (root) level.push_back(root);
    while (!level.empty()) {
        std::vector<Node*> next;
        res.emplace_back();
        for (Node* n : level) {
            res.back().push_back(n->key);
            if (n->left) next.push_back(n->left);
            if (n->right) next.push_back(n->right);
        }
        level = next;
    }
    return res;
}

void LevelAVLTree::printTreeRec(Node* n, int space) const {
    if (!n) return;
    space += 5;
    printTreeRec(n->right, space);
    std::cout << std::endl;
    for (int i = 5; i < space; ++i) std::cout << " ";
    std::cout << n->key;
    printTreeRec(n->left, space);
}

void LevelAVLTree::printTree() const {
    printTreeRec(root, 0);
    std::cout << std::endl;
}
