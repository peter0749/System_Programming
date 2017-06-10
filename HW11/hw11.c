#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *const parentExeArgs[] = { "wc", NULL };
    char *childExeArgs[] = { "cat", NULL, NULL };
    int pipefd[2];
    if (argc<2) {
        fprintf(stderr, "Please enter filepath!\n");
        fprintf(stderr, "Usage: %s filepath\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pipe(pipefd);

    pid_t pid = fork();
    if(pid != 0) { // parent
        close(pipefd[1]);
        dup2(pipefd[0], 0); // -> stdin
        close(pipefd[0]);
        printf("%4cline%4cword%4csize\n", ' ', ' ', ' ');
        execv("/usr/bin/wc", parentExeArgs);
    }
    else { // child ( fork() == 0)
        close(pipefd[0]);
        dup2(pipefd[1], 1); // -> stdout
        close(pipefd[1]);
        childExeArgs[1] = argv[1]; // copy filepath pointer
        execv("/bin/cat", childExeArgs);
    } 
    return EXIT_SUCCESS;
}
