#ifndef MYSET_HPP
#define MYSET_HPP
#include "MyHashTable.hpp"

// АТД "Множество" на основе хеш-таблицы: элемент хранится как ключ.
class MySet {
private:
    MyHashTable table;

public:
    bool add(const std::string& val);       // false, если уже есть
    bool remove(const std::string& val);    // false, если нет
    bool contains(const std::string& val) const;
    int getSize() const;
    void print() const;
    std::vector<std::string> getData() const;
};
#endif
