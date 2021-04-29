/*
MULTI THREADING

1.  Forking 
    - Included in <unistd.h>
    - Creates a child PROCESS that runs the remaining code alongside the parent
    - Parent and child use separate code, data, files, registers and stacks

2.  POSIX Threading
    - Included in <pthread.h>
    - Creates a thread in the SAME MEMORY SPACE
    - Threads use SAME code, data, files, but SEPARATE registers and stacks
    - Run with time-slocing on single-core, run in parallel on multi-core

    Functions & References
    - pthread_t     -->     Data type for declaring thread
    - pthread_create(pthread_t *restrict thread,                // thread name
                        const pthread_attr_t *restrict attr,    // attributes (usually NULL)
                        void *(*start_routine)(void *),         // function to thread
                        void *restrict arg);                    // argument for function
    - pthread_join(pthread_t thread,    // thread to WAIT for before continuing
                    void **retval)      // exit status pointer (usually NULL)
    - pthread_exit(void *retval)        // Exit calling thread while other threads run
*/

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

void forking()
{
    printf("FORKING\n");
    printf("Hello world!\n");
    pid_t ch_pid = fork();
    fork();
    fork();
    if (ch_pid != 0) {  // parent is main pid
        printf("Hello world from parent!\n");
    }
    else {  // child is 0
        printf("Hello world from child!\n");
    }
    printf("Hello world from both!\n");
}

static void *print_msg(void *ptr)
{ 
    char *text;
    text = (char *) ptr;
    printf("%s \n", text);
}

void threading()
{
    printf("THREADING\n");
    pthread_t thread1, thread2;

    char *str1 = "I am thread 1";
    char *str2 = "I am thread 2";
    int T1ret, T2ret;

    /* Create two threads */
    T1ret = pthread_create(&thread1, NULL, print_msg, (void*) str1);
    T2ret = pthread_create(&thread2, NULL, print_msg, (void*) str2);

    // The main program (thread 0?) now waits for both threads to finish before it continue (to exit)
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
}

int main()
{
    forking();
    threading();
}
