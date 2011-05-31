package main

import  (
	"fmt"
	"log"
	"go9p.googlecode.com/hg/p"
	"go9p.googlecode.com/hg/p/srv"
)

type Ops struct {
}

func dirQid() *p.Qid {
	var qid p.Qid
	qid.Path = 1
	qid.Version = 1
	qid.Type = p.QTDIR
	
	return &qid
}

func (this *Ops) Attach(req *srv.Req) {
  tc := req.Tc
	fmt.Printf("got attach req %p, afid: %p, aname: %s\n", req, req.Afid, tc.Aname)

	if len(tc.Aname) == 0 {
		req.RespondRattach(dirQid())
	} else {
		req.RespondError(&p.Error{"test", int(12)})
	}
}

func (this *Ops) Walk(req *srv.Req) {
	fmt.Printf("got walk req %p, wname %s\n", req, req.Tc.Wname)

	
}

func (this *Ops) Open(req *srv.Req) {
	fmt.Printf("got open req %p\n", req)
}

func (this *Ops) Create(req *srv.Req) {
	fmt.Printf("got create req %p\n", req)
}

func (this *Ops) Read(req *srv.Req) {
	fmt.Printf("got Read req %p\n", req)
}

func (this *Ops) Write(req *srv.Req) {
	fmt.Printf("got write req %p\n", req)
}

func (this *Ops) Clunk(req *srv.Req) {
	fmt.Printf("got clunk req %p\n", req)
}

func (this *Ops) Remove(req *srv.Req) {
	fmt.Printf("got remove req %p\n", req)
}

func (this *Ops) Stat(req *srv.Req) {
	fmt.Printf("got stat req %p: '%s' '%s'\n", req, req.Tc.Dir.Name, req.Tc.Wname)

	dir := new(p.Dir)
	dir.Qid = *dirQid()
	dir.Mode = 511 | p.DMDIR
	dir.Atime=1
	dir.Mtime=1
	dir.Length=0
	dir.Uid = "none"
	dir.Gid = "none"
	dir.Muid = "none"

	dir.Name = "/"

	req.RespondRstat(dir)
}

func (this *Ops) Wstat(req *srv.Req) {
	fmt.Printf("got wstat req %p\n", req)
}

func main() {
	fmt.Println("softare distributor")
	
	s := new(srv.Srv)
	s.Id = "sodi"
	s.Debuglevel = srv.DbgPrintFcalls
	
	var ops srv.ReqOps
	ops = new(Ops)
	
	res := s.Start(ops)
	fmt.Printf("start: %t\n", res)
	

	err := s.StartNetListener("tcp", "0.0.0.0:5645")
	if err != nil {
		log.Panicf("cannot start net listener %s\n", err)
	}

	fmt.Printf("%p\n", s)
//	time.Sleep("
}