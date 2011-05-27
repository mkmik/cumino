package xen

type Domain struct {
	DomId int
	Name string
	Memory int  // in kilobytes
}

type PhysInfo struct {
	TotalPages int32
	FreePages int32
	ScrubPages int32
	ThreadsPerCore int
	CoresPerSocket int
	NrCpus int

}