// minishell.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("mysh> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;
        if (strcmp(line, "exit") == 0)
            break;

        int argc = 0;
        char *tok = strtok(line, " ");
        while (tok && argc < MAX_ARGS - 1) {
            args[argc++] = tok;
            tok = strtok(NULL, " ");
        }
        args[argc] = NULL;

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);
            perror(args[0]);
            _exit(127);
        }

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("[exited %d]\n", WEXITSTATUS(status));
    }

    return 0;
}
