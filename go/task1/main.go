// Задание 1, вариант 10. Строка со скобками.
// Удалить минимальное число скобок, чтобы последовательность стала правильной.
// Стек из ЛР1 хранит индексы открывающих скобок без пары. O(n).
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"

	"lab2/structs"
)

func fixBrackets(s string) string {
	removed := make([]bool, len(s))
	st := &structs.MyStack{}
	for i := 0; i < len(s); i++ {
		if s[i] == '(' {
			st.Push(strconv.Itoa(i))
		} else if s[i] == ')' {
			if _, err := st.Pop(); err != nil {
				removed[i] = true // закрывающая без пары
			}
		}
	}
	for {
		v, err := st.Pop()
		if err != nil {
			break
		}
		idx, _ := strconv.Atoi(v)
		removed[idx] = true // открывающие без пары
	}
	res := []byte{}
	for i := 0; i < len(s); i++ {
		if !removed[i] {
			res = append(res, s[i])
		}
	}
	return string(res)
}

func main() {
	fmt.Print("Ввод: ")
	sc := bufio.NewScanner(os.Stdin)
	sc.Scan()
	fmt.Println("Вывод:", fixBrackets(sc.Text()))
}
