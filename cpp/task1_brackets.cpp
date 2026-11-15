// Задание 1, вариант 10. Строка со скобками.
// Удалить минимальное число скобок, чтобы скобочная последовательность стала правильной.
// Стек из ЛР1 (MyStack): храним в нём индексы открывающих скобок без пары.
// Сложность O(n).
#include "MyStack.hpp"
#include <iostream>
#include <string>
#include <vector>

std::string fixBrackets(const std::string& s) {
    std::vector<bool> removed(s.size(), false);
    MyStack st;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '(') {
            st.push(std::to_string(i));
        } else if (s[i] == ')') {
            if (st.length() == 0) removed[i] = true;  // закрывающая без пары
            else st.pop();
        }
    }
    while (st.length() > 0) removed[std::stoi(st.pop())] = true;  // открывающие без пары
    std::string res;
    for (int i = 0; i < (int)s.size(); ++i)
        if (!removed[i]) res += s[i];
    return res;
}

int main() {
    std::string line;
    std::cout << "Ввод: ";
    std::getline(std::cin, line);
    std::cout << "Вывод: " << fixBrackets(line) << std::endl;
    return 0;
}
