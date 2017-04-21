#ifndef STRUCT_STRING
#define STRUCT_STRING
#include "minmaxutils.h"
typedef struct {
    int length;
    char *str;
}String;

typedef struct {
    int size;
    String *data;
}StringStack;

int cmpString (const void *a, const void *b) {
    const String *first = (const String*)a;
    const String *second = (const String*)b;
    return strncmp (first->str,second->str, MIN(first->length, second->length));
}

void des_String(String *str) {
    if (str->str==NULL) return;
    free(str->str); str->str=NULL;
    str->length=0;
}

void init_StringStack(StringStack *stk) {
    stk->size=0;
    stk->data=NULL;
}

void des_StringStack(StringStack *stk) {
    if (stk->data == NULL ) return;
    for (int i=0; i<stk->size; ++i) {
        des_String(&stk->data[i]);
    }
    free(stk->data); stk->data=NULL;
    stk->size=0;
}

StringStack* push_StringStack(StringStack *stk, const char *str) {
    String *temp = (String*)malloc(sizeof(String)*(stk->size+1));
    int i=0;
    for (i=0; i<stk->size; ++i) {
        temp[i].str = stk->data[i].str;
        temp[i].length = stk->data[i].length;
    }
    temp[i].str = (char*)malloc(strlen(str)+1);
    strcpy(temp[i].str, str);
    temp[i].length = strlen(temp[i].str);
    free(stk->data);
    stk->data = temp;
    ++stk->size;
    return stk;
}
#endif
