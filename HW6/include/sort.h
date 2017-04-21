#ifndef INCLUDE_SORT
#define INCLUDE_SORT
#include "global.h"
#include "String.h"

void sort_fileList (String *files, size_t n) {
    // note: n must valid
    qsort (files, n, sizeof(String), cmpString);
}
#endif
