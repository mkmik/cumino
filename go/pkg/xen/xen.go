package xen

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <xenctrl.h>
// #include <xs.h>
// typedef struct xs_handle xs_handle_t;
// char *castToString(void* s) { return (char*)s; }
import "C"

import (
	"fmt"
	"unsafe"
	"strconv"
)

type xen_handle struct {
	xc C.int
	xs *C.xs_handle_t
}

func Init() *xen_handle {
	fmt.Println("vimini ok")

	return &xen_handle{xc: C.xc_interface_open(), xs: C.xs_daemon_open_readonly()}
}

func (this *xen_handle) List() []Domain {

	fmt.Printf("opened xen %d %p\n", this.xc, this.xs)

	var info [100]C.xc_dominfo_t
	res := C.xc_domain_getinfo(this.xc, 0, 100-1, &info[0])
	
	domains := make([]Domain, res)
	slice := info[0:res]
	for idx, di := range(slice) {
		domid := int(di.domid)
		domains[idx].DomId = domid
		domains[idx].Name = this.Name(domid)
		domains[idx].Memory = this.Memory(domid)
	}
	
	return domains
}

func (this *xen_handle) PhysInfo() PhysInfo {
	var info C.xc_physinfo_t 
	C.xc_physinfo(this.xc, &info)

	pinfo := PhysInfo{int32(info.total_pages), int32(info.free_pages), int32(info.scrub_pages), int(info.threads_per_core), int(info.cores_per_socket), 0}
	fillPhysInfo(&pinfo, &info)
	return pinfo
}

func (this *xen_handle) Read(path string) string {
  p := C.CString(path)
  defer C.free(unsafe.Pointer(p))
	var len C.uint
	
	cname := C.xs_read(this.xs, C.XBT_NULL, p, &len)
	defer C.free(cname)

	return C.GoString(C.castToString(cname))
}

func (this *xen_handle) Name(id int) string {
	path := fmt.Sprintf("/local/domain/%d/name", id)
	return this.Read(path)
}

func (this *xen_handle) Memory(id int) int {
	path := fmt.Sprintf("/local/domain/%d/memory/target", id)
	memory, _ := strconv.Atoi(this.Read(path))
	return memory
}