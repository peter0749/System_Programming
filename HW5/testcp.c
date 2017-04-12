#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    const char *helpArg="--help"; // 6 characters
    const char *tmpName="./tmpFile";
    int fileInFd=0, fileOutFd=0;
    off_t fileSize=0;
    ssize_t inputSize=0, outputSize=0;
    const size_t BUFFER_SIZE=128*1024; // 128k
    char BUFFER[BUFFER_SIZE];
    struct stat statbuf;
    if (argc !=3 || argc == 2 && strncmp(argv[1],helpArg,6)==0 ) {
        fprintf(stderr,"Usage: acp source_file target_file\n");
    }
    fileInFd = open(argv[1], O_RDONLY);
    if (fileInFd == -1) {
        perror("Error open input file");
        exit(1);
    }
    stat(argv[1], &statbuf);
    fileOutFd = open(tmpName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR| S_IWUSR);
    if (fileOutFd == -1) {
        perror("Error open output file");
        exit(2);
    }
    fileSize = lseek(fileInFd, 0, SEEK_END);
    lseek(fileInFd, 0, SEEK_SET);
    lseek(fileOutFd, 0, SEEK_SET);
    while ( (inputSize = read(fileInFd, BUFFER, BUFFER_SIZE) ) > 0) {
        outputSize = write(fileOutFd, BUFFER, inputSize);
        if (outputSize != inputSize) {
            perror("input_size != output_size");
            fprintf(stderr,"size: %d vs %d\n",inputSize, outputSize);
            exit(3);
        }
    }
    close(fileInFd); close(fileOutFd);
#ifndef Release
    getchar(); // for observation
#endif
    if (rename(tmpName, argv[2]) < 0)
        perror("Error when moving");
    chmod(argv[2], statbuf.st_mode);
    return (EXIT_SUCCESS);
}
