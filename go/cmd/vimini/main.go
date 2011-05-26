package main

import (
	"vimini/xen"
	"fmt"
)

func main() {
	fmt.Println("vimini started")

	handle := xen.Init()

	domains := handle.List()
	for _, d := range(domains) {
		fmt.Printf("domain: %d (%s)\n", d.DomId, d.Name)
	}

	name := handle.Name(0)
	fmt.Printf("name: %s\n", name)
}