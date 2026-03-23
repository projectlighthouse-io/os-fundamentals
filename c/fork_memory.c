// fork_memory.c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int x = 42;

    pid_t pid = fork();

    if (pid == 0) {
        x = 99;
        printf("child:  x = %d at address %p\n", x, &x);
        _exit(0);
    }

    wait(NULL);
    printf("parent: x = %d at address %p\n", x, &x);
    return 0;
}
