package main

import (
	"vimini/xen"
	"fmt"
	"exec"
	"log"
)

func main() {
	fmt.Println("vimini started")

	cmd := exec.Command("/usr/sbin/xm", "list")
	data, err := cmd.CombinedOutput()
	if err != nil {
		log.Panicf("something happened %s\n", err)
	}

	fmt.Printf("output '%s'\n", string(data));	
}

func list() {
	handle := xen.Init()

	physInfo := handle.PhysInfo()

	fmt.Printf("total memory %s\n", xen.ByteSize(int64(physInfo.TotalPages) * 4096))
	fmt.Printf("free memory %s\n", xen.ByteSize(int64(physInfo.FreePages) * 4096))
	fmt.Printf("threads per core %d\n", physInfo.ThreadsPerCore)
	fmt.Printf("cores per socket %d\n", physInfo.CoresPerSocket)
	fmt.Printf("nr cpus %d\n", physInfo.NrCpus)

	domains := handle.List()
	for _, d := range(domains) {
		fmt.Printf("domain: %d (%s) %d Mb\n", d.DomId, d.Name, d.Memory / 1024)
	}

}