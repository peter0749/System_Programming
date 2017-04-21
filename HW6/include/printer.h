#ifndef INCLUDE_PRINTER
#define INCLUDE_PRINTER
#include "global.h"
void printer (char *fileName, unsigned int depth) {
    int i=0;
    for (i=0; i<depth; ++i) printf("  ");
    printf("-%d> %s\n", depth, fileName); // note that: fileName must ends with \0
}
// printer.c
#endif
