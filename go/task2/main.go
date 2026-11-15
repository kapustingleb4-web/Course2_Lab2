// Задание 2. АТД "Множество": SETADD, SETDEL, SET_AT, PRINT.
//
//	go run ./task2 --file sets.json --query 'SETADD myset 5'
package main

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"lab2/structs"
)

func load(filename string) map[string]*structs.MySet {
	sets := make(map[string]*structs.MySet)
	data, err := os.ReadFile(filename)
	if err != nil {
		return sets
	}
	raw := make(map[string][]string)
	if err := json.Unmarshal(data, &raw); err != nil {
		fmt.Fprintln(os.Stderr, "Load error:", err)
		return sets
	}
	for name, items := range raw {
		sets[name] = structs.NewSet()
		for _, v := range items {
			sets[name].Add(v)
		}
	}
	return sets
}

func save(sets map[string]*structs.MySet, filename string) {
	raw := make(map[string][]string)
	for name, s := range sets {
		raw[name] = s.GetData()
	}
	data, _ := json.MarshalIndent(raw, "", "    ")
	os.WriteFile(filename, data, 0644)
}

func main() {
	filename, query := "sets.json", ""
	args := os.Args[1:]
	for i := 0; i < len(args); i++ {
		if args[i] == "--file" && i+1 < len(args) {
			filename = args[i+1]
			i++
		} else if args[i] == "--query" && i+1 < len(args) {
			query = args[i+1]
			i++
		}
	}
	sets := load(filename)
	t := strings.Fields(query)
	for len(t) < 3 {
		t = append(t, "")
	}
	cmd, name, val := t[0], t[1], t[2]
	s, ok := sets[name]

	switch cmd {
	case "SETADD":
		if !ok {
			s = structs.NewSet()
			sets[name] = s
		}
		if s.Add(val) {
			fmt.Println("->", val)
			save(sets, filename)
		} else {
			fmt.Println("->", val, "already in set")
		}
	case "SETDEL":
		if ok && s.Remove(val) {
			fmt.Println("->", val)
			save(sets, filename)
		} else {
			fmt.Println("-> ERROR:", val, "not in set")
		}
	case "SET_AT":
		if ok && s.Contains(val) {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case "PRINT":
		if !ok {
			fmt.Fprintln(os.Stderr, "Error: set", name, "not found")
			return
		}
		fmt.Print(name, ": ")
		s.Print()
		fmt.Printf(" (size %d)\n", s.Size())
	default:
		fmt.Fprintln(os.Stderr, "Unknown command")
	}
}
