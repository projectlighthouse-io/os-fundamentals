// fork_basic.c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("before fork: PID %d\n", getpid());

    pid_t pid = fork();

    printf("after fork: PID %d, fork returned %d\n", getpid(), pid);
    return 0;
}
