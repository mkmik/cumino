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


func readRemoteFile(c *clnt.Clnt, name string, dest io.Writer) os.Error {
	file, err := c.FOpen(name, p.OREAD)
	if err != nil {
		return os.NewError(err.String())
	}
	defer file.Close()

	buf := make([]byte, 8192)
	for {
		n, err := file.Read(buf)
		if err != nil {
			return os.NewError(err.String())
		}

		if n == 0 {
			break
		}

		dest.Write(buf[0:n])
	}

	return nil
}

func readAllRemoteFile(c *clnt.Clnt, name string) ([]byte, os.Error) {
	data := bytes.NewBuffer(make([]byte, 0, 8192))

	err := readRemoteFile(c, name, data)
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
	defer func() { deleteNow <- temp.Name() }()
	temp.Chmod(0766)

	hash := sha256.New()
	dest := io.MultiWriter(temp, hash)

	fmt.Printf("downloading file\n")
	err = readRemoteFile(c, "/vimini", dest)
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot read remote file: %s\n", err))
	}
	temp.Close()

	sig, err := readAllRemoteFile(c, "/vimini.sha256")
	if err != nil {
		return os.NewError(fmt.Sprintf("cannot read remote file md5: %s\n", err))
	}

	sum := hash.Sum()
	valid := verify(sum, sig)

	if !valid {
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
	toBeDeleted := make(map[string]int)
	for {
		select {
		case reg := <-toDelete:
			toBeDeleted[reg] = 1

		case file := <-deleteNow:
			fmt.Printf("deleting now %s\n", file)
			toBeDeleted[file] = 0, false
			os.Remove(file)

		case sig := <-signal.Incoming:
			fmt.Printf("got signal %v\n", sig)

			for el, _ := range toBeDeleted {
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


func verify(hash []byte, sig []byte) bool {
	cert, err := ioutil.ReadFile("/home/marko/Projects/efg-auth/certs/cert.crt")
	if err != nil {
		log.Panicf("load certificate %s\n", err)
	}

	pcert, err := x509.ParseCertificate(cert)

	if err != nil {
		log.Panicf("parse cert %s\n", err)
	}

	pub := pcert.PublicKey.(*rsa.PublicKey)

	err = rsa.VerifyPKCS1v15(pub, crypto.SHA256, hash, sig)
	return err == nil
}

func main() {
	fmt.Printf("node checker\n")

	flag.Parse()

	clnt.DefaultDebuglevel = *debuglevel

	go handleSignals()

	downloader()

}
