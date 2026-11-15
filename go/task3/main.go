// Задание 3, вариант 4. Максимальное количество пересечений.
// Подмножество хранится в массиве ЛР1 (порядок) и во множестве (проверка за O(1)). O(k^2 * m).
package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"lab2/structs"
)

type subset struct {
	items *structs.MyArray
	set   *structs.MySet
}

func parse(s string) []subset {
	var res []subset
	depth := 0
	num := ""
	for _, c := range s {
		switch {
		case c == '{':
			depth++
			if depth == 2 {
				res = append(res, subset{&structs.MyArray{}, structs.NewSet()})
			}
		case c == '}' || c == ',' || c == ' ':
			if num != "" && len(res) > 0 {
				if res[len(res)-1].set.Add(num) {
					res[len(res)-1].items.PushBack(num)
				}
				num = ""
			}
			if c == '}' {
				depth--
			}
		case c >= '0' && c <= '9':
			num += string(c)
		}
	}
	return res
}

func format(s subset) string {
	return "{" + strings.Join(s.items.GetData(), ", ") + "}"
}

func main() {
	fmt.Print("Ввод: ")
	sc := bufio.NewScanner(os.Stdin)
	sc.Scan()
	subs := parse(sc.Text())
	if len(subs) < 2 {
		fmt.Println("Нужно хотя бы два подмножества")
		return
	}
	bestI, bestJ, best := 0, 1, -1
	var bestCommon []string
	for i := range subs {
		for j := i + 1; j < len(subs); j++ {
			var common []string
			for _, v := range subs[i].items.GetData() {
				if subs[j].set.Contains(v) {
					common = append(common, v)
				}
			}
			if len(common) >= best { // при равенстве берём последнюю пару
				best, bestI, bestJ, bestCommon = len(common), i, j, common
			}
		}
	}
	fmt.Printf("Вывод: {%s, %s}, количество общих элементов: %d", format(subs[bestI]), format(subs[bestJ]), best)
	if best > 0 {
		fmt.Printf(" (%s)", strings.Join(bestCommon, ", "))
	}
	fmt.Println()
}
