// process_layout.rs
// prints addresses of text, data, BSS, heap, and stack segments.
// run this and then compare with `cat /proc/<pid>/maps` in another terminal.
//
// addresses will differ between runs due to ASLR (Address Space Layout Randomization),
// but the relative ordering is always: text < data < BSS < heap, stack way up high.
//
// usage:
//   rustc -o process_layout process_layout.rs
//   ./process_layout
use std::io::{self, Read};

static GLOBAL_INIT: i32 = 42;    // data segment
static GLOBAL_UNINIT: i32 = 0;   // zero-initialized — typically placed in BSS

fn print_region(label: &str, addr: usize) {
    println!("  {:<20} {:#x}", label, addr);
}

fn main() {
    let stack_var: i32 = 99;
    let heap_var = Box::new([0i32; 256]);

    let pid = std::process::id();
    println!("PID: {pid}\n");
    println!("Memory layout (low to high):");
    print_region("Text (main):", main as usize);
    print_region("Data (GLOBAL_INIT):", &GLOBAL_INIT as *const _ as usize);
    print_region("BSS (GLOBAL_UNINIT):", &GLOBAL_UNINIT as *const _ as usize);
    print_region("Heap (Box'd):", heap_var.as_ptr() as usize);
    print_region("Stack (local var):", &stack_var as *const _ as usize);

    println!("\nVerify with: cat /proc/{pid}/maps");
    println!("Press Enter to exit...");
    let _ = io::stdin().read(&mut [0u8]);
}
