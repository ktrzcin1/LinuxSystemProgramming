#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


void* sig_thread_routine (void *data){
    
    int s, num;
    //get signalmask
    sigset_t* signals = (sigset_t*) data;
    //listen form signals
    while(1)
    {
        s = sigwait(signals, &num);
        if (s != 0)
        handle_error_en(s, "Signal handler.");
        printf("Handling signal:\t%d\n", num);
    }
}

int
main(void){
    //create mask of signals
    sigset_t sig_mask;
    sigfillset(&sig_mask);
    int e;
    //set mask of signals
    e =pthread_sigmask(SIG_BLOCK, &sig_mask, NULL);
    if (e != 0)
        handle_error_en(e, "Signal mask.");
    //create thread
    pthread_t sig_thread;
    e = pthread_create(&sig_thread, NULL, &sig_thread_routine, (void *) &sig_mask);
    if (e != 0)
        handle_error_en(e, "Thread create.");

    //print PID and pause.
    printf("PID:%8d\n", getpid());
    pause();
    return 0;
}