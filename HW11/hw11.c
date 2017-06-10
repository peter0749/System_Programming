#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *const childExeArgs[] = { "wc", NULL };
    char *parentExeArgs[] = { "cat", NULL, NULL };
    int pipefd[2];
    if (argc<2) {
        fprintf(stderr, "Please enter filepath!\n");
        fprintf(stderr, "Usage: %s filepath\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pipe(pipefd);

    pid_t pid = fork();
    if(pid != 0) {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        printf("%4cline%4cword%4csize\n", ' ', ' ', ' ');
        execv("/usr/bin/wc", childExeArgs);
        //execlp("wc", "wc", "-l", NULL);
    }
    else {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        parentExeArgs[1] = argv[1]; // copy filepath pointer
        execv("/bin/cat", parentExeArgs);
        //execlp("ls", "ls", NULL);
    } 
    return EXIT_SUCCESS;
}
