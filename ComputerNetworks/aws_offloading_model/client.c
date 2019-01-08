#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>


void error_check(int status, const char* comment)
{
    if(status == -1)
    {
        perror(comment);
        exit(1);
    }
}

int bootup_TCP(addrinfo* hints_tcp, addrinfo** res_tcp, addrinfo** p_tcp, 
    const char* dest_port, int backlog)
{
    //Reference from Beej's 
    //http://beej.us/guide/bgnet/output/html/multipage/syscalls.html
    memset(hints_tcp, 0, sizeof *hints_tcp); // make sure the struct is empty
    hints_tcp->ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 to force version
    hints_tcp->ai_socktype = SOCK_STREAM; // TCP stream sockets
    int status;

    if ((status = getaddrinfo("127.0.0.1", dest_port, hints_tcp, res_tcp)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    //create TCP socket
    int sockfd_tcp = socket((*res_tcp)->ai_family, (*res_tcp)->ai_socktype, 0);
    error_check(sockfd_tcp, "socket");

    //intiate TCP connection with AWS server
    status = connect(sockfd_tcp, (*res_tcp)->ai_addr, (*res_tcp)->ai_addrlen); 
    error_check(status, "connect");

    return sockfd_tcp;
}



int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p;
    int sockfd;
    int backlog = 5;
    int bytes_sent;
    int bytes_recv;

    int maximum_bufsize = 1000;
    char buf[maximum_bufsize];
    char *function;
    char *input;

    //Read function & input from command line arguments
    if(argc<3)
    {
        fprintf(stderr, "Not enough arguments given: ./client <function> <input>\n");
        exit(1);
    }

    if ( strcmp(argv[1], "DIV") != 0 && strcmp(argv[1], "LOG") != 0)
    {
        fprintf(stderr, "The <function> type cannot be handled\n");
        exit(1);
    }

    if (atof(argv[2])>=1 || atof(argv[2])<=0)
    {
        fprintf(stderr, "The input <value> should be in range of (0, 1)\n");
        exit(1);
    }

    function = argv[1];
    input = argv[2];
    strcpy(buf, function);
    strcat(buf, input);

    //Boot up client TCP socket
    sockfd = bootup_TCP(&hints, &res, &p, "25396", backlog);
    printf("The client is up and running\n");

    //send & receive messages with AWS server through TCP
    bytes_sent = send(sockfd, buf, strlen(buf), 0);
    error_check(bytes_sent, "send");  
    printf("The client sent <%s> and %s to AWS\n", input, function);
    bytes_recv = recv(sockfd, buf, maximum_bufsize-1, 0);
    error_check(bytes_recv, "recv");
    buf[bytes_recv] = '\0';
    printf("According to AWS, %s on <%s>: <%s>\n", function, input, buf);

    close(sockfd);
    freeaddrinfo(res);
}

