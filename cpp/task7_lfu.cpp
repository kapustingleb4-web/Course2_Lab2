// Задание 7, вариант 2. LFU-кэш (Least Frequently Used).
//   keys  : ключ -> узел двусвязного списка
//   freqs : частота -> двусвязный список узлов (в хвосте - самые свежие)
//   minFreq - минимальная частота.
// При переполнении вытесняется голова списка minFreq: самый редкий,
// а среди равных - тот, к которому дольше всего не обращались. GET и SET - O(1).
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class LFUCache {
private:
    struct Node {
        int key, value, freq;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), freq(1), prev(nullptr), next(nullptr) {}
    };
    // Двусвязный список узлов (как DoublyList из ЛР1, но с доступом к узлу)
    struct FreqList {
        Node* head = nullptr;
        Node* tail = nullptr;
        int size = 0;
        void push_back(Node* n) {
            n->prev = tail; n->next = nullptr;
            if (tail) tail->next = n; else head = n;
            tail = n;
            size++;
        }
        void remove(Node* n) {  // O(1): узел известен
            if (n->prev) n->prev->next = n->next; else head = n->next;
            if (n->next) n->next->prev = n->prev; else tail = n->prev;
            size--;
        }
    };

    int capacity;
    int minFreq = 0;
    std::unordered_map<int, Node*> keys;
    std::unordered_map<int, FreqList> freqs;

    void touch(Node* n) {  // обращение: переносим узел в список freq+1
        freqs[n->freq].remove(n);
        if (freqs[n->freq].size == 0 && minFreq == n->freq) minFreq++;
        n->freq++;
        freqs[n->freq].push_back(n);
    }

public:
    LFUCache(int cap) : capacity(cap) {}
    ~LFUCache() { for (auto& [k, n] : keys) delete n; }

    int get(int key) {
        if (!keys.count(key)) return -1;
        Node* n = keys[key];
        touch(n);
        return n->value;
    }

    void set(int key, int value) {
        if (capacity <= 0) return;
        if (keys.count(key)) {
            keys[key]->value = value;
            touch(keys[key]);
            return;
        }
        if ((int)keys.size() == capacity) {  // вытесняем
            Node* victim = freqs[minFreq].head;
            freqs[minFreq].remove(victim);
            keys.erase(victim->key);
            delete victim;
        }
        Node* n = new Node(key, value);
        keys[key] = n;
        freqs[1].push_back(n);
        minFreq = 1;
    }
};

// Ввод: cap Q, затем Q запросов "SET x y" / "GET x"
int main() {
    int cap, q;
    std::cout << "Введите cap и Q, затем запросы:" << std::endl;
    std::cin >> cap >> q;
    LFUCache cache(cap);
    std::string out;
    for (int i = 0; i < q; ++i) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "SET") {
            int x, y;
            std::cin >> x >> y;
            cache.set(x, y);
        } else if (cmd == "GET") {
            int x;
            std::cin >> x;
            out += (out.empty() ? "" : " ") + std::to_string(cache.get(x));
        }
    }
    std::cout << "Вывод: " << out << std::endl;
    return 0;
}
