package structs

import "fmt"

// MyHashTable - хеш-таблица (метод цепочек), ключ и значение - строки.
type MyHashTable struct {
	buckets  []*htNode
	capacity int
	size     int
}

type htNode struct {
	key, value string
	next       *htNode
}

func NewHashTable() *MyHashTable {
	return &MyHashTable{buckets: make([]*htNode, 16), capacity: 16}
}

// djb2: h = h*33 + c
func (h *MyHashTable) hash(key string) int {
	var v uint64 = 5381
	for i := 0; i < len(key); i++ {
		v = v*33 + uint64(key[i])
	}
	return int(v % uint64(h.capacity))
}

func (h *MyHashTable) rehash() {
	old := h.buckets
	h.capacity *= 2
	h.buckets = make([]*htNode, h.capacity)
	for _, cur := range old {
		for cur != nil {
			next := cur.next
			idx := h.hash(cur.key)
			cur.next = h.buckets[idx]
			h.buckets[idx] = cur
			cur = next
		}
	}
}

func (h *MyHashTable) Insert(key, value string) {
	idx := h.hash(key)
	for cur := h.buckets[idx]; cur != nil; cur = cur.next {
		if cur.key == key {
			cur.value = value
			return
		}
	}
	h.buckets[idx] = &htNode{key, value, h.buckets[idx]}
	h.size++
	if h.size*4 > h.capacity*3 { // заполнение > 0.75
		h.rehash()
	}
}

func (h *MyHashTable) Get(key string) (string, bool) {
	for cur := h.buckets[h.hash(key)]; cur != nil; cur = cur.next {
		if cur.key == key {
			return cur.value, true
		}
	}
	return "", false
}

func (h *MyHashTable) Contains(key string) bool {
	_, ok := h.Get(key)
	return ok
}

func (h *MyHashTable) Remove(key string) bool {
	idx := h.hash(key)
	var prev *htNode
	for cur := h.buckets[idx]; cur != nil; cur = cur.next {
		if cur.key == key {
			if prev != nil {
				prev.next = cur.next
			} else {
				h.buckets[idx] = cur.next
			}
			h.size--
			return true
		}
		prev = cur
	}
	return false
}

func (h *MyHashTable) Size() int { return h.size }

func (h *MyHashTable) Keys() []string {
	var res []string
	for _, cur := range h.buckets {
		for ; cur != nil; cur = cur.next {
			res = append(res, cur.key)
		}
	}
	return res
}

func (h *MyHashTable) Values() []string {
	var res []string
	for _, cur := range h.buckets {
		for ; cur != nil; cur = cur.next {
			res = append(res, cur.value)
		}
	}
	return res
}

func (h *MyHashTable) Print() {
	fmt.Print("{ ")
	for _, cur := range h.buckets {
		for ; cur != nil; cur = cur.next {
			fmt.Print(cur.key, ":", cur.value, " ")
		}
	}
	fmt.Print("}")
}
