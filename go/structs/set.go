package structs

import "fmt"

// MySet - АТД "Множество" на основе хеш-таблицы: элемент хранится как ключ.
type MySet struct {
	table *MyHashTable
}

func NewSet() *MySet { return &MySet{table: NewHashTable()} }

func (s *MySet) Add(val string) bool {
	if s.table.Contains(val) {
		return false
	}
	s.table.Insert(val, "")
	return true
}

func (s *MySet) Remove(val string) bool   { return s.table.Remove(val) }
func (s *MySet) Contains(val string) bool { return s.table.Contains(val) }
func (s *MySet) Size() int                { return s.table.Size() }
func (s *MySet) GetData() []string        { return s.table.Keys() }

func (s *MySet) Print() {
	fmt.Print("{ ")
	for _, k := range s.table.Keys() {
		fmt.Print(k, " ")
	}
	fmt.Print("}")
}
