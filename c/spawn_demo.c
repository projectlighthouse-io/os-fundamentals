// spawn_demo.c
#include <spawn.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

extern char **environ;

int main() {
    pid_t pid;
    char *args[] = {"ls", "-la", "/tmp", NULL};

    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);

    int ret = posix_spawn(&pid, "/bin/ls", &actions, NULL, args, environ);
    if (ret != 0) {
        fprintf(stderr, "posix_spawn: %s\n", strerror(ret));
        return 1;
    }

    int status;
    waitpid(pid, &status, 0);
    posix_spawn_file_actions_destroy(&actions);
    printf("[exited %d]\n", WEXITSTATUS(status));
    return 0;
}
