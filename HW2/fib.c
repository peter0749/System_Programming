#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int LLINT; // Storage data type
void Fib(LLINT , LLINT, size_t , LLINT *); // Function to compute Fibonacci
void FibPrint(size_t, size_t, LLINT *); // Print Fib terms [from , to)
void WrongArg(void);  // puts warning message
void showHelp(void);  // puts help message

int main(int argc, char **argv) {
    int i=0, Size=0;
    LLINT *data=NULL;
    if(argc!=4) { // There must be four arguments
        // If the format is not match, then print warning message.
        WrongArg(); 
        // Program exit unusually
        exit(1);
    }
    Size=atoi(argv[3]);
    if(Size<1) exit(2); // Check the number of terms
    data = (LLINT*)malloc(sizeof(LLINT)*Size); // allocate Fibonacci mem
    Fib(0LL, 1LL, Size, data); // Interative build fib. series
    FibPrint(0, Size, data);   // Print fib
    for(i=0; i!=Size; ++i) data[i]=0LL; // Clean-up memory
    free(data); data=NULL; // Release the memory
    return 0; // The program exit normally.
}


void Fib(LLINT first, LLINT second, size_t Size, LLINT *data) {
    int i=2;
    data[0]=first, data[1]=second;
    if(Size<2) return;
    for(i=2; i!=Size; ++i) data[i] = data[i-1]+data[i-2];
}


void FibPrint(size_t from, size_t to, LLINT *data) {
    // Print all terms.
    int i=from;
    if(from<0||from>=to) return;
    for(i=from; i!=to; ++i) printf("Fib_%d:\t%lld\n", i, data[i]);
}

void showHelp(void) {
    const char *warnMsg="Usage: ./fib first second terms\n";
    fputs(warnMsg,stderr);
}
void WrongArg(void) {
    fputs("Please enter proper arguments...\n",stderr);
    showHelp();
}
