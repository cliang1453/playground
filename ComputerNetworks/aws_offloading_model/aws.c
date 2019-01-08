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

    //bind to the port number specified in getaddrinfo()
    status = bind(sockfd_tcp, (*res_tcp)->ai_addr, (*res_tcp)->ai_addrlen);
    error_check(status, "bind");

    status = listen(sockfd_tcp, backlog); 
    error_check(status, "listen");
 
    return sockfd_tcp;
}

int bootup_UDP(addrinfo* hints_udp, addrinfo** res_udp, addrinfo** p_udp, 
    const char* dest_port, sockaddr_in* my_addr)
{
    //Reference from Beej's 
    //http://beej.us/guide/bgnet/output/html/multipage/syscalls.html
    memset(hints_udp, 0, sizeof *hints_udp); // make sure the struct is empty
    hints_udp->ai_family = AF_UNSPEC;  // AF_INET or AF_INET6 to force version
    hints_udp->ai_socktype = SOCK_DGRAM; // udp stream sockets

    my_addr->sin_family = AF_INET;
    my_addr->sin_port = htons(24396);    
    my_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(my_addr->sin_zero, '\0', sizeof my_addr->sin_zero);

    int status;
    if ((status = getaddrinfo("127.0.0.1", dest_port, hints_udp, res_udp)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    //create udp socket
    int sockfd_udp = socket((*res_udp)->ai_family, (*res_udp)->ai_socktype, 0);
    error_check(sockfd_udp, "socket");

    //bind udp static port
    status = bind(sockfd_udp, (struct sockaddr *)my_addr, sizeof *my_addr);
    error_check(status, "bind");

    return sockfd_udp;
}

void calculate_final_result(char* input, int maximum_bufsize, float* x_collection, const char* function)
{
    float result = 0;
    if(strcmp(function, "LOG") == 0)
    {
        for(int i=0; i<6; i++)
            result = result - x_collection[i]/(i+1);
    }
    else if(strcmp(function, "DIV") == 0)
    {
        result = result + 1;
        for(int i=0; i<6; i++)
            result = result + x_collection[i];
    }
    else
    {
        fprintf(stderr, "unrecognizable function %s", function);
        exit(1);
    }

    int ret = snprintf(input, maximum_bufsize, "%f", result);
    if(ret < 0)
    {
        fprintf(stderr, "Error in calculating final result\n");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    struct addrinfo hints_tcp, *res_tcp, *p_tcp;
    struct addrinfo hints_udp, *res_udp, *p_udp;
    int new_sockfd, sockfd_tcp, sockfd_udp;
    int backlog = 5;
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    struct sockaddr_in my_addr;
    socklen_t len_udp = sizeof(my_addr);
    struct sockaddr_in their_addr;
    socklen_t addr_len;
    int bytes_recv;
    int bytes_sent;

    int maximum_bufsize = 1000;
    char buf[maximum_bufsize];
    int function_len = 3;
    char function[function_len+1];
    char input[maximum_bufsize];
    float x_collection[6];


    //Boot up TCP socket
    sockfd_tcp = bootup_TCP(&hints_tcp, &res_tcp, &p_tcp, "25396", backlog);
    error_check(getsockname(sockfd_tcp, (struct sockaddr *)&sin, &len), "getsockname");
    printf("The AWS is up and running\n");

    while(true)
    {
        //Receive TCP reqeust from client
        addr_len = sizeof their_addr;
        new_sockfd = accept(sockfd_tcp, (struct sockaddr *)&their_addr, &addr_len);
        error_check(new_sockfd, "accept");
        bytes_recv = recv(new_sockfd, buf, maximum_bufsize-1, 0);
        error_check(bytes_recv, "recv");

        buf[bytes_recv] = '\0';
        memcpy(function, buf, function_len);
        function[function_len] = '\0';
        memcpy(input, buf + function_len, strlen(buf) + 1 -function_len);
        x_collection[0] = atof(input);
        printf("The AWS received input <%s> and function = %s from the client using TCP over port <%d>\n", 
              input, function, ntohs(their_addr.sin_port));

        //Boot up UDP socket and send to UDP request to Backend server A
        sockfd_udp = bootup_UDP(&hints_udp, &res_udp, &p_udp, "21396", &my_addr);
        error_check(getsockname(sockfd_udp, (struct sockaddr *)&my_addr, &len_udp), "getsockname");
        bytes_sent = sendto(sockfd_udp, input, strlen(input), 0, res_udp->ai_addr, res_udp->ai_addrlen);
        error_check(bytes_sent, "sendto");
        bytes_recv = recvfrom(sockfd_udp, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom");
        x_collection[1] = atof(buf);
        close(sockfd_udp);
        freeaddrinfo(res_udp);

        //Boot up UDP socket and send to UDP request to Backend server B
        sockfd_udp = bootup_UDP(&hints_udp, &res_udp, &p_udp, "22396", &my_addr);
        error_check(getsockname(sockfd_udp, (struct sockaddr *)&my_addr, &len_udp), "getsockname");
        bytes_sent = sendto(sockfd_udp, input, strlen(input), 0, res_udp->ai_addr, res_udp->ai_addrlen);
        error_check(bytes_sent, "sendto");
        bytes_recv = recvfrom(sockfd_udp, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom");
        x_collection[2] = atof(buf);
        close(sockfd_udp);
        freeaddrinfo(res_udp);

        //Boot up UDP socket and send to UDP request to Backend server C
        sockfd_udp = bootup_UDP(&hints_udp, &res_udp, &p_udp, "23396", &my_addr);
        error_check(getsockname(sockfd_udp, (struct sockaddr *)&my_addr, &len_udp), "getsockname");
        bytes_sent = sendto(sockfd_udp, input, strlen(input), 0, res_udp->ai_addr, res_udp->ai_addrlen);
        error_check(bytes_sent, "sendto");
        bytes_recv = recvfrom(sockfd_udp, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom");
        x_collection[4] = atof(buf);
        close(sockfd_udp);
        freeaddrinfo(res_udp);

        //Print port information
        printf("The AWS sent <%s> to Backend-Server A\n", input);        
        printf("The AWS sent <%s> to Backend-Server B\n", input);
        printf("The AWS sent <%s> to Backend-Server C\n", input);
        printf("AWS received <%f> from Backend-Server A using UDP over port number <%d>\n",  x_collection[1], ntohs(my_addr.sin_port));        
        printf("AWS received <%f> from Backend-Server B using UDP over port number <%d>\n",  x_collection[2], ntohs(my_addr.sin_port));
        printf("AWS received <%f> from Backend-Server C using UDP over port number <%d>\n",  x_collection[4], ntohs(my_addr.sin_port));
        snprintf(input, maximum_bufsize, "%f", x_collection[1]);

        //Boot up UDP socket and send to UDP request to Backend server A for a second time        
        sockfd_udp = bootup_UDP(&hints_udp, &res_udp, &p_udp, "21396", &my_addr);
        error_check(getsockname(sockfd_udp, (struct sockaddr *)&my_addr, &len_udp), "getsockname");
        bytes_sent = sendto(sockfd_udp, input, strlen(input), 0, res_udp->ai_addr, res_udp->ai_addrlen);
        error_check(bytes_sent, "sendto");
        bytes_recv = recvfrom(sockfd_udp, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom");
        x_collection[3] = atof(buf);
        close(sockfd_udp);
        freeaddrinfo(res_udp);

        //Boot up UDP socket and send to UDP request to Backend server B for a second time        
        sockfd_udp = bootup_UDP(&hints_udp, &res_udp, &p_udp, "22396", &my_addr);
        error_check(getsockname(sockfd_udp, (struct sockaddr *)&my_addr, &len_udp), "getsockname");
        bytes_sent = sendto(sockfd_udp, input, strlen(input), 0, res_udp->ai_addr, res_udp->ai_addrlen);
        error_check(bytes_sent, "sendto");
        bytes_recv = recvfrom(sockfd_udp, buf, maximum_bufsize-1, 0, (struct sockaddr *)&their_addr, &addr_len);
        error_check(bytes_recv, "recvfrom");
        x_collection[5] = atof(buf);
        close(sockfd_udp);
        freeaddrinfo(res_udp);

        //Print port information
        printf("The AWS sent <%s> to Backend-Server A\n", input); 
        printf("The AWS sent <%s> to Backend-Server B\n", input);
        printf("AWS received <%f> from Backend-Server A using UDP over port number <%d>\n",  x_collection[3], ntohs(my_addr.sin_port));
        printf("AWS received <%f> from Backend-Server B using UDP over port number <%d>\n",  x_collection[5], ntohs(my_addr.sin_port));
        printf("Values of power received by AWS: <%f, %f, %f, %f, %f, %f>\n", 
            x_collection[0], x_collection[1], x_collection[2], x_collection[3], x_collection[4], x_collection[5]);

        //Calculate the final result after collecting all the powers
        calculate_final_result(input, maximum_bufsize, x_collection, function);
        printf("AWS calculate <%s> on <%f> : <%s>\n", function, x_collection[0], input);

        //Send TCP response to client
        bytes_sent = send(new_sockfd, input, strlen(input), 0);
        error_check(bytes_sent, "send");
        printf("The AWS sent <%s> to client\n", input);
    }

    close(new_sockfd);
    close(sockfd_tcp);
    freeaddrinfo(res_tcp);
}

