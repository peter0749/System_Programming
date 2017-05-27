#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
    pid_t pid=0;
    FILE *fp=NULL;
    int fd=0;
    fork();
    fork();
    fork();
    fork();
    pid = getpid();
    printf("%d\n", (int)pid);
    return 0;
}
