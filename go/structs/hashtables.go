package structs

import "fmt"

// Задание 6.1: метод цепочек и хеширование кукушки. Хеш-функция передаётся параметром (для 6.2).
type HashFunc func(string) uint64

func HashDjb2(s string) uint64 {
	var h uint64 = 5381
	for i := 0; i < len(s); i++ {
		h = h*33 + uint64(s[i])
	}
	return h
}

func hashFnv(s string, seed uint64) uint64 {
	h := uint64(1469598103934665603) ^ seed
	for i := 0; i < len(s); i++ {
		h ^= uint64(s[i])
		h *= 1099511628211
	}
	return h ^ (h >> 29)
}

// ---------------- Метод цепочек ----------------
type ChainHashTable struct {
	buckets    []*htNode
	capacity   int
	size       int
	fn         HashFunc
	autoResize bool
}

func NewChainHashTable(capacity int, fn HashFunc, autoResize bool) *ChainHashTable {
	return &ChainHashTable{buckets: make([]*htNode, capacity), capacity: capacity, fn: fn, autoResize: autoResize}
}

func (t *ChainHashTable) Index(key string) int { return int(t.fn(key) % uint64(t.capacity)) }

func (t *ChainHashTable) rehash() {
	old := t.buckets
	t.capacity *= 2
	t.buckets = make([]*htNode, t.capacity)
	for _, cur := range old {
		for cur != nil {
			next := cur.next
			idx := t.Index(cur.key)
			cur.next = t.buckets[idx]
			t.buckets[idx] = cur
			cur = next
		}
	}
}

func (t *ChainHashTable) Insert(key, value string) {
	idx := t.Index(key)
	for cur := t.buckets[idx]; cur != nil; cur = cur.next {
		if cur.key == key {
			cur.value = value
			return
		}
	}
	t.buckets[idx] = &htNode{key, value, t.buckets[idx]}
	t.size++
	if t.autoResize && t.size > t.capacity {
		t.rehash()
	}
}

func (t *ChainHashTable) Find(key string) (string, bool) {
	for cur := t.buckets[t.Index(key)]; cur != nil; cur = cur.next {
		if cur.key == key {
			return cur.value, true
		}
	}
	return "", false
}

func (t *ChainHashTable) Remove(key string) bool {
	idx := t.Index(key)
	var prev *htNode
	for cur := t.buckets[idx]; cur != nil; cur = cur.next {
		if cur.key == key {
			if prev != nil {
				prev.next = cur.next
			} else {
				t.buckets[idx] = cur.next
			}
			t.size--
			return true
		}
		prev = cur
	}
	return false
}

func (t *ChainHashTable) Stats() (maxChain int, avgChain float64) {
	nonEmpty := 0
	for _, cur := range t.buckets {
		l := 0
		for ; cur != nil; cur = cur.next {
			l++
		}
		if l > 0 {
			nonEmpty++
		}
		maxChain = max(maxChain, l)
	}
	if nonEmpty > 0 {
		avgChain = float64(t.size) / float64(nonEmpty)
	}
	return
}

func (t *ChainHashTable) Print() {
	for i, cur := range t.buckets {
		fmt.Printf("[%d]", i)
		for ; cur != nil; cur = cur.next {
			fmt.Printf(" -> (%s: %s)", cur.key, cur.value)
		}
		fmt.Println()
	}
}

// ---------------- Хеширование кукушки ----------------
type slot struct {
	key, value string
	used       bool
}

type CuckooHashTable struct {
	t1, t2   []slot
	capacity int
	size     int
	seed     uint64
	Kicks    int // сколько раз вытесняли элемент
	Rehashes int // сколько раз перестраивали таблицы
}

func NewCuckooHashTable(capacity int) *CuckooHashTable {
	return &CuckooHashTable{t1: make([]slot, capacity), t2: make([]slot, capacity), capacity: capacity, seed: 0x9E3779B97F4A7C15}
}

func (t *CuckooHashTable) Capacity() int { return t.capacity }

func (t *CuckooHashTable) h1(k string) int {
	h := HashDjb2(k) ^ (t.seed * 0x2545F4914F6CDD1D)
	h ^= h >> 31
	return int(h % uint64(t.capacity))
}

func (t *CuckooHashTable) h2(k string) int { return int(hashFnv(k, t.seed) % uint64(t.capacity)) }

func (t *CuckooHashTable) Find(key string) (string, bool) {
	if a := t.t1[t.h1(key)]; a.used && a.key == key {
		return a.value, true
	}
	if b := t.t2[t.h2(key)]; b.used && b.key == key {
		return b.value, true
	}
	return "", false
}

// place кладёт элемент, вытесняя жильцов; при зацикливании возвращает выпавший элемент
func (t *CuckooHashTable) place(key, value string) (string, string, bool) {
	maxLoop := 32
	for c := t.capacity; c > 1; c /= 2 {
		maxLoop++
	}
	for i := 0; i < maxLoop; i++ {
		a := &t.t1[t.h1(key)]
		if !a.used {
			*a = slot{key, value, true}
			return "", "", true
		}
		key, a.key = a.key, key
		value, a.value = a.value, value
		t.Kicks++
		b := &t.t2[t.h2(key)]
		if !b.used {
			*b = slot{key, value, true}
			return "", "", true
		}
		key, b.key = b.key, key
		value, b.value = b.value, value
		t.Kicks++
	}
	return key, value, false
}

func (t *CuckooHashTable) rehash(newCap int) {
	o1, o2 := t.t1, t.t2
	t.Rehashes++
	for {
		t.seed = t.seed*6364136223846793005 + 1442695040888963407
		t.capacity = newCap
		t.t1, t.t2 = make([]slot, newCap), make([]slot, newCap)
		t.size = 0
		ok := true
		for i := 0; i < len(o1) && ok; i++ {
			for _, s := range []slot{o1[i], o2[i]} {
				if !s.used {
					continue
				}
				if _, _, placed := t.place(s.key, s.value); !placed {
					ok = false
					break
				}
				t.size++
			}
		}
		if ok {
			return
		}
		newCap *= 2
	}
}

func (t *CuckooHashTable) Insert(key, value string) {
	if a := &t.t1[t.h1(key)]; a.used && a.key == key {
		a.value = value
		return
	}
	if b := &t.t2[t.h2(key)]; b.used && b.key == key {
		b.value = value
		return
	}
	if (t.size+1)*2 > t.capacity {
		t.rehash(t.capacity * 2)
	}
	k, v := key, value
	for {
		var placed bool
		if k, v, placed = t.place(k, v); placed {
			break
		}
		t.rehash(t.capacity)
	}
	t.size++
}

func (t *CuckooHashTable) Remove(key string) bool {
	if a := &t.t1[t.h1(key)]; a.used && a.key == key {
		a.used = false
		t.size--
		return true
	}
	if b := &t.t2[t.h2(key)]; b.used && b.key == key {
		b.used = false
		t.size--
		return true
	}
	return false
}

func (t *CuckooHashTable) Print() {
	show := func(s slot) string {
		if !s.used {
			return "-"
		}
		return s.key + "=" + s.value
	}
	for i := 0; i < t.capacity; i++ {
		fmt.Printf("[%d]  T1: %s   T2: %s\n", i, show(t.t1[i]), show(t.t2[i]))
	}
}
