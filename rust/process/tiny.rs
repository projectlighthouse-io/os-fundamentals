// tiny.rs
// minimal program to observe VSZ vs RSS in Rust.
// no GC, no runtime heap pre-mapping -- VSZ will be ~4-8 MB, RSS ~2-3 MB.
// compare with the Go version to see the difference.
//
// usage:
//   rustc -o tiny_rs tiny.rs
//   ./tiny_rs &
//   ps -o pid,vsz,rss,comm -p $!
//   kill $!
use std::{thread, time::Duration};

fn main() {
    println!("hello");
    thread::sleep(Duration::from_secs(30));
}
