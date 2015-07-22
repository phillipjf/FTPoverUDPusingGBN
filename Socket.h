#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
using namespace std;

#define PORT 10031

#define BUFFER 128
#define HEADER 3
#define CONTENT (BUFFER - HEADER)

class Socket {
public:
    int sockfd;
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    int addr_len;
    char buf[BUFFER];
    struct hostent *he;
    int numbytes;

    char* returnHost;

    Socket();
    void send(string, char*);
    int receive(bool);
    void respond(char*);
};