// Задание 6.1, вариант 2. Метод цепочек и хеширование кукушки: демонстрация и замеры времени.
package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"time"

	"lab2/structs"
)

func ns(d time.Duration, n int) float64 { return float64(d.Nanoseconds()) / float64(n) }

func demo() {
	fmt.Println("=== Демонстрация (маленькие таблицы) ===")
	chain := structs.NewChainHashTable(5, structs.HashDjb2, false)
	cuckoo := structs.NewCuckooHashTable(5)
	for _, k := range []string{"apple", "banana", "cherry", "date", "elder", "fig", "grape"} {
		chain.Insert(k, strconv.Itoa(len(k)))
		cuckoo.Insert(k, strconv.Itoa(len(k)))
	}
	fmt.Println("Метод цепочек (5 корзин, без расширения):")
	chain.Print()
	fmt.Printf("Кукушка (две таблицы по %d, вытеснений: %d, перестроений: %d):\n", cuckoo.Capacity(), cuckoo.Kicks, cuckoo.Rehashes)
	cuckoo.Print()
	a, _ := chain.Find("cherry")
	b, _ := cuckoo.Find("cherry")
	fmt.Printf("find(cherry): chain=%s, cuckoo=%s\n", a, b)
	chain.Remove("cherry")
	cuckoo.Remove("cherry")
	_, okA := chain.Find("cherry")
	_, okB := cuckoo.Find("cherry")
	fmt.Printf("после remove(cherry): chain=%v, cuckoo=%v\n\n", okA, okB)
}

func main() {
	demo()
	fmt.Println("=== Эмпирический анализ (время в нс на операцию) ===")
	fmt.Printf("%-9s%-9s%9s%9s%9s%9s   структура\n", "N", "метод", "insert", "find+", "find-", "remove")
	rng := rand.New(rand.NewSource(42))
	for _, n := range []int{1000, 10000, 100000} {
		in, out := make([]string, n), make([]string, n)
		for i := range in {
			in[i] = "key" + strconv.FormatUint(rng.Uint64(), 10)
			out[i] = "miss" + strconv.FormatUint(rng.Uint64(), 10)
		}
		{
			t := structs.NewChainHashTable(16, structs.HashDjb2, true)
			a := time.Now()
			for _, k := range in {
				t.Insert(k, k)
			}
			b := time.Now()
			for _, k := range in {
				t.Find(k)
			}
			c := time.Now()
			for _, k := range out {
				t.Find(k)
			}
			d := time.Now()
			mx, avg := t.Stats()
			for _, k := range in {
				t.Remove(k)
			}
			e := time.Now()
			fmt.Printf("%-9d%-9s%9.1f%9.1f%9.1f%9.1f   max цепочка=%d, ср. цепочка=%.2f\n", n, "chain",
				ns(b.Sub(a), n), ns(c.Sub(b), n), ns(d.Sub(c), n), ns(e.Sub(d), n), mx, avg)
		}
		{
			t := structs.NewCuckooHashTable(16)
			a := time.Now()
			for _, k := range in {
				t.Insert(k, k)
			}
			b := time.Now()
			for _, k := range in {
				t.Find(k)
			}
			c := time.Now()
			for _, k := range out {
				t.Find(k)
			}
			d := time.Now()
			kicks, rh := t.Kicks, t.Rehashes
			for _, k := range in {
				t.Remove(k)
			}
			e := time.Now()
			fmt.Printf("%-9d%-9s%9.1f%9.1f%9.1f%9.1f   вытеснений/вставку=%.2f, перестроений=%d\n", n, "cuckoo",
				ns(b.Sub(a), n), ns(c.Sub(b), n), ns(d.Sub(c), n), ns(e.Sub(d), n), float64(kicks)/float64(n), rh)
		}
	}
}
