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
    int chunk_size;
}_THREAD_ARGS;

double GEN_RAND(unsigned int *seed) {
    return (double)rand_r(seed)/(double)RAND_MAX; // generate random float [0,1]
}

void *PI_KERNEL_FUNC( void* __ARGS) {
    _THREAD_ARGS *p = (_THREAD_ARGS*)__ARGS;
    int chunk_size = p->chunk_size;
    unsigned int seed = 0xDEADBEEF; 
    unsigned int i=0, local_total=0; // local vars
    unsigned int *local_in = NULL;
    local_in = (unsigned int*)malloc(sizeof(unsigned int));
    double x=0.0,y=0.0;
    local_total = chunk_size;
    fprintf(stderr,"Hi! worker %d, there are %d numbers to compute.\n", p->tid, local_total);
    seed = time(NULL);
    for (i=0; i<local_total; ++i) {
        x = GEN_RAND(&seed);
        y = GEN_RAND(&seed);
        *local_in += (x*x+y*y < 1.0)?1:0;
    }
    fprintf(stderr,"Bye! worker %d, the result is %lf.\n", p->tid, (double)(*local_in)/(double)local_total*4.0);
    pthread_exit((void*)local_in);
}

int main(int argc, char *argv[]) {
    if (argc<3) {
        fprintf(stderr,"Please enter #loop and #thread!\n");
        exit(EXIT_FAILURE);
    }
    int i=0, j=0, total=0, in=0, chunk_size=0;
    void *return_status=NULL;
    int loopN = atoi(argv[1]);
    int threadN = atoi(argv[2]);
    _THREAD_ARGS *args=NULL;
    struct timeval st, et;
    struct timezone stz, etz;
    if (loopN<1||threadN<1) {
        fprintf(stderr,"Wrong arguments! Only if #loop>=1 and #thread>=1 is valid.\n");
        exit(EXIT_FAILURE);
    }
    unsigned int seed = time(NULL);
    chunk_size = (loopN+threadN-1)/threadN; // chunk*threadN >= loopN
    total = chunk_size*threadN; // total area
    in    = 0;
    printf("chunk_size: %d\nthreadN: %d\n", chunk_size, threadN);
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
        pthread_join(threads[i], &return_status); // wait thread i terminate
        assert(return_status!=NULL);
        in += *((int*)return_status);
        free(return_status); // destroy the malloced result
        return_status = NULL;
    }
    gettimeofday(&et, &etz);
    free(threads);
    free(args);
    printf("Estimated PI: %lf\n", (double)in/(double)total*4.0);
    printf("Elapsed time: %lf seconds\n", (double)(et.tv_sec*1e6+et.tv_usec-st.tv_sec*1e6-st.tv_usec)/(double)1e6);
    return EXIT_SUCCESS;
}
