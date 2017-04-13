#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseHomeDir (char *str, const char *src, char *buf, const char *home) {
    if (src!=NULL && *src=='~') {
        strcpy(buf, home);
        strcat(buf, "/");
        strcat(buf, src);
        strcpy(str, buf);
    } else {
        strcpy(str, src);
    }
}

int main(int argc, char **argv) {
    const char *helpArg="--help"; // 6 characters
    const char *tmpName="./tmpFile";
    int fileInFd=0, fileOutFd=0;
    ssize_t inputSize=0, outputSize=0;
    const size_t BUFFER_SIZE=128*1024; // 128k
    char BUFFER[BUFFER_SIZE];
    char sourcePath[BUFFER_SIZE], targetPath[BUFFER_SIZE];
    struct stat statbuf;
    struct passwd *passwdEnt = getpwuid(getuid());
    char *home = passwdEnt->pw_dir;
    if (argc !=3 || ( argc == 2 && strncmp(argv[1],helpArg,6)==0) ) {
        fprintf(stderr,"Usage: acp source_file target_file\n");
        exit(1);
    }
    parseHomeDir(sourcePath, argv[1], BUFFER, home);
    parseHomeDir(targetPath, argv[2], BUFFER, home);
    fileInFd = open(sourcePath, O_RDONLY);
    if (fileInFd == -1) {
        perror("Error open input file");
        exit(2);
    }
    stat(argv[1], &statbuf);
    fileOutFd = open(tmpName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR| S_IWUSR);
    if (fileOutFd == -1) {
        perror("Error open output file");
        exit(3);
    }
    lseek(fileInFd, 0, SEEK_SET);
    lseek(fileOutFd, 0, SEEK_SET);
    while ( (inputSize = read(fileInFd, BUFFER, BUFFER_SIZE) ) > 0) {
        outputSize = write(fileOutFd, BUFFER, inputSize);
        if (outputSize != inputSize) {
            perror("input_size != output_size");
            fprintf(stderr,"size: %d vs %d\n",(int)inputSize, (int)outputSize);
            exit(4);
        }
    }
    close(fileInFd); close(fileOutFd);
#ifndef Release
    getchar(); // for observation
#endif
    if (rename(tmpName, targetPath) < 0)
        perror("Error when moving");
    chmod(argv[2], statbuf.st_mode);
    return (EXIT_SUCCESS);
}
