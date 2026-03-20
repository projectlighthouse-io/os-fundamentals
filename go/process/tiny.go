// tiny.go
// minimal program to observe VSZ vs RSS in Go.
// the Go runtime pre-maps a large virtual address region for its GC heap,
// so VSZ will be ~1 GB while RSS is only a few MB.
//
// usage:
//   go build -o tiny tiny.go
//   ./tiny &
//   ps -o pid,vsz,rss,comm -p $!
//   kill $!
package main

import (
	"fmt"
	"time"
)

func main() {
	fmt.Println("hello")
	time.Sleep(30 * time.Second)
}
