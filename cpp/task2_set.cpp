// Задание 2. АТД "Множество": SETADD, SETDEL, SET_AT.
//   ./set --file sets.json --query 'SETADD myset 5'
// Множества хранятся в JSON так же, как в ЛР1 (json.hpp из lab1).
#include "MySet.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

using json = nlohmann::json;

void loadFromFile(map<string, MySet>& sets, const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) return;
    try {
        json j; f >> j;
        for (auto& el : j.items())
            for (auto& v : el.value().get<vector<string>>()) sets[el.key()].add(v);
    } catch (exception& e) { cerr << "Load error: " << e.what() << endl; }
}

void saveToFile(const map<string, MySet>& sets, const string& filename) {
    json j;
    for (auto const& [name, s] : sets) j[name] = s.getData();
    ofstream f(filename);
    if (f.is_open()) f << j.dump(4);
}

int main(int argc, char* argv[]) {
    string filename = "sets.json";
    string query;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filename = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    map<string, MySet> sets;
    loadFromFile(sets, filename);

    stringstream ss(query);
    string cmd, name, val;
    ss >> cmd >> name >> val;

    if (cmd == "SETADD") {
        if (sets[name].add(val)) { cout << "-> " << val << endl; saveToFile(sets, filename); }
        else cout << "-> " << val << " already in set" << endl;
    } else if (cmd == "SETDEL") {
        if (sets.count(name) && sets[name].remove(val)) { cout << "-> " << val << endl; saveToFile(sets, filename); }
        else cout << "-> ERROR: " << val << " not in set" << endl;
    } else if (cmd == "SET_AT") {
        cout << "-> " << (sets.count(name) && sets[name].contains(val) ? "TRUE" : "FALSE") << endl;
    } else if (cmd == "PRINT") {
        if (!sets.count(name)) { cerr << "Error: set '" << name << "' not found" << endl; return 0; }
        cout << name << ": "; sets[name].print(); cout << " (size " << sets[name].getSize() << ")" << endl;
    } else {
        cerr << "Unknown command" << endl;
    }
    return 0;
}
