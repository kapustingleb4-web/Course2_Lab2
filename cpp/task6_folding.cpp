// Задание 6.2, вариант 1. Хеш-функция методом свёртки.
// Ключ делится на части по 3 цифры слева направо, части складываются:
// 523456795 -> 523 + 456 + 795 = 1774, индекс = сумма % размер таблицы.
// Для нечисловых ключей части - группы по 3 символа (коды символов как число).
#include "HashTables.hpp"
#include <iostream>
#include <string>

bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}

unsigned long long fold(const std::string& key, std::string& expr) {
    unsigned long long sum = 0;
    expr.clear();
    bool digits = isNumber(key);
    for (size_t i = 0; i < key.size(); i += 3) {
        std::string part = key.substr(i, 3);
        unsigned long long v = 0;
        if (digits) v = std::stoull(part);
        else for (unsigned char c : part) v = v * 256 + c;
        sum += v;
        if (!expr.empty()) expr += "+";
        expr += digits ? part : std::to_string(v);
    }
    return sum;
}

unsigned long long foldHash(const std::string& key) {
    std::string expr;
    return fold(key, expr);
}

int main() {
    const int tableSize = 11;
    ChainHashTable table(tableSize, foldHash, false);  // свёртка - хеш-функция таблицы из 6.1

    std::cout << "Введите ключи через пробел (Ctrl+D - конец):" << std::endl;
    std::string key;
    while (std::cin >> key) {
        std::string expr;
        unsigned long long h = fold(key, expr);
        std::cout << "Ввод: " << key << std::endl;
        std::cout << "Вывод: " << expr << " = " << h << "." << std::endl;
        std::cout << "  индекс в таблице размера " << tableSize << ": " << h << " mod " << tableSize
                  << " = " << table.index(key) << std::endl;
        table.insert(key, std::to_string(h));
    }
    std::cout << std::endl << "Хеш-таблица (метод цепочек + свёртка):" << std::endl;
    table.print();
    return 0;
}
