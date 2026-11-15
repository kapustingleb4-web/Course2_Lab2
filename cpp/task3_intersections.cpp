// Задание 3, вариант 4. Максимальное количество пересечений.
// Среди подмножеств найти два с наибольшим числом общих элементов.
// Каждое подмножество хранится в массиве ЛР1 (порядок для вывода) и во множестве (проверка за O(1)).
// Сложность O(k^2 * m).
#include "MyArray.hpp"
#include "MySet.hpp"
#include <iostream>
#include <string>
#include <deque>
#include <vector>

struct Subset {
    MyArray items;
    MySet set;
};

// Разбор строки вида {{1, 2, 3}, {2, 3, 4}, {5, 6}}
std::deque<Subset> parse(const std::string& s) {
    std::deque<Subset> res;  // deque: MyArray нельзя копировать, а deque не переносит элементы при добавлении
    int depth = 0;
    std::string num;
    for (char c : s) {
        if (c == '{') {
            depth++;
            if (depth == 2) res.emplace_back();
        } else if (c == '}' || c == ',' || c == ' ') {
            if (!num.empty() && !res.empty()) {
                if (res.back().set.add(num)) res.back().items.push_back(num);
                num.clear();
            }
            if (c == '}') depth--;
        } else if (isdigit((unsigned char)c)) {
            num += c;
        }
    }
    return res;
}

void printSubset(const Subset& s) {
    std::cout << "{";
    for (int i = 0; i < s.items.length(); ++i) std::cout << (i ? ", " : "") << s.items.get(i);
    std::cout << "}";
}

int main() {
    std::string line;
    std::cout << "Ввод: ";
    std::getline(std::cin, line);
    std::deque<Subset> subs = parse(line);
    if (subs.size() < 2) { std::cout << "Нужно хотя бы два подмножества" << std::endl; return 0; }

    int bestI = 0, bestJ = 1, best = -1;
    std::vector<std::string> bestCommon;
    for (int i = 0; i < (int)subs.size(); ++i) {
        for (int j = i + 1; j < (int)subs.size(); ++j) {
            std::vector<std::string> common;
            for (int t = 0; t < subs[i].items.length(); ++t)
                if (subs[j].set.contains(subs[i].items.get(t))) common.push_back(subs[i].items.get(t));
            if ((int)common.size() >= best) {  // при равенстве берём последнюю пару (как в примере)
                best = common.size(); bestI = i; bestJ = j; bestCommon = common;
            }
        }
    }

    std::cout << "Вывод: {";
    printSubset(subs[bestI]);
    std::cout << ", ";
    printSubset(subs[bestJ]);
    std::cout << "}, количество общих элементов: " << best;
    if (best > 0) {
        std::cout << " (";
        for (int t = 0; t < (int)bestCommon.size(); ++t) std::cout << (t ? ", " : "") << bestCommon[t];
        std::cout << ")";
    }
    std::cout << std::endl;
    return 0;
}
