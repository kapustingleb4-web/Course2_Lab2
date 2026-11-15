// Задание 2. АТД "Множество": SETADD, SETDEL, SET_AT.
//   ./set --file sets.json --query 'SETADD myset 5'
// Множества хранятся в JSON так же, как в ЛР1 (json.hpp из lab1).
#include "MySet.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using json = nlohmann::json;

void loadFromFile(std::map<std::string, MySet>& sets, const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;
    try {
        json j; f >> j;
        for (auto& el : j.items())
            for (auto& v : el.value().get<std::vector<std::string>>()) sets[el.key()].add(v);
    } catch (std::exception& e) { std::cerr << "Load error: " << e.what() << std::endl; }
}

void saveToFile(const std::map<std::string, MySet>& sets, const std::string& filename) {
    json j;
    for (auto const& [name, s] : sets) j[name] = s.getData();
    std::ofstream f(filename);
    if (f.is_open()) f << j.dump(4);
}

int main(int argc, char* argv[]) {
    std::string filename = "sets.json";
    std::string query;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filename = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    std::map<std::string, MySet> sets;
    loadFromFile(sets, filename);

    std::stringstream ss(query);
    std::string cmd, name, val;
    ss >> cmd >> name >> val;

    if (cmd == "SETADD") {
        if (sets[name].add(val)) { std::cout << "-> " << val << std::endl; saveToFile(sets, filename); }
        else std::cout << "-> " << val << " already in set" << std::endl;
    } else if (cmd == "SETDEL") {
        if (sets.count(name) && sets[name].remove(val)) { std::cout << "-> " << val << std::endl; saveToFile(sets, filename); }
        else std::cout << "-> ERROR: " << val << " not in set" << std::endl;
    } else if (cmd == "SET_AT") {
        std::cout << "-> " << (sets.count(name) && sets[name].contains(val) ? "TRUE" : "FALSE") << std::endl;
    } else if (cmd == "PRINT") {
        if (!sets.count(name)) { std::cerr << "Error: set '" << name << "' not found" << std::endl; return 0; }
        std::cout << name << ": "; sets[name].print(); std::cout << " (size " << sets[name].getSize() << ")" << std::endl;
    } else {
        std::cerr << "Unknown command" << std::endl;
    }
    return 0;
}
