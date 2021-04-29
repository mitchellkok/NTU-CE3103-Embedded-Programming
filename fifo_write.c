#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// takes in an argument to pass through mypipe
int main(int argc, char *argv[])    
{ 
    FILE *fp;

    mkfifo("mypipe",0666); // create the named pipe (using <sys/stat.h>)
    if((fp = fopen("mypipe", "w")) == NULL)
    {
        fprintf(stderr,"fopen caused an error\n");
        exit(1);
    }

    fputs(argv[1], fp); // write argument to mypipe
    fclose(fp);
    return(0);
}