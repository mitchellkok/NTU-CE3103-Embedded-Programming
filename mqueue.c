/*
POSIX MESSAGE QUEUE
- Message queue is a buffer that is used in non-shared memory environments, where tasks 
  communicate by passing messages to each other  rather than by accessing shared variables.
- Included in <mqueue.h>, compile with linker option '-lrt'
- Has an explicit length and defined type (vs Named pipe that sends data w/o structure)
- Implement the “producer-consumer” model of cooperating processes or threads.

*/

#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <linux/posix_types.h>

mqd_t mqdesc;
typedef struct 
{ 
    int msg_id;
    int msg_val;
    void* msg_ptr;
} STRUCT_MSG; // struct used to encapsulate message data into one data object

STRUCT_MSG gst_Message;
STRUCT_MSG gpst_ReceiveMessage;

void* first_thread_run_function(void* arg)
{ 
    printf("I am in first thread - sending message \n");

    // Create queue, or gain access to existing queue
    mqdesc = mq_open("/First", O_RDWR|O_CREAT,
                    (S_IRUSR|S_IWUSR |S_IRGRP|S_IROTH),
                    NULL);

    // mq_send() normally will block until there is space on the queue
    mqd_t mq_RetVal = mq_send(mqdesc, (const char*) 
                        &gst_Message, sizeof(gst_Message), 0);
    printf("mq_send return val %d\n", mq_RetVal);
    printf("message sent = %d\n", gst_Message.msg_val);
    while(1);
}

void* second_thread_run_function(void* arg)
{ 
    unsigned int msg_prio;
    printf("I am in second thread - receiving message\n");
    struct mq_attr attr;

    mq_getattr(mqdesc,&attr);   // get queue info
    // mq_setattr() can change things such as blocking behaviour

    // receive a message from the queue (the message will be removed)
    // returns number of bytes received
    mqd_t mq_RetVal = mq_receive(mqdesc, (char*)
                            &gpst_ReceiveMessage, attr.mq_msgsize, &msg_prio);
    printf("mq_receive return val = %d\n", mq_RetVal);
    printf("message received = %d\n", gpst_ReceiveMessage.msg_val);
}

int main(int argc, char *argv[])
{ 
    int status;
    struct sched_param thread_param;

    pthread_attr_t thread_attr;
    pthread_t second_thread_id;
    pthread_t first_thread_id;
    thread_param.sched_priority = 5;

    int my_msg = 47;
    gst_Message.msg_val = my_msg;
    
    pthread_create(&first_thread_id, NULL, first_thread_run_function, NULL);
    pthread_create(&second_thread_id, NULL, second_thread_run_function, NULL);

    pthread_join(first_thread_id, NULL);
    pthread_join(second_thread_id, NULL);

    return 0;
}