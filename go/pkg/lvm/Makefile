include $(GOROOT)/src/Make.inc

TARG=vimini/lvm
CGOFILES=lvm.go
#CGO_CFLAGS=-I../../deps/xen/$(XENVERSION)/include
#CGO_LDFLAGS=-L../../deps/xen/$(XENVERSION)/lib -lxenctrl -lxenstore -lxenguest
CGO_LDFLAGS=-llvm2app

include $(GOROOT)/src/Make.pkg