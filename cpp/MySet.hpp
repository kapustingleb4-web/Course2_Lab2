#ifndef MYSET_HPP
#define MYSET_HPP
#include "MyHashTable.hpp"

using namespace std;

// АТД "Множество" на основе хеш-таблицы: элемент хранится как ключ.
class MySet {
private:
    MyHashTable table;

public:
    bool add(const string& val);       // false, если уже есть
    bool remove(const string& val);    // false, если нет
    bool contains(const string& val) const;
    int getSize() const;
    void print() const;
    vector<string> getData() const;
};
#endif
