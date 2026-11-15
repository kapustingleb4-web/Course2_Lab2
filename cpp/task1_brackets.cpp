// Задание 1, вариант 10. Строка со скобками.
// Удалить минимальное число скобок, чтобы скобочная последовательность стала правильной.
// Стек из ЛР1 (MyStack): храним в нём индексы открывающих скобок без пары.
// Сложность O(n).
#include "MyStack.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string fixBrackets(const string& s) {
    vector<bool> removed(s.size(), false);
    MyStack st;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '(') {
            st.push(to_string(i));
        } else if (s[i] == ')') {
            if (st.length() == 0) removed[i] = true;  // закрывающая без пары
            else st.pop();
        }
    }
    while (st.length() > 0) removed[stoi(st.pop())] = true;  // открывающие без пары
    string res;
    for (int i = 0; i < (int)s.size(); ++i)
        if (!removed[i]) res += s[i];
    return res;
}

int main() {
    string line;
    cout << "Ввод: ";
    getline(cin, line);
    cout << "Вывод: " << fixBrackets(line) << endl;
    return 0;
}
