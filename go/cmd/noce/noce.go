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
	"os/signal"
	"crypto"
	"crypto/x509"
	"crypto/rsa"
	"crypto/sha256"
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
	temp, err := ioutil.TempFile(".", ".download-")
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot create temp file: %s\n", err))
	}
	defer temp.Close()
	toDelete <- temp.Name()
	defer func() {deleteNow <- temp.Name()}()
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

func mountWait() *clnt.Clnt {
  user := p.OsUsers.Uid2User(os.Geteuid())

	for {
		c, perr := clnt.Mount("tcp", *addr, "", user)
		if perr != nil {
			log.Printf("cannot mount: %s\n", perr)
			time.Sleep(500e6)
		} else {
			return c
		}
	}
	// should never get here
	return nil
}

func downloader() {
	for {
		c := mountWait()
		for {
			err := download(c)
			if err != nil {
				fmt.Printf("cannot download: %s\n", err)
				break
			}
		}
	}
}


var toDelete = make(chan string, 10)
var deleteNow = make(chan string, 10)

func handleSignals() {
	toBeDeleted := make(map[string] int)
	for {
		select {
		case reg := <- toDelete:
			toBeDeleted[reg] = 1

		case file := <- deleteNow:
			toBeDeleted[file] = 0, false
			os.Remove(file)

		case sig := <- signal.Incoming:
			fmt.Printf("got signal %v\n", sig)
			
			for el, _ := range(toBeDeleted) {
				fmt.Printf("Deleting temporary %s\n", el)
        os.Remove(el)
			}

			ux, ok := sig.(signal.UnixSignal)
			if ok {
				os.Exit(int(ux))
			} else {
				os.Exit(1)
			}
		}
	}
}


func main() {
	fmt.Printf("node checker\n")

	flag.Parse()

  clnt.DefaultDebuglevel = *debuglevel
 
	go handleSignals()

//	downloader()
	
	cert, err := ioutil.ReadFile("/home/marko/Projects/efg-auth/certs/cert.crt")
	if err != nil {
		log.Panicf("load certificate %s\n", err)
	}
	
	pcert, err := x509.ParseCertificate(cert)

	if err != nil {
		log.Panicf("parse cert %s\n", err)
	}
	
	pub := pcert.PublicKey.(*rsa.PublicKey)
	fmt.Printf("key %v\n", pub)

	sig, err := ioutil.ReadFile("/tmp/test.sha256")
	if err != nil {
		log.Panicf("load sig %s\n", err)
	}

	algo := sha256.New()
	algo.Write([]byte("ciao"))
	fhash := algo.Sum()

	err = rsa.VerifyPKCS1v15(pub, crypto.SHA256, fhash, sig)
	if err != nil {
		fmt.Printf("failed verify %s\n", err)
	} else {
		fmt.Printf("verify ok\n")
	}

}

