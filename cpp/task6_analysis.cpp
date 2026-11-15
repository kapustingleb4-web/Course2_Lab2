// Задание 6.1, вариант 2. Метод цепочек и хеширование кукушки.
// Демонстрация на маленьких таблицах + эмпирический анализ времени операций для разных N.
#include "HashTables.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

using Clock = chrono::steady_clock;

double nsPerOp(Clock::time_point a, Clock::time_point b, int n) {
    return (double)chrono::duration_cast<chrono::nanoseconds>(b - a).count() / n;
}

void demo() {
    cout << "=== Демонстрация (маленькие таблицы) ===" << endl;
    ChainHashTable chain(5, hashDjb2, false);
    CuckooHashTable cuckoo(5);
    vector<string> keys = {"apple", "banana", "cherry", "date", "elder", "fig", "grape"};
    for (auto& k : keys) {
        chain.insert(k, to_string(k.size()));
        cuckoo.insert(k, to_string(k.size()));
    }
    cout << "Метод цепочек (5 корзин, без расширения):" << endl;
    chain.print();
    cout << "Кукушка (две таблицы по " << cuckoo.getCapacity() << ", вытеснений: " << cuckoo.kicks
              << ", перестроений: " << cuckoo.rehashes << "):" << endl;
    cuckoo.print();
    string v;
    cout << "find(cherry): chain=" << (chain.find("cherry", v) ? v : "нет")
              << ", cuckoo=" << (cuckoo.find("cherry", v) ? v : "нет") << endl;
    chain.remove("cherry");
    cuckoo.remove("cherry");
    cout << "после remove(cherry): chain=" << (chain.find("cherry", v) ? "есть" : "нет")
              << ", cuckoo=" << (cuckoo.find("cherry", v) ? "есть" : "нет") << endl << endl;
}

void analysis() {
    cout << "=== Эмпирический анализ (время в нс на операцию) ===" << endl;
    cout << left << setw(9) << "N" << setw(9) << "метод" << right
              << setw(9) << "insert" << setw(9) << "find+" << setw(9) << "find-"
              << setw(9) << "remove" << "   структура" << endl;
    mt19937_64 rng(42);
    for (int n : {1000, 10000, 100000}) {
        vector<string> in(n), out(n);
        for (int i = 0; i < n; ++i) {
            in[i] = "key" + to_string(rng());
            out[i] = "miss" + to_string(rng());
        }
        string v;
        {
            ChainHashTable t;
            auto a = Clock::now();
            for (auto& k : in) t.insert(k, k);
            auto b = Clock::now();
            for (auto& k : in) t.find(k, v);
            auto c = Clock::now();
            for (auto& k : out) t.find(k, v);
            auto d = Clock::now();
            int mx, em; double avg;
            t.stats(mx, avg, em);
            for (auto& k : in) t.remove(k);
            auto e = Clock::now();
            cout << left << setw(9) << n << setw(9) << "chain" << right << fixed << setprecision(1)
                      << setw(9) << nsPerOp(a, b, n) << setw(9) << nsPerOp(b, c, n) << setw(9) << nsPerOp(c, d, n)
                      << setw(9) << nsPerOp(d, e, n) << "   max цепочка=" << mx << ", ср. цепочка=" << setprecision(2) << avg << endl;
        }
        {
            CuckooHashTable t;
            auto a = Clock::now();
            for (auto& k : in) t.insert(k, k);
            auto b = Clock::now();
            for (auto& k : in) t.find(k, v);
            auto c = Clock::now();
            for (auto& k : out) t.find(k, v);
            auto d = Clock::now();
            long long kicks = t.kicks;
            int rh = t.rehashes;
            for (auto& k : in) t.remove(k);
            auto e = Clock::now();
            cout << left << setw(9) << n << setw(9) << "cuckoo" << right << fixed << setprecision(1)
                      << setw(9) << nsPerOp(a, b, n) << setw(9) << nsPerOp(b, c, n) << setw(9) << nsPerOp(c, d, n)
                      << setw(9) << nsPerOp(d, e, n) << "   вытеснений/вставку=" << setprecision(2) << (double)kicks / n
                      << ", перестроений=" << rh << endl;
        }
    }
}

int main() {
    demo();
    analysis();
    return 0;
}
