#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int main(void) {
    const size_t BUFF_LEN=4096;
    const size_t Account_LEN=256;
    const size_t Score_SIZE=12;
    const char *FILE_NAME="grade";
    char buff[BUFF_LEN];
    char Account[Account_LEN];
    int scores[Score_SIZE];
    int fd=0;
    int data_Len=0;
    int lock_status=0;

    while(fgets( buff, BUFF_LEN-32, stdin )!=NULL) {
        fd = open ( FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU );
        if(fd<0) perror("Failed to openfile");
        sscanf(buff, "%s %d %d %d %d %d", Account, scores, scores+1, scores+2, scores+3, scores+4);
        sprintf(buff, "%s %d %d %d %d %d\n", Account, scores[0], scores[1], scores[2], scores[3], scores[4]);
        data_Len = strlen(buff);
        lock_status = flock(fd, LOCK_EX);
#ifdef WATCH_LOCK
        printf("This process got the lock!\n");
#endif
        if(lock_status!=0) perror("Lock: ");
#ifdef WATCH_LOCK
        int i=5;
        printf("Counting down");
        fflush(stdout);
        while(i) {
            printf("...%d", i);
            fflush(stdout);
            sleep(1);
            --i;
        }
        printf("\n");
#endif
        write(fd, buff, data_Len);
        close(fd);
#ifdef WATCH_LOCK
        printf("File has been written.\nUnlocked\n");
#endif
    }
    return 0;
}
