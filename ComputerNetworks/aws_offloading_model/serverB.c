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

void calculate_cube(char* input, int maximum_bufsize)
{
    float x = atof(input);
    int ret = snprintf(input, maximum_bufsize, "%f", x*x*x);
    if(ret < 0)
    {
        fprintf(stderr, "Error in calculating cube");
        exit(1);
    }
}

int bootup_UDP(addrinfo* hints_udp, addrinfo** res_udp, addrinfo** p_udp, 
    const char* dest_port)
{
    //Reference from Beej's 
    //http://beej.us/guide/bgnet/output/html/multipage/syscalls.html
    memset(hints_udp, 0, sizeof *hints_udp); // make sure the struct is empty
    hints_udp->ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 to force version
    hints_udp->ai_socktype = SOCK_DGRAM; // udp stream sockets

    int status;
    if ((status = getaddrinfo("127.0.0.1", dest_port, hints_udp, res_udp)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    //create udp socket
    int sockfd_udp = socket((*res_udp)->ai_family, (*res_udp)->ai_socktype, 0);
    error_check(sockfd_udp, "socket");
    status = bind(sockfd_udp, (*res_udp)->ai_addr, (*res_udp)->ai_addrlen);
    error_check(status, "bind");
    return sockfd_udp;
}


int main(int argc, char *argv[])
{

    //initial get address info setup
    struct addrinfo hints, *res, *p;
    int sockfd;
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    struct sockaddr_in their_addr;
    socklen_t addr_len;

    int maximum_bufsize = 1000;
    char buf[maximum_bufsize];
    char input[maximum_bufsize];
    int bytes_recv;
    int bytes_sent;

    //Create UDP socket
    sockfd = bootup_UDP(&hints, &res, &p, "22396");
    error_check(getsockname(sockfd, (struct sockaddr *)&sin, &len), "getsockname");
    printf("The server B is up and running using UDP on port <%d>\n", ntohs(sin.sin_port));

    while(true)
    {
        // Receive AWS UDP request
        addr_len = sizeof their_addr;
        bytes_recv = recvfrom(sockfd, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom"); 
        buf[bytes_recv] = '\0';
        memcpy(input, buf, strlen(buf) + 1);
        printf("The Server B received <%s>\n", input);

        // Processing the input value
        calculate_cube(input, maximum_bufsize);
        printf("The Server B calculated cube <%s>\n", input);

        // Send calculated result back to AWS via UDP
        bytes_sent = sendto(sockfd, input, strlen(input), 0, (struct sockaddr *)&their_addr,  sizeof their_addr);
        error_check(bytes_sent, "sendto");
        printf("The Server B finished sending the output to AWS\n");
    }

    close(sockfd);
    freeaddrinfo(res);
}

