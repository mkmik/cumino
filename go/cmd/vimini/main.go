package main

import (
	"vimini/xen"
	"fmt"
)

func main() {
	fmt.Println("vimini started")

	handle := xen.Init()

	physInfo := handle.PhysInfo()

	fmt.Printf("total memory %d\n", int64(physInfo.TotalPages) * 4096)

	domains := handle.List()
	for _, d := range(domains) {
		fmt.Printf("domain: %d (%s) %d Mb\n", d.DomId, d.Name, d.Memory / 1024)
	}

	name := handle.Name(0)
	fmt.Printf("name: %s\n", name)
}