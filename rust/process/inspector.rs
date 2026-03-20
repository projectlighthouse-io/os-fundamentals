// inspector.rs
// reads /proc/<pid>/ and prints a human-readable process summary.
// this is the exercise from the "what is a process" chapter.
//
// usage:
//   rustc -o inspector inspector.rs
//   ./inspector $$
use std::env;
use std::fs;

fn main() {
    let pid = match env::args().nth(1) {
        Some(p) => p,
        None => {
            eprintln!("usage: inspector <pid>");
            std::process::exit(1);
        }
    };

    let base = format!("/proc/{}", pid);

    // process name
    let comm = read_proc_file(&base, "comm");

    // parse status fields
    let status = read_proc_file(&base, "status");
    let state = parse_status_field(&status, "State:");
    let ppid = parse_status_field(&status, "PPid:");
    let vm_size = parse_status_field(&status, "VmSize:");
    let vm_rss = parse_status_field(&status, "VmRSS:");

    // count open file descriptors
    let fd_count = match fs::read_dir(format!("{}/fd", base)) {
        Ok(entries) => entries.count(),
        Err(_) => 0,
    };

    // parse memory regions
    let maps = read_proc_file(&base, "maps");
    let (exec, rw, ro, other) = count_vmas(&maps);

    println!("Process {}: {}", pid, comm.trim());
    println!("  State:    {}", format_state(state.trim()));
    println!("  Parent:   {}", ppid.trim());
    println!(
        "  Memory:   {} virtual, {} resident",
        vm_size.trim(),
        vm_rss.trim()
    );
    println!("  Open FDs: {}", fd_count);
    println!(
        "  VMAs:     {} executable, {} read-write, {} read-only, {} other",
        exec, rw, ro, other
    );
}

fn read_proc_file(base: &str, name: &str) -> String {
    let path = format!("{}/{}", base, name);
    fs::read_to_string(&path).unwrap_or_else(|e| {
        eprintln!("failed to read {}: {}", path, e);
        std::process::exit(1);
    })
}

fn parse_status_field<'a>(status: &'a str, field: &str) -> &'a str {
    for line in status.lines() {
        if line.starts_with(field) {
            return line[field.len()..].trim();
        }
    }
    "unknown"
}

fn format_state(state: &str) -> String {
    let description = match state.chars().next() {
        Some('R') => "Running",
        Some('S') => "Sleeping (waiting for event)",
        Some('D') => "Disk sleep (uninterruptible)",
        Some('T') => "Stopped",
        Some('Z') => "Zombie",
        Some('I') => "Idle (kernel thread)",
        _ => "Unknown",
    };
    format!("{} ({})", description, state)
}

fn count_vmas(maps: &str) -> (usize, usize, usize, usize) {
    let mut exec = 0;
    let mut rw = 0;
    let mut ro = 0;
    let mut other = 0;

    for line in maps.lines() {
        let perms: &str = match line.split_whitespace().nth(1) {
            Some(p) => p,
            None => continue,
        };

        if perms.contains('x') {
            exec += 1;
        } else if perms.starts_with("rw") {
            rw += 1;
        } else if perms.starts_with("r-") {
            ro += 1;
        } else {
            other += 1;
        }
    }

    (exec, rw, ro, other)
}
