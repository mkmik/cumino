package main

import (
	"os"
	"flag"
	"fmt"
)

var port = flag.Int("p", 5873, "port")

var Usage = func() {
    fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
}

func main() {
	flag.Parse()

	fmt.Printf("port %d\n", *port)
}
