#include "MyHashTable.hpp"

using namespace std;

MyHashTable::MyHashTable(int cap) : capacity(cap < 1 ? 1 : cap), size(0) {
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) buckets[i] = nullptr;
}

MyHashTable::~MyHashTable() {
    for (int i = 0; i < capacity; ++i) {
        Node* cur = buckets[i];
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    delete[] buckets;
}

MyHashTable::MyHashTable(const MyHashTable& other) : capacity(other.capacity), size(0) {
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) buckets[i] = nullptr;
    for (int i = 0; i < other.capacity; ++i)
        for (Node* cur = other.buckets[i]; cur; cur = cur->next) insert(cur->key, cur->value);
}

MyHashTable& MyHashTable::operator=(const MyHashTable& other) {
    if (this == &other) return *this;
    MyHashTable tmp(other);
    swap(buckets, tmp.buckets);
    swap(capacity, tmp.capacity);
    swap(size, tmp.size);
    return *this;
}

// djb2: h = h * 33 + c, затем остаток от деления на размер таблицы
int MyHashTable::hash(const string& key) const {
    unsigned long long h = 5381;
    for (unsigned char c : key) h = h * 33 + c;
    return static_cast<int>(h % capacity);
}

void MyHashTable::rehash() {
    int oldCap = capacity;
    Node** old = buckets;
    capacity *= 2;
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) buckets[i] = nullptr;
    for (int i = 0; i < oldCap; ++i) {
        Node* cur = old[i];
        while (cur) {
            Node* next = cur->next;
            int idx = hash(cur->key);
            cur->next = buckets[idx];
            buckets[idx] = cur;
            cur = next;
        }
    }
    delete[] old;
}

void MyHashTable::insert(const string& key, const string& value) {
    int idx = hash(key);
    for (Node* cur = buckets[idx]; cur; cur = cur->next) {
        if (cur->key == key) { cur->value = value; return; }
    }
    Node* n = new Node(key, value);
    n->next = buckets[idx];
    buckets[idx] = n;
    size++;
    if (size * 4 > capacity * 3) rehash();  // коэффициент заполнения > 0.75
}

bool MyHashTable::get(const string& key, string& out) const {
    for (Node* cur = buckets[hash(key)]; cur; cur = cur->next) {
        if (cur->key == key) { out = cur->value; return true; }
    }
    return false;
}

bool MyHashTable::contains(const string& key) const {
    string tmp;
    return get(key, tmp);
}

bool MyHashTable::remove(const string& key) {
    int idx = hash(key);
    Node* cur = buckets[idx];
    Node* prev = nullptr;
    while (cur) {
        if (cur->key == key) {
            if (prev) prev->next = cur->next; else buckets[idx] = cur->next;
            delete cur;
            size--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

int MyHashTable::getSize() const { return size; }

void MyHashTable::print() const {
    cout << "{ ";
    for (int i = 0; i < capacity; ++i)
        for (Node* cur = buckets[i]; cur; cur = cur->next) cout << cur->key << ":" << cur->value << " ";
    cout << "}";
}

vector<string> MyHashTable::keys() const {
    vector<string> res;
    for (int i = 0; i < capacity; ++i)
        for (Node* cur = buckets[i]; cur; cur = cur->next) res.push_back(cur->key);
    return res;
}

vector<string> MyHashTable::values() const {
    vector<string> res;
    for (int i = 0; i < capacity; ++i)
        for (Node* cur = buckets[i]; cur; cur = cur->next) res.push_back(cur->value);
    return res;
}
