package xen

type Domain struct {
	DomId int
	Name string
	Memory int  // in kilobytes
}

type PhysInfo struct {
	TotalPages int32
}