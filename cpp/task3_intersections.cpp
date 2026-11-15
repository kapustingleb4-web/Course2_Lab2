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

using namespace std;

struct Subset {
    MyArray items;
    MySet set;
};

// Разбор строки вида {{1, 2, 3}, {2, 3, 4}, {5, 6}}
deque<Subset> parse(const string& s) {
    deque<Subset> res;  // deque: MyArray нельзя копировать, а deque не переносит элементы при добавлении
    int depth = 0;
    string num;
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
    cout << "{";
    for (int i = 0; i < s.items.length(); ++i) cout << (i ? ", " : "") << s.items.get(i);
    cout << "}";
}

int main() {
    string line;
    cout << "Ввод: ";
    getline(cin, line);
    deque<Subset> subs = parse(line);
    if (subs.size() < 2) { cout << "Нужно хотя бы два подмножества" << endl; return 0; }

    int bestI = 0, bestJ = 1, best = -1;
    vector<string> bestCommon;
    for (int i = 0; i < (int)subs.size(); ++i) {
        for (int j = i + 1; j < (int)subs.size(); ++j) {
            vector<string> common;
            for (int t = 0; t < subs[i].items.length(); ++t)
                if (subs[j].set.contains(subs[i].items.get(t))) common.push_back(subs[i].items.get(t));
            if ((int)common.size() >= best) {  // при равенстве берём последнюю пару (как в примере)
                best = common.size(); bestI = i; bestJ = j; bestCommon = common;
            }
        }
    }

    cout << "Вывод: {";
    printSubset(subs[bestI]);
    cout << ", ";
    printSubset(subs[bestJ]);
    cout << "}, количество общих элементов: " << best;
    if (best > 0) {
        cout << " (";
        for (int t = 0; t < (int)bestCommon.size(); ++t) cout << (t ? ", " : "") << bestCommon[t];
        cout << ")";
    }
    cout << endl;
    return 0;
}
