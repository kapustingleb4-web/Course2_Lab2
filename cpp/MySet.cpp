#include "MySet.hpp"

bool MySet::add(const std::string& val) {
    if (table.contains(val)) return false;
    table.insert(val, "");
    return true;
}

bool MySet::remove(const std::string& val) { return table.remove(val); }

bool MySet::contains(const std::string& val) const { return table.contains(val); }

int MySet::getSize() const { return table.getSize(); }

void MySet::print() const {
    std::cout << "{ ";
    for (auto& k : table.keys()) std::cout << k << " ";
    std::cout << "}";
}

std::vector<std::string> MySet::getData() const { return table.keys(); }
