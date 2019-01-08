# AWS offloading Model
This is an implementation of a simulation of computational offloading model. 

## Personal Information
Full Name: Chen Liang

Student ID: 7314634396

## Project Details
This model consists of a client, and 3 backend servers and an intermediate server (AWS server). The client sends out an computational request to the AWS server, which distributes the task separately to 3 backend servers. After finishing different tasks, the 3 backend servers send back each of their computational results to the AWS server. The AWS server processes the collected results and sends the final result to the client. In this project, the computational request is to compute taylor series of <code>1/(1-x)</code> or <code>log(1-x)</code> up to 6 orders, given <code>0<x<1</code>. The client and the AWS server connect through TCP socket. The AWS server and 3 backend servers connect through UDP socket. 

<code>client.c</code>: The implementation of client. After establishing a TCP connection with AWS, the client sends computational request to AWS consisting of function name and input value.

<code>aws.c</code>: The implementation of AWS server. After establishing a TCP connection with the client, the AWS server receives the request and extracts the <code>function name</code>/<code>input value</code> from the request. Each time the AWS server needs computational resources, it creates UDP socket (at a static port 24396) with one of the backend servers, sends the <code>input value</code> to the backend server, and closes the socket when computational result is returned. The AWS server computes the final result according to the <code>function name</code> on the collected results from the backend servers. It returns the final result to the client through the established TCP connection.

<code>serverA.c</code>: The implementation of backend serverA. After establishing a UDP connection with AWS, it receives(at a static port 21396) the <code>input value</code> from AWS server. It sends back <code>input value ^ 2</code> to the AWS server. 

<code>serverB.c</code>: The implementation of backend serverB. After establishing a UDP connection with AWS, it receives(at a static port 22396) the <code>input value</code> from AWS server. It sends back <code>input value ^ 3</code> to the AWS server. 

<code>serverC.c</code>: The implementation of backend serverC. After establishing a UDP connection with AWS, it receives(at a static port 23396) the <code>input value</code> from AWS server. It sends back <code>input value ^ 5</code> to the AWS server. 

The format of all message exchanged is char array.

## Usage
<code>make clean</code> <code>make all</code>: compile and create executables

<code>make serverA</code>: run serverA

<code>make serverB</code>: run serverB

<code>make serverC</code>: run serverC

<code>make serverC</code>: run serverC

## Reference
[Beej's Guide to Network Programming: System Calls or Bust](http://beej.us/guide/bgnet/output/html/multipage/syscalls.html)