// Задание 6.1, вариант 2. Метод цепочек и хеширование кукушки.
// Демонстрация на маленьких таблицах + эмпирический анализ времени операций для разных N.
#include "HashTables.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

double nsPerOp(Clock::time_point a, Clock::time_point b, int n) {
    return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(b - a).count() / n;
}

void demo() {
    std::cout << "=== Демонстрация (маленькие таблицы) ===" << std::endl;
    ChainHashTable chain(5, hashDjb2, false);
    CuckooHashTable cuckoo(5);
    std::vector<std::string> keys = {"apple", "banana", "cherry", "date", "elder", "fig", "grape"};
    for (auto& k : keys) {
        chain.insert(k, std::to_string(k.size()));
        cuckoo.insert(k, std::to_string(k.size()));
    }
    std::cout << "Метод цепочек (5 корзин, без расширения):" << std::endl;
    chain.print();
    std::cout << "Кукушка (две таблицы по " << cuckoo.getCapacity() << ", вытеснений: " << cuckoo.kicks
              << ", перестроений: " << cuckoo.rehashes << "):" << std::endl;
    cuckoo.print();
    std::string v;
    std::cout << "find(cherry): chain=" << (chain.find("cherry", v) ? v : "нет")
              << ", cuckoo=" << (cuckoo.find("cherry", v) ? v : "нет") << std::endl;
    chain.remove("cherry");
    cuckoo.remove("cherry");
    std::cout << "после remove(cherry): chain=" << (chain.find("cherry", v) ? "есть" : "нет")
              << ", cuckoo=" << (cuckoo.find("cherry", v) ? "есть" : "нет") << std::endl << std::endl;
}

void analysis() {
    std::cout << "=== Эмпирический анализ (время в нс на операцию) ===" << std::endl;
    std::cout << std::left << std::setw(9) << "N" << std::setw(9) << "метод" << std::right
              << std::setw(9) << "insert" << std::setw(9) << "find+" << std::setw(9) << "find-"
              << std::setw(9) << "remove" << "   структура" << std::endl;
    std::mt19937_64 rng(42);
    for (int n : {1000, 10000, 100000}) {
        std::vector<std::string> in(n), out(n);
        for (int i = 0; i < n; ++i) {
            in[i] = "key" + std::to_string(rng());
            out[i] = "miss" + std::to_string(rng());
        }
        std::string v;
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
            std::cout << std::left << std::setw(9) << n << std::setw(9) << "chain" << std::right << std::fixed << std::setprecision(1)
                      << std::setw(9) << nsPerOp(a, b, n) << std::setw(9) << nsPerOp(b, c, n) << std::setw(9) << nsPerOp(c, d, n)
                      << std::setw(9) << nsPerOp(d, e, n) << "   max цепочка=" << mx << ", ср. цепочка=" << std::setprecision(2) << avg << std::endl;
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
            std::cout << std::left << std::setw(9) << n << std::setw(9) << "cuckoo" << std::right << std::fixed << std::setprecision(1)
                      << std::setw(9) << nsPerOp(a, b, n) << std::setw(9) << nsPerOp(b, c, n) << std::setw(9) << nsPerOp(c, d, n)
                      << std::setw(9) << nsPerOp(d, e, n) << "   вытеснений/вставку=" << std::setprecision(2) << (double)kicks / n
                      << ", перестроений=" << rh << std::endl;
        }
    }
}

int main() {
    demo();
    analysis();
    return 0;
}
