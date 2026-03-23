// zombie_demo.c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("child (%d) exiting immediately\n", getpid());
        _exit(0);
    }

    printf("parent (%d) sleeping 30s. child %d is now a zombie.\n",
           getpid(), pid);
    sleep(30);
    return 0;
}
