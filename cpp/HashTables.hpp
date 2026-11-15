#ifndef HASHTABLES_HPP
#define HASHTABLES_HPP
#include <string>

using namespace std;

// Задание 6.1, вариант 2: метод цепочек и хеширование кукушки.
// Хеш-функция передаётся параметром, чтобы в 6.2 подключить метод свёртки.
typedef unsigned long long (*HashFunc)(const string&);

unsigned long long hashDjb2(const string& s);
unsigned long long hashFnv(const string& s, unsigned long long seed);

// ---------- Метод цепочек ----------
class ChainHashTable {
private:
    struct Node {
        string key;
        string value;
        Node* next;
        Node(const string& k, const string& v, Node* n) : key(k), value(v), next(n) {}
    };
    Node** buckets;
    int capacity;
    int size;
    HashFunc func;
    bool autoResize;
    void rehash();

public:
    ChainHashTable(int cap = 16, HashFunc f = hashDjb2, bool resize = true);
    ~ChainHashTable();
    ChainHashTable(const ChainHashTable&) = delete;
    ChainHashTable& operator=(const ChainHashTable&) = delete;

    int index(const string& key) const;
    void insert(const string& key, const string& value);
    bool find(const string& key, string& out) const;
    bool remove(const string& key);
    void stats(int& maxChain, double& avgChain, int& empty) const;
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    void print() const;
};

// ---------- Хеширование кукушки ----------
class CuckooHashTable {
private:
    struct Slot {
        string key;
        string value;
        bool used = false;
    };
    Slot* t1;
    Slot* t2;
    int capacity;
    int size;
    unsigned long long seed;

    int h1(const string& key) const;
    int h2(const string& key) const;
    bool place(string& key, string& value);
    void rehash(int newCap);

public:
    long long kicks = 0;     // сколько раз вытесняли элемент
    int rehashes = 0;        // сколько раз перестраивали таблицы

    CuckooHashTable(int cap = 16);
    ~CuckooHashTable();
    CuckooHashTable(const CuckooHashTable&) = delete;
    CuckooHashTable& operator=(const CuckooHashTable&) = delete;

    void insert(const string& key, const string& value);
    bool find(const string& key, string& out) const;
    bool remove(const string& key);
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    void print() const;
};
#endif
