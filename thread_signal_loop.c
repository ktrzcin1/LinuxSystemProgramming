#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>

int global[10];
typedef struct {
    int threads_count;
    pthread_t* next;
    int thread_id;

} thread_data;

void *
thread_func(void * arg)
{
    thread_data* data = (thread_data*) arg;
    printf("Thread #%d running.\n", data->thread_id);

    sigset_t sig_mask;
    sigemptyset(&sig_mask);
    sigaddset(&sig_mask, SIGUSR1);

    int sig_num;
    while(1){
        sigwait(&sig_mask, &sig_num);
        printf("Thread %d recieved signal %d.\n", data->thread_id, sig_num);
        printf("Sending signal to next thread.\n");
        sleep(1);
        pthread_kill(*(data->next), SIGUSR1);
    }
    return NULL;
}

int main(int argc, char** argv){
    //SIGUSR1 as signal to throw between threads.

    printf("PID: %d\n", getpid());
    int threads_count = 10;
    //MASK
    sigset_t sig_mask;
    sigemptyset(&sig_mask);
    sigaddset(&sig_mask, SIGUSR1);
    pthread_sigmask(SIG_SETMASK, &sig_mask, NULL);
    //every thread blocks this signal
    //MASK

    //THREAD
    //Thread Array
    pthread_t** thread_pointers = (pthread_t**) malloc(sizeof(pthread_t*));
    int i = 0;
    for(i=0; i < threads_count; i++)
    {
        thread_pointers[i] = (pthread_t*) malloc(sizeof(pthread_t));
    }
    
    //Create Threads
    for(i=0; i < threads_count; i++)
    {
        int next_thread_id = (i+1)%threads_count;
        thread_data *data = (thread_data*) malloc(sizeof(thread_data));
        data->thread_id = i;
        data->next = thread_pointers[next_thread_id];
        data->threads_count = threads_count;
        

        pthread_create(thread_pointers[i], NULL, thread_func, (void*) data);
    }
    

    printf("Main sleep...\n");
    sleep(3);
    printf("Sending signal to first thread.\n");
    sleep(1);
    pthread_kill(*thread_pointers[0], SIGUSR1);
    sleep(3);
    pause();
    printf("Exit.\n");
    return 0;
}
