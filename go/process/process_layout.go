// process_layout.go
// shows Go runtime memory statistics (HeapAlloc, HeapSys, Sys, StackSys)
// and reads /proc/self/maps to display the first 10 memory regions.
//
// compare with the Rust version to see how different runtimes
// map virtual memory differently.
//
// usage:
//   go build -o process_layout process_layout.go && ./process_layout
package main

import (
	"fmt"
	"os"
	"runtime"
	"strings"
)

func main() {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)

	fmt.Printf("PID: %d\n\n", os.Getpid())
	fmt.Printf("Go runtime memory:\n")
	fmt.Printf("  HeapAlloc (in-use):   %d kB\n", m.HeapAlloc/1024)
	fmt.Printf("  HeapSys   (from OS):  %d kB\n", m.HeapSys/1024)
	fmt.Printf("  Sys       (total):    %d kB\n", m.Sys/1024)
	fmt.Printf("  StackSys:             %d kB\n", m.StackSys/1024)

	// read our own memory map -- same kernel data structure, different language
	data, err := os.ReadFile(fmt.Sprintf("/proc/%d/maps", os.Getpid()))
	if err != nil {
		fmt.Fprintf(os.Stderr, "could not read maps: %v\n", err)
		return
	}
	lines := strings.Split(string(data), "\n")
	fmt.Printf("\nFirst 10 memory regions:\n")
	for i, line := range lines {
		if i >= 10 || line == "" {
			break
		}
		fmt.Println("  ", line)
	}
}
