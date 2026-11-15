// Задание 6.2, вариант 1. Хеш-функция методом свёртки:
// 523456795 -> 523 + 456 + 795 = 1774, индекс = сумма % размер таблицы.
package main

import (
	"fmt"
	"strconv"
	"strings"

	"lab2/structs"
)

func isNumber(s string) bool {
	if s == "" {
		return false
	}
	for _, c := range s {
		if c < '0' || c > '9' {
			return false
		}
	}
	return true
}

func fold(key string) (uint64, string) {
	var sum uint64
	var parts []string
	digits := isNumber(key)
	for i := 0; i < len(key); i += 3 {
		part := key[i:min(i+3, len(key))]
		var v uint64
		if digits {
			v, _ = strconv.ParseUint(part, 10, 64)
			parts = append(parts, part)
		} else {
			for j := 0; j < len(part); j++ {
				v = v*256 + uint64(part[j])
			}
			parts = append(parts, strconv.FormatUint(v, 10))
		}
		sum += v
	}
	return sum, strings.Join(parts, "+")
}

func foldHash(key string) uint64 {
	h, _ := fold(key)
	return h
}

func main() {
	const size = 11
	table := structs.NewChainHashTable(size, foldHash, false)
	fmt.Println("Введите ключи через пробел (Ctrl+D - конец):")
	for {
		var key string
		if _, err := fmt.Scan(&key); err != nil {
			break
		}
		h, expr := fold(key)
		fmt.Println("Ввод:", key)
		fmt.Printf("Вывод: %s = %d.\n", expr, h)
		fmt.Printf("  индекс в таблице размера %d: %d mod %d = %d\n", size, h, size, table.Index(key))
		table.Insert(key, strconv.FormatUint(h, 10))
	}
	fmt.Println("\nХеш-таблица (метод цепочек + свёртка):")
	table.Print()
}
