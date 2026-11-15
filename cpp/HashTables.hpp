#ifndef HASHTABLES_HPP
#define HASHTABLES_HPP
#include <string>

// Задание 6.1, вариант 2: метод цепочек и хеширование кукушки.
// Хеш-функция передаётся параметром, чтобы в 6.2 подключить метод свёртки.
typedef unsigned long long (*HashFunc)(const std::string&);

unsigned long long hashDjb2(const std::string& s);
unsigned long long hashFnv(const std::string& s, unsigned long long seed);

// ---------- Метод цепочек ----------
class ChainHashTable {
private:
    struct Node {
        std::string key;
        std::string value;
        Node* next;
        Node(const std::string& k, const std::string& v, Node* n) : key(k), value(v), next(n) {}
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

    int index(const std::string& key) const;
    void insert(const std::string& key, const std::string& value);
    bool find(const std::string& key, std::string& out) const;
    bool remove(const std::string& key);
    void stats(int& maxChain, double& avgChain, int& empty) const;
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    void print() const;
};

// ---------- Хеширование кукушки ----------
class CuckooHashTable {
private:
    struct Slot {
        std::string key;
        std::string value;
        bool used = false;
    };
    Slot* t1;
    Slot* t2;
    int capacity;
    int size;
    unsigned long long seed;

    int h1(const std::string& key) const;
    int h2(const std::string& key) const;
    bool place(std::string& key, std::string& value);
    void rehash(int newCap);

public:
    long long kicks = 0;     // сколько раз вытесняли элемент
    int rehashes = 0;        // сколько раз перестраивали таблицы

    CuckooHashTable(int cap = 16);
    ~CuckooHashTable();
    CuckooHashTable(const CuckooHashTable&) = delete;
    CuckooHashTable& operator=(const CuckooHashTable&) = delete;

    void insert(const std::string& key, const std::string& value);
    bool find(const std::string& key, std::string& out) const;
    bool remove(const std::string& key);
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    void print() const;
};
#endif
