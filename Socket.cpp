#include "Socket.h"

Socket::Socket() {
	/*
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    */
}

void Socket::respond(char* message) {
	send(returnHost, message);
}

void Socket::send(string host, char* message) {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

	if ((he=gethostbyname(host.c_str())) == NULL) {  // get the host info
        perror("gethostbyname");
        exit(1);
    }

    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(PORT); // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if ((numbytes=sendto(sockfd, message, strlen(message), 0,
         (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
        perror("sendto");
        exit(1);
    }

    close(sockfd);
    usleep(2500);
}

int Socket::receive(bool timeout) {
    int ret = 1;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(PORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr,
                                          sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    // TIMEOUT
    if (timeout) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 20000;
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof tv)) {
            perror("setsockopt");
            ret = -2; 
        }       
    }
 
    // SOCKET REUSE
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    addr_len = sizeof(struct sockaddr);
    if ((numbytes=recvfrom(sockfd,buf, BUFFER, 0,
                       (struct sockaddr *)&their_addr, (socklen_t*)&addr_len)) == -1) {
        ret = -1;
        //exit (1); // timeout
    }

    buf[numbytes] = '\0';
    returnHost = inet_ntoa(their_addr.sin_addr);
    close(sockfd);
    usleep(5000);
    return ret;
}