#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *message(void *buffer)
{
	int *num;
	num = (int *) buffer;
    *num *= 5; 
    buffer = num;
}


void error(char *m)
{
    perror(m); //print given str before actual error message
}

int main(int argc, char *argv[])
{ 
    int sockfd, newsockfd, port, clilen, n;
    int buffer;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread1;

    if (argc < 2) 
        error("ERROR, no port provided\n");

    port = atoi(argv[1]);  // parse argument as int
    // create socket: domain=IPv4, type=two-way stream, protocol=default-for-socket-type (InternetProtocol)
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    if (sockfd < 0) 
        error("ERROR opening socket");

    //memset() function fills the first n bytes of the memory area pointed to by s with the constant byte c.
    memset((char *) &serv_addr, 0, sizeof(serv_addr)); // start with a clean address structure

    serv_addr.sin_family = AF_INET; // The address family for the transport address.
    serv_addr.sin_addr.s_addr = INADDR_ANY; // IN_ADDR structure that contains an IPv4 transport address.
    serv_addr.sin_port = htons(port); // transport protocol port number. 
    // htons - host to network; makes sure that numbers are stored in memory in network byte order, MSB first.

    //bind assigns the address specified by addr to the socket referred to by the file descriptor sockfd.  
    //addrlen specifies the size, in bytes, of the address structure
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR binding to socket");

    listen(sockfd,2); //marks the socket referred to by sockfd as a passive socket
    // backlog = 2, number of incoming connections that can be queued for acceptance

    clilen = sizeof(cli_addr);

    while (1) {
        // extracts the first connection request on the queue of pending connections for the  listening socket (sockfd),
        // creates a new connected socket, and returns a new file descriptor referring to that new socket. 
        // The newly created socket (newsockfd) is not in the listening state. The original socket sockfd is unaffected by this call.
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);  //BLOCKING

        if (newsockfd < 0) 
            error("ERROR on accept");

        n = read(newsockfd, &buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");

        printf("Message received: %d\n",buffer);

        pthread_create(&thread1, NULL, message, (void*) &buffer);
        pthread_join(thread1,NULL);

        n = write(newsockfd, &buffer,sizeof(buffer));
        
        if (n < 0)
            error("ERROR writing back to socket");
    }
    return 0;
}



