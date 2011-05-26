package main

import (
	"vimini/xen"
	"fmt"
)

func main() {
	fmt.Println("vimini started")

	handle := xen.Init()
	
	for i:=0; i<100000; i++ {
		handle.List()
	}


	domains := handle.List()
	for _, d := range(domains) {
		fmt.Printf("domain: %d (%s)\n", d.DomId, d.Name)
	}

	name := handle.Name(0)
	fmt.Printf("name: %s\n", name)
}