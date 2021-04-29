/*
FIFO NAMED PIPE
1. fifo_write
    - Creates the pipe file 'mypipe' using mkfifo from <sys/stat.h>
    - Writes argument to 'mypipe' using fputs() from <stdio.h>
    - 'mypipe' DOES NOT STORE ANY INFO!! It only pipes it to another process
    - fifo_write must be running until fifor_read runs, or message will be lost

2. fifo_read
    - Creates the pipe file 'mypipe' if it does not exist
    - Reads data from 'mypipe' using fgets() from <stdio.h>;
     -fifo_read can remain running to receive multiple messages

LIMITATIONS
- Programs will wait indefinitely until data is available to be read or written
  This is in a blocking manner by default
- The two programs must be able to access the same filesystem (must be local IPC)

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void main(void)
{ 
    FILE *fp;
    char buf[20];

    /* Create the FIFO if it does not exist */
    umask(0); //set user mask permission for file creation
    mknod("mypipe", S_IFIFO|0666, 0); // can use either function!
    //mkfifo("mypipe",0666); // create the named pipe (using <sys/stat.h>)

    while(1)
    { 
        fp = fopen("mypipe", "r");
        fgets(buf, 20, fp);
        printf("We received: %s\n", buf);
        fclose(fp);
    }
}