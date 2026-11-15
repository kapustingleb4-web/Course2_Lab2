#include "MySet.hpp"

using namespace std;

bool MySet::add(const string& val) {
    if (table.contains(val)) return false;
    table.insert(val, "");
    return true;
}

bool MySet::remove(const string& val) { return table.remove(val); }

bool MySet::contains(const string& val) const { return table.contains(val); }

int MySet::getSize() const { return table.getSize(); }

void MySet::print() const {
    cout << "{ ";
    for (auto& k : table.keys()) cout << k << " ";
    cout << "}";
}

vector<string> MySet::getData() const { return table.keys(); }
