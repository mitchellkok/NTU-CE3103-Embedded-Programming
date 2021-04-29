/*
SOCKETS
- An API that enable a user program to pass data to another program
- Socket can be used over the network or on the same machine
  But LESS EFFICIENT LOCALLY than named pipes due to overheads

1. Client
    - Usage: ./client [hostname(IP)] [port]

2. Server
    - Usage: ./server [port]
    - Performs functions on request from a client process, normally over a network

Socket Types
- Data can be transferred using standard protocols such as TCP and UDP
    -- Stream Socket, which uses TCP
    -- Datagram Socket, which uses UDP in a connectionless manner
- Behaves like a low-level file descriptor
  Accessed using commands such as read() and write()

Message Queue
- Has an explicit length and defined type (vs Named pipe that sends data w/o structure)
- Implement the “producer-consumer” model of cooperating processes or threads.

Notes
- Private IP:               192.168.0.0 to 192.168.255.255
    -- Reserved for use on closed networks (not public internet)
- TCP/IP Privileged Ports:  0 to 1023 
    -- Only root users can start servers binding to these port numbers
    -- ‘safe’ to assume that root can be trusted

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for read and write functions
#include <netdb.h>

void error(char *m)
{
    perror(m);
    exit(0);
}

int main(int argc, char *argv[ ])
{ 
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buffer;

    if (argc < 3) 
        error("usage client [hostname] [port]\n");

    port = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket: (IPv4, TCP, InternetProtocol)

    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    // returns a pointer to a hostent structure that includes one or more IP addresses.

    if (server == NULL)
        error("ERROR, no such host\n");

    //memset() function fills the first n bytes of the memory area pointed to by s with the constant byte c.
    memset((char *) &serv_addr, 0, sizeof(serv_addr)); // start with a clean address structure
    serv_addr.sin_family = AF_INET; // internet socket
    bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port); // transport protocol port number. 
    // htons - host to network; makes sure that numbers are stored in memory in network byte order, MSB first.

    // try to connect an available socket on a server.
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    printf("Please enter the message: ");
    scanf("%d", &buffer);

    n = write(sockfd, &buffer,sizeof(buffer));
    
    if (n < 0)
        error("ERROR writing to socket");
    
    n = read(sockfd, &buffer,sizeof(buffer));
    if (n < 0)
        error("ERROR reading from socket");

    printf("%d\n",buffer);

    return 0;
}