package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"go9p.googlecode.com/hg/p"
	"go9p.googlecode.com/hg/p/clnt"
	"io"
	"strings"
	"io/ioutil"
	"bytes"
	"crypto/md5"
	"encoding/hex"
	"time"
)

var addr = flag.String("addr", "127.0.0.1:5645", "network address")
var debuglevel = flag.Int("d", 0, "debuglevel")


func readRemoteFile(c *clnt.Clnt, name string, dest io.Writer) ([]byte, os.Error) {
	file, err := c.FOpen(name, p.OREAD)
  if err != nil {
		return nil, os.NewError(err.String())
  }
	defer file.Close()

	hash := md5.New()
	buf := make([]byte, 8192)
  for {
   	n, err := file.Read(buf)
    if err != nil {
			return nil, os.NewError(err.String())
    }
    
    if n == 0 {
    	break
    }
  
		dest.Write(buf[0:n])
		hash.Write(buf[0:n])
  }


	return hash.Sum(), nil
}

func readAllRemoteFile(c *clnt.Clnt, name string) ([]byte, os.Error) {
	data := bytes.NewBuffer(make([]byte, 0, 8192))

	_, err := readRemoteFile(c, name, data)
	if err != nil {
		return nil, err
	}

	return data.Bytes(), nil
}

func splitChecksum(checksum string) string {
	idx := strings.Index(checksum, " ")
	return checksum[0:idx]
}

func download(c *clnt.Clnt) os.Error {
	temp, err := ioutil.TempFile(".", ".")
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot create temp file: %s\n", err))
	}
	defer temp.Close()
	defer os.Remove(temp.Name())
	temp.Chmod(0766)
	

	fmt.Printf("downloading file\n")
	sum, err := readRemoteFile(c, "/vimini", temp)
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot read remote file: %s\n", err))
	}
	temp.Close()
	

	checksum, err := readAllRemoteFile(c, "/vimini.md5")
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot read remote file md5: %s\n", err))
	}
	
	if hex.EncodeToString(sum) != splitChecksum(string(checksum)) {
		fmt.Printf("wrong checksum: %s\n", hex.EncodeToString(sum))
	} else {
		os.Rename(temp.Name(), "software/vimini")
		fmt.Printf("file downloaded\n")
	}

	return nil
}

func main() {
//	fmt.Printf("node checker\n")

	flag.Parse()

  user := p.OsUsers.Uid2User(os.Geteuid())
  clnt.DefaultDebuglevel = *debuglevel

  c, perr := clnt.Mount("tcp", *addr, "", user)
  if perr != nil {
    log.Panicf("cannot mount: %s\n", perr)
  }

	for {
		err := download(c)
		if err != nil {
			fmt.Printf("cannot download: %s\n", err)

			fmt.Printf("retrying\n")
			for {
				time.Sleep(500e6)
				
				c, perr = clnt.Mount("tcp", *addr, "", user)
				if perr != nil {
					log.Printf("cannot mount: %s\n", perr)
				} else {
					break
				}
			}
			
		}
	}


	
	
	//	ioutil.WriteFile("software/vimini.md5", []byte(hex.EncodeToString(sum)), 0666)

}

