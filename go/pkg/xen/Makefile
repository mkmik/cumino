include $(GOROOT)/src/Make.inc

TARG=vimini/xen
GOFILES=domain.go
CGOFILES=xen.go
CGO_CFLAGS=-I../../../c/xen/include
CGO_LDFLAGS=-L../../../c/xen/lib -lxenctrl -lxenstore

include $(GOROOT)/src/Make.pkg