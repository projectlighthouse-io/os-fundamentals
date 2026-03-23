// fork_cow.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

long get_minor_faults(void) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_minflt;
}

int main() {
    size_t size = 100 * 1024 * 1024;  // 100 MB
    char *buffer = malloc(size);
    memset(buffer, 'A', size);         // fault in all pages

    pid_t pid = fork();

    if (pid == 0) {
        long before = get_minor_faults();
        printf("  [child] minor faults before writes: %ld\n", before);

        memset(buffer, 'B', size / 2);    // write to 50 MB
        long after_half = get_minor_faults();
        printf("  [child] minor faults after 50 MB:   %ld (+%ld COW copies)\n",
               after_half, after_half - before);

        memset(buffer + size / 2, 'C', size / 2);  // write remaining 50 MB
        long after_all = get_minor_faults();
        printf("  [child] minor faults after 100 MB:  %ld (+%ld COW copies)\n",
               after_all, after_all - after_half);

        _exit(0);
    }

    wait(NULL);
    free(buffer);
    return 0;
}
