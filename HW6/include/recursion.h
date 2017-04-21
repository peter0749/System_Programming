#ifndef INCLUDE_RECURSION
#define INCLUDE_RECURSION
#include "global.h"
#include "watcher.h"
#include "printer.h"
#include "String.h"

void recursion(StringStack *stk, char *dirName, int crr_depth, const int max_depth, char p) {
    if (crr_depth == max_depth) return;
    DIR *dir=NULL;
    struct dirent* ent=NULL;
    struct stat statBuf;
    char *pathName = NULL;
    dir = opendir(dirName);
    if (dir==NULL) {
        perror("error");
        return;
    }
    ent = readdir(dir);
    while (ent!=NULL) {
        pathName = NULL;
        pathName = (char*)malloc(sizeof(char)*(strlen(dirName)+strlen(ent->d_name)+3));
        if (pathName==NULL) {
            fprintf(stderr,"No Memory!");
            return;
        }
        strcpy(pathName, dirName);
        strcat(pathName, "/");
        strcat(pathName, ent->d_name);
        stat(pathName, &statBuf);
        //getchar();
        push_StringStack(stk, pathName);
        if(p) printer(pathName, crr_depth+1);
        if(S_ISDIR(statBuf.st_mode) && strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 ) {
            recursion (stk, pathName, crr_depth+1, max_depth, p);
        }
        free (pathName); pathName=NULL;
        ent = readdir(dir);
    }
    closedir(dir);
}

#endif
