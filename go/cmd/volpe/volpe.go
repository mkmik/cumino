package main

import (
	"vimini/lvm"
	"fmt"
)

func main() {
	lvm := lvm.Init()

	vgname := "dlib21x"
	vg := lvm.Open(vgname, "r")
	fmt.Printf("VG %p\n", vg)

	
	list := vg.List()
	fmt.Printf("list %p\n", list)

}