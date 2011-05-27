package xen

import (
	"fmt"
)

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

type ByteSize int64
const (
    _ = iota  // ignore first value by assigning to blank identifier
    KB ByteSize = 1<<(10*iota)
    MB
    GB
    TB
)

func (b ByteSize) String() string {
    switch {
    case b >= TB:
        return fmt.Sprintf("%.2fTB", float64(b/TB))
    case b >= GB:
        return fmt.Sprintf("%.2fGB", float64(b/GB))
    case b >= MB:
        return fmt.Sprintf("%.2fMB", float64(b/MB))
    case b >= KB:
        return fmt.Sprintf("%.2fKB", float64(b/KB))
    }
    return fmt.Sprintf("%.2fB", float64(b))
}