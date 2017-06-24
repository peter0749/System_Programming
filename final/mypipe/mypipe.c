#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *parentExeArgs[] = { NULL, NULL, NULL };
    char *childExeArgs[] = { NULL, NULL, NULL };
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
        parentExeArgs[0] = argv[3];
        parentExeArgs[1] = argv[4];
        execvp(parentExeArgs[0], parentExeArgs);
    }
    else { // child ( fork() == 0)
        close(pipefd[0]);
        dup2(pipefd[1], 1); // -> stdout
        close(pipefd[1]);
        childExeArgs[0] = argv[1];
        childExeArgs[1] = argv[2];
        execvp(childExeArgs[0], childExeArgs);
    } 
    return EXIT_SUCCESS;
}
