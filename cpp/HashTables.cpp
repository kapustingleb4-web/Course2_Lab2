#include "HashTables.hpp"
#include <iostream>
#include <utility>

using namespace std;

unsigned long long hashDjb2(const string& s) {
    unsigned long long h = 5381;
    for (unsigned char c : s) h = h * 33 + c;
    return h;
}

unsigned long long hashFnv(const string& s, unsigned long long seed) {
    unsigned long long h = 1469598103934665603ULL ^ seed;
    for (unsigned char c : s) {
        h ^= c;
        h *= 1099511628211ULL;
    }
    return h ^ (h >> 29);
}

// ================= Метод цепочек =================
ChainHashTable::ChainHashTable(int cap, HashFunc f, bool resize)
    : capacity(cap < 1 ? 1 : cap), size(0), func(f), autoResize(resize) {
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) buckets[i] = nullptr;
}

ChainHashTable::~ChainHashTable() {
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

int ChainHashTable::index(const string& key) const { return func(key) % capacity; }

void ChainHashTable::rehash() {
    Node** old = buckets;
    int oldCap = capacity;
    capacity *= 2;
    buckets = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) buckets[i] = nullptr;
    for (int i = 0; i < oldCap; ++i) {
        Node* cur = old[i];
        while (cur) {
            Node* next = cur->next;
            int idx = index(cur->key);
            cur->next = buckets[idx];
            buckets[idx] = cur;
            cur = next;
        }
    }
    delete[] old;
}

void ChainHashTable::insert(const string& key, const string& value) {
    int idx = index(key);
    for (Node* cur = buckets[idx]; cur; cur = cur->next)
        if (cur->key == key) { cur->value = value; return; }
    buckets[idx] = new Node(key, value, buckets[idx]);
    size++;
    if (autoResize && size > capacity) rehash();
}

bool ChainHashTable::find(const string& key, string& out) const {
    for (Node* cur = buckets[index(key)]; cur; cur = cur->next)
        if (cur->key == key) { out = cur->value; return true; }
    return false;
}

bool ChainHashTable::remove(const string& key) {
    int idx = index(key);
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

void ChainHashTable::stats(int& maxChain, double& avgChain, int& empty) const {
    maxChain = 0; empty = 0;
    int nonEmpty = 0;
    for (int i = 0; i < capacity; ++i) {
        int len = 0;
        for (Node* cur = buckets[i]; cur; cur = cur->next) len++;
        if (len == 0) empty++; else nonEmpty++;
        if (len > maxChain) maxChain = len;
    }
    avgChain = nonEmpty ? (double)size / nonEmpty : 0.0;
}

void ChainHashTable::print() const {
    for (int i = 0; i < capacity; ++i) {
        cout << "[" << i << "]";
        for (Node* cur = buckets[i]; cur; cur = cur->next) cout << " -> (" << cur->key << ": " << cur->value << ")";
        cout << endl;
    }
}

// ================= Хеширование кукушки =================
CuckooHashTable::CuckooHashTable(int cap) : capacity(cap < 2 ? 2 : cap), size(0), seed(0x9E3779B97F4A7C15ULL) {
    t1 = new Slot[capacity];
    t2 = new Slot[capacity];
}

CuckooHashTable::~CuckooHashTable() {
    delete[] t1;
    delete[] t2;
}

int CuckooHashTable::h1(const string& key) const {
    unsigned long long h = hashDjb2(key) ^ (seed * 0x2545F4914F6CDD1DULL);
    h ^= h >> 31;
    return h % capacity;
}

int CuckooHashTable::h2(const string& key) const { return hashFnv(key, seed) % capacity; }

bool CuckooHashTable::find(const string& key, string& out) const {
    const Slot& a = t1[h1(key)];
    if (a.used && a.key == key) { out = a.value; return true; }
    const Slot& b = t2[h2(key)];
    if (b.used && b.key == key) { out = b.value; return true; }
    return false;  // максимум 2 проверки - O(1) в худшем случае
}

// Кладём элемент; если место занято - вытесняем жильца в его другую таблицу.
// Если цепочка вытеснений слишком длинная (цикл), возвращаем false.
bool CuckooHashTable::place(string& key, string& value) {
    int maxLoop = 32;
    for (int c = capacity; c > 1; c /= 2) maxLoop++;
    for (int i = 0; i < maxLoop; ++i) {
        Slot& a = t1[h1(key)];
        if (!a.used) { a.key = key; a.value = value; a.used = true; return true; }
        swap(key, a.key);
        swap(value, a.value);
        kicks++;
        Slot& b = t2[h2(key)];
        if (!b.used) { b.key = key; b.value = value; b.used = true; return true; }
        swap(key, b.key);
        swap(value, b.value);
        kicks++;
    }
    return false;
}

// Перестроение: новые хеш-функции (новый seed), при неудаче - таблицы больше
void CuckooHashTable::rehash(int newCap) {
    Slot* o1 = t1;
    Slot* o2 = t2;
    int oldCap = capacity;
    rehashes++;
    while (true) {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        capacity = newCap;
        t1 = new Slot[capacity];
        t2 = new Slot[capacity];
        size = 0;
        bool ok = true;
        for (int i = 0; i < oldCap && ok; ++i) {
            for (Slot* s : {&o1[i], &o2[i]}) {
                if (!s->used) continue;
                string k = s->key, v = s->value;
                if (!place(k, v)) { ok = false; break; }
                size++;
            }
        }
        if (ok) break;
        delete[] t1;
        delete[] t2;
        newCap *= 2;
    }
    delete[] o1;
    delete[] o2;
}

void CuckooHashTable::insert(const string& key, const string& value) {
    Slot& a = t1[h1(key)];
    if (a.used && a.key == key) { a.value = value; return; }
    Slot& b = t2[h2(key)];
    if (b.used && b.key == key) { b.value = value; return; }
    if ((size + 1) * 2 > capacity) rehash(capacity * 2);  // держим заполнение <= 50%
    string k = key, v = value;
    while (!place(k, v)) {
        rehash(capacity);  // не удалось - новые функции, вставляем выпавший элемент снова
    }
    size++;
}

bool CuckooHashTable::remove(const string& key) {
    Slot& a = t1[h1(key)];
    if (a.used && a.key == key) { a.used = false; size--; return true; }
    Slot& b = t2[h2(key)];
    if (b.used && b.key == key) { b.used = false; size--; return true; }
    return false;
}

void CuckooHashTable::print() const {
    for (int i = 0; i < capacity; ++i) {
        cout << "[" << i << "]  T1: " << (t1[i].used ? t1[i].key + "=" + t1[i].value : "-")
                  << "   T2: " << (t2[i].used ? t2[i].key + "=" + t2[i].value : "-") << endl;
    }
}
