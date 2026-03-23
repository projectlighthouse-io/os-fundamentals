// orphan_demo.c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        sleep(2);
        printf("child (%d): my parent is now %d\n", getpid(), getppid());
        return 0;
    }

    printf("parent (%d) exiting. child %d will be orphaned.\n", getpid(), pid);
    return 0;
}
