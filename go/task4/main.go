// Задание 4, вариант 4. Пирамида.
// Ответ = сумма максимальных высот по всем различным ширинам.
// Хеш-таблица: ширина -> максимальная высота. O(N).
package main

import (
	"fmt"
	"strconv"

	"lab2/structs"
)

func main() {
	var n int
	fmt.Scan(&n)
	best := structs.NewHashTable()
	for i := 0; i < n; i++ {
		var w, h int64
		fmt.Scan(&w, &h)
		key := strconv.FormatInt(w, 10)
		cur, ok := best.Get(key)
		curH, _ := strconv.ParseInt(cur, 10, 64)
		if !ok || curH < h {
			best.Insert(key, strconv.FormatInt(h, 10))
		}
	}
	var total int64
	for _, v := range best.Values() {
		h, _ := strconv.ParseInt(v, 10, 64)
		total += h
	}
	fmt.Println(total)
}
