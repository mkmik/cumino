package main

import (
	"os"
	"flag"
	"fmt"
)

var omitNewline = flag.Bool("n", false, "don't print final newline")

var Usage = func() {
    fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
}

func main() {
	flag.Parse()

	if !*omitNewline {
		println("ok")
	}
}
