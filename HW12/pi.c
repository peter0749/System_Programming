#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <assert.h>

typedef struct {
    int tid;
    unsigned long long int chunk_size;
}_THREAD_ARGS;


// NO MUTEX LOCK IS NEEDED

double GEN_RAND(unsigned int *seed) {
    return (double)rand_r(seed)/(double)RAND_MAX; // generate random float [0,1]
}

void *PI_KERNEL_FUNC( void* __ARGS) {
    _THREAD_ARGS *p = (_THREAD_ARGS*)__ARGS;
    unsigned long long int chunk_size = p->chunk_size;
    unsigned int seed = clock()^p->tid;  // no clock() or time() is needed
    unsigned long long int i=0, local_total=0; // local vars
    unsigned long long int local_in = 0; 
    long double x=0.0L,y=0.0L;
    local_total = chunk_size;
    fprintf(stderr,"Hi! worker %d, there are %llu numbers to compute.\n", p->tid, local_total);
    for (i=0; i<local_total; ++i) {
        x = GEN_RAND(&seed);
        y = GEN_RAND(&seed);
        local_in += (x*x+y*y <= 1.0L)?1:0; // use == ?
    }
    fprintf(stderr,"Bye! worker %d, the result is %lf.\n", p->tid, (double)(local_in)/(double)local_total*4.0);
    p->chunk_size = local_in; // return this result
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc<3) {
        fprintf(stderr,"Please enter #loop and #thread!\n");
        exit(EXIT_FAILURE);
    }
    unsigned long long int i=0, j=0, total=0, in=0, chunk_size=0;
    unsigned long long int loopN = (unsigned long long int)atoll(argv[1]);
    int threadN = atoi(argv[2]);
    _THREAD_ARGS *args=NULL;
    struct timeval st, et;
    struct timezone stz, etz;
    if (loopN<1L||threadN<1) {
        fprintf(stderr,"Wrong arguments! Only if #loop>=1 and #thread>=1 is valid.\n");
        exit(EXIT_FAILURE);
    }
    chunk_size = (loopN+(unsigned long long int)threadN-1L)/(unsigned long long int)threadN; // chunk*threadN >= loopN
    total = chunk_size*(unsigned long long int)threadN; // total area
    in    = 0L;
    printf("chunk_size: %llu\nthreadN: %d\ntotal: %llu\n", chunk_size, threadN, total);
    pthread_t *threads = NULL;

    threads = (pthread_t*)malloc(sizeof(pthread_t)*threadN);
    assert(threads!=NULL);

    args = (_THREAD_ARGS*)malloc(sizeof(_THREAD_ARGS)*threadN);
    assert(args!=NULL);

    // beginning of computing
    gettimeofday(&st, &stz);
    for (i=0; i<threadN; ++i) {
        args[i].tid=i;
        args[i].chunk_size=chunk_size;
        int re = pthread_create(threads+i, NULL, PI_KERNEL_FUNC, (void*)(args+i));
        if (re) {
            fprintf(stderr,"Failed to create new thread!\n");
            exit(EXIT_FAILURE);
        }
    }
    // wait all threads terminate
    for (i=0; i<threadN; ++i) {
        pthread_join(threads[i], NULL); // wait thread i terminate
        in += args[i].chunk_size;
    }
    gettimeofday(&et, &etz);
    free(threads);
    free(args);
    printf("Estimated PI: %.8LF\n", (long double)in/(long double)total*4.0);
    printf("Elapsed time: %lf seconds\n", (double)(et.tv_sec*1e6+et.tv_usec-st.tv_sec*1e6-st.tv_usec)/(double)1e6);
    return EXIT_SUCCESS;
}
