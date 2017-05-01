#ifndef INCLUDE_WATCHER
#define INCLUDE_WATCHER
#include "global.h"
#include "String.h"
void watcher_mainLoop (StringStack *files, const int BUF_LEN) {
    int fd=0;
    char *BUFFER=NULL;
    int i=0;
    fd = inotify_init();
    if (fd < 0) {
        perror("error");
        return;
    }
    BUFFER = (char*)malloc(BUF_LEN+1);
    if (BUFFER==NULL) {
        fprintf(stderr,"Out of memory!\n");
        return;
    }
    for (i=0; i<files->size; ++i) {
        if( inotify_add_watch(fd, files->data[i].str, IN_ALL_EVENTS) < 0 ) {
            perror("error");
        }
    }
    int num=0;
    char *p=NULL;
    struct inotify_event *event=NULL;
    while(1) {
        num = read(fd, BUFFER, BUF_LEN);
        if (num<0) {
            perror("error");
            return;
        }
        for (p=BUFFER; p<BUFFER+num; ) {
            event = (struct inotify_event *)p;
            if (event->cookie & IN_MOVED_FROM) printf("File: %s is moved from somewhere\n", event->name);
            if (event->cookie & IN_MOVED_TO  ) printf("File: %s is moved to somewhere\n", event->name);
            if (event->mask &   IN_ACCESS    ) printf("File: %s is accessed\n", event->name);
            if (event->mask &   IN_CREATE    ) {
                printf("File: %s is createed\nI'm watching it...\n", event->name);
            }
            if (event->mask &   IN_DELETE    ) printf("File: %s is deleted\n", event->name);
            if (event->mask &   IN_MODIFY    ) printf("File: %s is modified\n", event->name);
            p+=sizeof(struct inotify_event)+event->len;
        }
    }
    free(BUFFER); BUFFER=NULL;
}
#endif
