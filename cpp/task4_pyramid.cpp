// Задание 4, вариант 4. Пирамида.
// Блок можно ставить только на блок строго большей ширины, значит все ширины в пирамиде разные,
// и для каждой ширины выгодно взять самый высокий блок.
// Ответ = сумма максимальных высот по всем различным ширинам.
// Хеш-таблица: ширина -> максимальная высота. Сложность O(N).
#include "MyHashTable.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    cin >> n;
    MyHashTable best;
    for (int i = 0; i < n; ++i) {
        long long w, h;
        cin >> w >> h;
        string key = to_string(w), cur;
        if (!best.get(key, cur) || stoll(cur) < h) best.insert(key, to_string(h));
    }
    long long total = 0;
    for (auto& v : best.values()) total += stoll(v);
    cout << total << endl;
    return 0;
}
