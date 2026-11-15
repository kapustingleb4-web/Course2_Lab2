package structs

import (
	"fmt"
	"strings"
)

// LevelAVLTree - АВЛ-дерево с методом GetLevels (обход в ширину) для задания 5.
type LevelAVLTree struct {
	root *avlNode
}

type avlNode struct {
	key         int
	left, right *avlNode
	height      int
}

func height(n *avlNode) int {
	if n == nil {
		return 0
	}
	return n.height
}

func updateHeight(n *avlNode) { n.height = 1 + max(height(n.left), height(n.right)) }

func getBalance(n *avlNode) int { return height(n.left) - height(n.right) }

func rightRotate(y *avlNode) *avlNode {
	x := y.left
	y.left = x.right
	x.right = y
	updateHeight(y)
	updateHeight(x)
	return x
}

func leftRotate(x *avlNode) *avlNode {
	y := x.right
	x.right = y.left
	y.left = x
	updateHeight(x)
	updateHeight(y)
	return y
}

func balance(n *avlNode) *avlNode {
	updateHeight(n)
	bf := getBalance(n)
	if bf > 1 {
		if getBalance(n.left) < 0 { // LR
			n.left = leftRotate(n.left)
		}
		return rightRotate(n) // LL
	}
	if bf < -1 {
		if getBalance(n.right) > 0 { // RL
			n.right = rightRotate(n.right)
		}
		return leftRotate(n) // RR
	}
	return n
}

func insert(n *avlNode, key int) *avlNode {
	if n == nil {
		return &avlNode{key: key, height: 1}
	}
	if key < n.key {
		n.left = insert(n.left, key)
	} else if key > n.key {
		n.right = insert(n.right, key)
	} else {
		return n
	}
	return balance(n)
}

func (t *LevelAVLTree) Insert(key int) { t.root = insert(t.root, key) }

// GetLevels - значения узлов по уровням (сверху вниз, слева направо)
func (t *LevelAVLTree) GetLevels() [][]int {
	var res [][]int
	var level []*avlNode
	if t.root != nil {
		level = append(level, t.root)
	}
	for len(level) > 0 {
		var next []*avlNode
		var vals []int
		for _, n := range level {
			vals = append(vals, n.key)
			if n.left != nil {
				next = append(next, n.left)
			}
			if n.right != nil {
				next = append(next, n.right)
			}
		}
		res = append(res, vals)
		level = next
	}
	return res
}

func printRec(n *avlNode, space int) {
	if n == nil {
		return
	}
	space += 5
	printRec(n.right, space)
	fmt.Print("\n", strings.Repeat(" ", space-5), n.key)
	printRec(n.left, space)
}

func (t *LevelAVLTree) PrintTree() {
	printRec(t.root, 0)
	fmt.Println()
}
