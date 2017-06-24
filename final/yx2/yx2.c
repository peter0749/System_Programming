#include <sys/time.h>
#include <sys/signalfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <assert.h>

typedef struct {
    int tid;
    unsigned long long int chunk_size;
    unsigned long long int progress;
}_THREAD_ARGS;

typedef struct {
    int thread_n;
    unsigned long long int total;
    _THREAD_ARGS *thread_args;
}_WATCHER;

// NO MUTEX LOCK IS NEEDED

double GEN_RAND(unsigned int *seed, double range) {
    return (double)rand_r(seed)*range/(double)RAND_MAX; // generate random float [0,1]
}

void *PI_KERNEL_FUNC( void* __ARGS) {
#define INLINE_FX(x) (x)*(x)
    _THREAD_ARGS *p = (_THREAD_ARGS*)__ARGS;
    unsigned long long int chunk_size = p->chunk_size;
    unsigned int seed = 0xDEADBEEF+p->tid;  // no clock() or time() is needed
    unsigned long long int i=0, local_total=0; // local vars
    long double x=0.0L,y=0.0L;
    local_total = chunk_size;
    p->chunk_size = 0;
    for (i=0; i<local_total; ++i) {
        x = GEN_RAND(&seed,8.0);
        y = GEN_RAND(&seed,64.0);
        p->chunk_size += ( y < INLINE_FX(x) )?1:0; // use == ?
        ++p->progress;
    }
    pthread_exit(NULL);
#undef INLINE_FX
}

void *READ_KERNEL(void *CHECKER) {
#define what_err(msg) \
    { perror(msg); exit(EXIT_FAILURE); }
    _WATCHER *p = (_WATCHER*)CHECKER;
    _THREAD_ARGS *kernel_args = p->thread_args;
    int thread_n = p->thread_n;
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo fdsi; // Use to receive signal informations
    ssize_t s;
	fprintf(stderr, "Listener is ready!\n");
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);// Ctrl+Z (Stop)
    /* Block signals so that they aren't handled
       according to their default dispositions */

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        what_err("sigprocmask");

    sfd = signalfd(-1, &mask, 0); // listen new signal fd
    if (sfd == -1)
        what_err("signalfd");

    while(1) {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo)); // Okay, Blocking is seted
        if (s != sizeof(struct signalfd_siginfo)) // Abnormal readed
            what_err("read");

        // Print which signal read
        switch (fdsi.ssi_signo) {
            case SIGTSTP: {
                int i=0;
                unsigned long long int tot=0, tot_progress=0;
                for (i=0; i<thread_n; ++i) {
                    tot += kernel_args[i].chunk_size;
                    tot_progress += kernel_args[i].progress;
                }
                fprintf(stderr, "Dots in the area: %llu / %llu , %d/100\n", tot, p->total, (int)((double)tot_progress/(double)p->total * 100.0));
            }
            break;
            default:
                printf("Something strange...\n");
            break;
        }
    }
	//fprintf(stderr, "exited\n");
#undef what_err
}

int main(int argc, char *argv[]) {
    if (argc<3) {
        fprintf(stderr,"Please enter #loop and #thread!\n");
        exit(EXIT_FAILURE);
    }
    unsigned long long int i=0, j=0, in=0, chunk_size=0;
    unsigned long long int total=0;
    unsigned long long int loopN = (unsigned long long int)atoll(argv[1]);
    int threadN = atoi(argv[2]);
    _THREAD_ARGS *args=NULL;
    struct timeval st, et;
    struct timezone stz, etz;
    if (loopN<1L||threadN<1) {
        fprintf(stderr,"Wrong arguments! Only if #loop>=1 and #thread>=1 is valid.\n");
        exit(EXIT_FAILURE);
    }
    sigset_t mask;
	assert(sigprocmask(SIG_BLOCK, &mask, NULL)!=-1);
    chunk_size = (loopN+(unsigned long long int)threadN-1L)/(unsigned long long int)threadN; // chunk*threadN >= loopN
    total = chunk_size*(unsigned long long int)threadN; // total area
    in    = 0L;
    printf("chunk_size: %llu\nthreadN: %d\ntotal: %llu\n", chunk_size, threadN, total);
    
    pthread_t watcher;
    _WATCHER watcher_args;

    pthread_t *threads = NULL;

    threads = (pthread_t*)malloc(sizeof(pthread_t)*threadN);
    assert(threads!=NULL);

    args = (_THREAD_ARGS*)malloc(sizeof(_THREAD_ARGS)*threadN);
    assert(args!=NULL);

    watcher_args.total = total;
    watcher_args.thread_n = threadN;
    watcher_args.thread_args = args;

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
	fprintf(stderr,"All thread created.\n");
	
    int re = pthread_create(&watcher, NULL, READ_KERNEL, (void*)&watcher_args);
    if (re) exit(EXIT_FAILURE);

    // wait all threads terminate
    for (i=0; i<threadN; ++i) {
        pthread_join(threads[i], NULL); // wait thread i terminate
        in += args[i].chunk_size;
    }
    //pthread_join(watcher, NULL); // not wait watcher thread exits
    gettimeofday(&et, &etz);
    free(threads);
    free(args);
    printf("Estimated Integral: %.8LF\n", (long double)in/(long double)total*512.0);
    printf("Elapsed time: %lf seconds\n", (double)(et.tv_sec*1e6+et.tv_usec-st.tv_sec*1e6-st.tv_usec)/(double)1e6);
    return EXIT_SUCCESS;
}
