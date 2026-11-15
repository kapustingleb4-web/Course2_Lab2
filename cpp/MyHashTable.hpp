#ifndef MYHASHTABLE_HPP
#define MYHASHTABLE_HPP
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Хеш-таблица (метод цепочек), ключ и значение - строки.
// Используется в заданиях 2, 3, 4 (множество, пирамида).
class MyHashTable {
private:
    struct Node {
        string key;
        string value;
        Node* next;
        Node(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
    };
    Node** buckets;
    int capacity;
    int size;

    int hash(const string& key) const;
    void rehash();

public:
    MyHashTable(int cap = 16);
    ~MyHashTable();
    MyHashTable(const MyHashTable& other);
    MyHashTable& operator=(const MyHashTable& other);

    void insert(const string& key, const string& value);
    bool get(const string& key, string& out) const;
    bool contains(const string& key) const;
    bool remove(const string& key);
    int getSize() const;
    void print() const;

    vector<string> keys() const;
    vector<string> values() const;
};
#endif
