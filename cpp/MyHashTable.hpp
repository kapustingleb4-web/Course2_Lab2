#ifndef MYHASHTABLE_HPP
#define MYHASHTABLE_HPP
#include <string>
#include <vector>
#include <iostream>

// Хеш-таблица (метод цепочек), ключ и значение - строки.
// Используется в заданиях 2, 3, 4 (множество, пирамида).
class MyHashTable {
private:
    struct Node {
        std::string key;
        std::string value;
        Node* next;
        Node(const std::string& k, const std::string& v) : key(k), value(v), next(nullptr) {}
    };
    Node** buckets;
    int capacity;
    int size;

    int hash(const std::string& key) const;
    void rehash();

public:
    MyHashTable(int cap = 16);
    ~MyHashTable();
    MyHashTable(const MyHashTable& other);
    MyHashTable& operator=(const MyHashTable& other);

    void insert(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& out) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);
    int getSize() const;
    void print() const;

    std::vector<std::string> keys() const;
    std::vector<std::string> values() const;
};
#endif
