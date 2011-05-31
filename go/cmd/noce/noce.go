package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"go9p.googlecode.com/hg/p"
	"go9p.googlecode.com/hg/p/clnt"
	"io/ioutil"
// "bytes"
	"crypto/md5"
	"encoding/hex"
)

var addr = flag.String("addr", "127.0.0.1:5645", "network address")
var debuglevel = flag.Int("d", 0, "debuglevel")

func main() {
//	fmt.Printf("node checker\n")

	flag.Parse()

  user := p.OsUsers.Uid2User(os.Geteuid())
  clnt.DefaultDebuglevel = *debuglevel

  c, err := clnt.Mount("tcp", *addr, "", user)
  if err != nil {
    log.Panicf("cannot mount: %s\n", err)
  }

	file, err := c.FOpen("/vimini", p.OREAD)
  if err != nil {
    log.Panicf("cannot open: %s\n", err)
  }

	temp, oerr := ioutil.TempFile(".", ".")
	if oerr != nil {
		log.Panicf("cannot create temp file: %s\n", oerr)
	}
	defer temp.Close()

	hash := md5.New()
	buf := make([]byte, 8192)
  for {
   	n, err := file.Read(buf)
    if err != nil {
      log.Panicf("cannot read: %s\n", err)
    }
    
    if n == 0 {
    	break
    }
  
		temp.Write(buf[0:n])
		hash.Write(buf[0:n])
  }

	sum := hash.Sum()
	temp.Chmod(0766)
	os.Rename(temp.Name(), "software/vimini")
	ioutil.WriteFile("software/vimini.md5", []byte(hex.EncodeToString(sum)), 0666)
	fmt.Printf("%s\n", hex.EncodeToString(sum))
}

