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
	fmt.Printf("free memory %d\n", int64(physInfo.FreePages) * 4096)
	fmt.Printf("threads per core %d\n", physInfo.ThreadsPerCore)
	fmt.Printf("cores per socket %d\n", physInfo.CoresPerSocket)
	fmt.Printf("nr cpus %d\n", physInfo.NrCpus)

	domains := handle.List()
	for _, d := range(domains) {
		fmt.Printf("domain: %d (%s) %d Mb\n", d.DomId, d.Name, d.Memory / 1024)
	}

	name := handle.Name(0)
	fmt.Printf("name: %s\n", name)
}