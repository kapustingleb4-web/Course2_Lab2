// Задание 4, вариант 4. Пирамида.
// Блок можно ставить только на блок строго большей ширины, значит все ширины в пирамиде разные,
// и для каждой ширины выгодно взять самый высокий блок.
// Ответ = сумма максимальных высот по всем различным ширинам.
// Хеш-таблица: ширина -> максимальная высота. Сложность O(N).
#include "MyHashTable.hpp"
#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;
    MyHashTable best;
    for (int i = 0; i < n; ++i) {
        long long w, h;
        std::cin >> w >> h;
        std::string key = std::to_string(w), cur;
        if (!best.get(key, cur) || std::stoll(cur) < h) best.insert(key, std::to_string(h));
    }
    long long total = 0;
    for (auto& v : best.values()) total += std::stoll(v);
    std::cout << total << std::endl;
    return 0;
}
