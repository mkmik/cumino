package xen

// #include <xenctrl.h>
import "C"

func fillPhysInfo(pinfo *PhysInfo, info *C.xc_physinfo_t) {
	pinfo.NrCpus = int(info.nr_cpus)
}