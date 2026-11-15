// Задание 7, вариант 2. LFU-кэш. GET и SET - O(1):
// keys: ключ -> узел, freqs: частота -> двусвязный список узлов, minFreq - минимальная частота.
package main

import (
	"fmt"
	"strings"
)

type node struct {
	key, value, freq int
	prev, next       *node
}

// двусвязный список узлов (в хвосте - самые свежие)
type freqList struct {
	head, tail *node
	size       int
}

func (l *freqList) pushBack(n *node) {
	n.prev, n.next = l.tail, nil
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
	l.size++
}

func (l *freqList) remove(n *node) {
	if n.prev != nil {
		n.prev.next = n.next
	} else {
		l.head = n.next
	}
	if n.next != nil {
		n.next.prev = n.prev
	} else {
		l.tail = n.prev
	}
	l.size--
}

type LFUCache struct {
	capacity, minFreq int
	keys              map[int]*node
	freqs             map[int]*freqList
}

func NewLFUCache(capacity int) *LFUCache {
	return &LFUCache{capacity: capacity, keys: map[int]*node{}, freqs: map[int]*freqList{}}
}

func (c *LFUCache) list(f int) *freqList {
	if c.freqs[f] == nil {
		c.freqs[f] = &freqList{}
	}
	return c.freqs[f]
}

func (c *LFUCache) touch(n *node) {
	c.list(n.freq).remove(n)
	if c.list(n.freq).size == 0 && c.minFreq == n.freq {
		c.minFreq++
	}
	n.freq++
	c.list(n.freq).pushBack(n)
}

func (c *LFUCache) Get(key int) int {
	n, ok := c.keys[key]
	if !ok {
		return -1
	}
	c.touch(n)
	return n.value
}

func (c *LFUCache) Set(key, value int) {
	if c.capacity <= 0 {
		return
	}
	if n, ok := c.keys[key]; ok {
		n.value = value
		c.touch(n)
		return
	}
	if len(c.keys) == c.capacity {
		victim := c.list(c.minFreq).head
		c.list(c.minFreq).remove(victim)
		delete(c.keys, victim.key)
	}
	n := &node{key: key, value: value, freq: 1}
	c.keys[key] = n
	c.list(1).pushBack(n)
	c.minFreq = 1
}

func main() {
	var capacity, q int
	fmt.Println("Введите cap и Q, затем запросы:")
	fmt.Scan(&capacity, &q)
	cache := NewLFUCache(capacity)
	var out []string
	for i := 0; i < q; i++ {
		var cmd string
		fmt.Scan(&cmd)
		if cmd == "SET" {
			var x, y int
			fmt.Scan(&x, &y)
			cache.Set(x, y)
		} else if cmd == "GET" {
			var x int
			fmt.Scan(&x)
			out = append(out, fmt.Sprint(cache.Get(x)))
		}
	}
	fmt.Println("Вывод:", strings.Join(out, " "))
}
