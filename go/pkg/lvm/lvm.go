package lvm

// #include <lvm2app.h>
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

func (this *vg_handle) List() []lv_info {
	C.lvm_vg_list_lvs(this.vg)
}

