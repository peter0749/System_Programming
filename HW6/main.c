#include "include/global.h"
#include "include/sort.h"
#include "include/printer.h"
#include "include/recursion.h"
#include "include/watcher.h"

#define BUF_LEN 8192
#define showHelp(arg0) \
    fprintf(stderr, "usage: %s DirName [maxdepth (default:3)] [-np not to printout filepaths]\n", arg0);\
fprintf(stderr, "use \"%s --help\" to print this help.\n", arg0);

int main(int argc, char **argv) {
    char *dirName=NULL;
    StringStack stack;
    int fd=0;
    int maxDepth=3;
    char printList=1;
    if (argc<2) {
        fprintf(stderr, "Please enter at least 1 argument.\n");
        showHelp(argv[0]);
        exit(1);
    } 
    if (argc>=3)
        maxDepth = atoi(argv[2]);
    if (argc>=4 && strcmp(argv[3],"-np")==0)
        printList = 0;
    if (strcmp(argv[1],"--help")==0) {
        showHelp(argv[0]);
        return 0;
    }
    dirName = (char*)malloc(strlen(argv[1])+5);
    if (argv[1][0]!='/') {
        strcat(dirName, "./");
        strcat(dirName, argv[1]);
    } else strcpy(dirName, argv[1]);

    // Now, implement watcher
    init_StringStack(&stack);
    push_StringStack(&stack, dirName);
    if (printList) printf("====== DIR STRUCTURE ======\n");
    recursion(&stack, dirName, 0, maxDepth, printList);
    sort_fileList (stack.data, stack.size);
    if (printList) {
        int i=0;
        printf("====== SORTED ======\n");
        for (i=0; i<stack.size; ++i) printf("%s\n", stack.data[i].str);
        printf("====== END ======\n");
    }
    printf("====== INOTIFY ======\n");
    watcher_mainLoop(&stack, BUF_LEN);
    des_StringStack(&stack);
    // End of watcher
    if (dirName!=NULL) {
        free(dirName);
        dirName=NULL;
    }
    close(fd);
    return 0;
}
