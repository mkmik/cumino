package xen

// #include <xenctrl.h>
import "C"

func fillPhysInfo(pinfo *PhysInfo, info *C.xc_physinfo_t) {
	pinfo.NrCpus = int(info.nr_nodes * info.sockets_per_node * info.cores_per_socket * info.threads_per_core)
}