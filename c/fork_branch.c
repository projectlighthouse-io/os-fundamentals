// fork_branch.c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("I am the child.  PID %d, parent %d\n", getpid(), getppid());
    } else {
        printf("I am the parent. PID %d, child %d\n", getpid(), pid);
    }

    return 0;
}
