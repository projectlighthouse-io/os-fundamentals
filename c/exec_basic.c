// exec_basic.c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("about to exec. PID: %d\n", getpid());

    char *args[] = {"ls", "-la", "/tmp", NULL};
    execvp("ls", args);

    printf("this line never executes on success\n");
    perror("exec failed");
    return 1;
}
