// Задание 5, вариант 2. Чтение дерева "змейкой" (АВЛ-дерево, уровни по очереди слева/справа). O(n).
package main

import (
	"fmt"

	"lab2/structs"
)

func main() {
	tree := &structs.LevelAVLTree{}
	fmt.Print("Введите числа (0 - конец ввода): ")
	for {
		var x int
		if _, err := fmt.Scan(&x); err != nil || x == 0 {
			break
		}
		tree.Insert(x)
	}
	fmt.Print("Дерево:")
	tree.PrintTree()

	fmt.Print("Змейка: ")
	leftToRight, first := true, true
	for _, level := range tree.GetLevels() {
		for i := range level {
			v := level[i]
			if !leftToRight {
				v = level[len(level)-1-i]
			}
			if !first {
				fmt.Print(" - ")
			}
			fmt.Print(v)
			first = false
		}
		leftToRight = !leftToRight
	}
	fmt.Println()
}
