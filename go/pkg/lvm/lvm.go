package lvm

/*
 #include <lvm2app.h>
 #include <libdevmapper.h>
 lv_list_t *toLvList(struct dm_list* list) { return (lv_list_t*)list; }

 struct logical_volume {
   int8_t uuid[32*2];
   char *name;
 };
*/
import "C"

import (
	"fmt"
	"unsafe"
)

type lvm_handle struct {
	lvm C.lvm_t
}

func Init() *lvm_handle {
	fmt.Println("initializing lvm")

	return &lvm_handle{lvm: C.lvm_init(nil)}
}

type vg_handle struct {
	vg C.vg_t
}

func (this *lvm_handle) Open(vg string, mode string) *vg_handle {
  cvg := C.CString(vg)
  defer C.free(unsafe.Pointer(cvg))

  cmode := C.CString(mode)
  defer C.free(unsafe.Pointer(cmode))


	return &vg_handle{vg: C.lvm_vg_open(this.lvm, cvg, cmode, 0)}
}

type lv_info struct {
}

func (this *vg_handle) List() []lv_info {
	dm_list := C.lvm_vg_list_lvs(this.vg)
	head := C.toLvList(dm_list)
//	fmt.Printf("List %p\n", head)

	for node := C.toLvList(head.list.n); C.toLvList(node.list.n) != head; node=C.toLvList(node.list.n) {
//		fmt.Printf("node %p\n", node)
		//fmt.Printf("node lv %p\n", node.lv)
		if node.lv != nil {
			name := C.lvm_lv_get_name(node.lv)
			//defer C.free(unsafe.Pointer(name)) //crashes
			fmt.Printf("volume name %s\n", C.GoString(name))
		}
	}

	var res [0]lv_info
	return res[:]
}

