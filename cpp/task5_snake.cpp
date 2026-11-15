// Задание 5, вариант 2. Чтение дерева "змейкой".
// Числа (до 0) вставляются в АВЛ-дерево, затем уровни выводятся попеременно
// слева направо и справа налево. Каждый узел просматривается один раз - O(n).
#include "LevelAVLTree.hpp"
#include <iostream>

int main() {
    LevelAVLTree tree;
    int x;
    std::cout << "Введите числа (0 - конец ввода): ";
    while (std::cin >> x && x != 0) tree.insert(x);

    std::cout << "Дерево:";
    tree.printTree();

    std::cout << "Змейка: ";
    bool leftToRight = true, first = true;
    for (auto& level : tree.getLevels()) {
        int n = level.size();
        for (int i = 0; i < n; ++i) {
            int v = leftToRight ? level[i] : level[n - 1 - i];
            std::cout << (first ? "" : " - ") << v;
            first = false;
        }
        leftToRight = !leftToRight;
    }
    std::cout << std::endl;
    return 0;
}
