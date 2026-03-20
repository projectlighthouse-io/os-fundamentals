.PHONY: all clean go rust

all: go rust

go:
	cd go/process && go build -o ../../bin/go/tiny tiny.go
	cd go/process && go build -o ../../bin/go/process_layout process_layout.go

rust:
	rustc -o bin/rust/tiny rust/process/tiny.rs
	rustc -o bin/rust/process_layout rust/process/process_layout.rs
	rustc -o bin/rust/inspector rust/process/inspector.rs

clean:
	rm -rf bin/
