CC = g++
CFLAGS = -g -Wall

.PHONY: default all clean

all: serverA_exe serverB_exe serverC_exe aws_exe client

serverA: serverA_exe
	./serverA_exe

serverB: serverB_exe
	./serverB_exe

serverC: serverC_exe
	./serverC_exe

aws: aws_exe
	./aws_exe

serverA_exe: serverA.c
	$(CC) $(CFLAGS) serverA.c -o serverA_exe

serverB_exe: serverB.c
	$(CC) $(CFLAGS) serverB.c -o serverB_exe

serverC_exe: serverC.c
	$(CC) $(CFLAGS) serverC.c -o serverC_exe

aws_exe: aws.c
	$(CC) $(CFLAGS) aws.c -o aws_exe

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f serverA_exe serverB_exe serverC_exe aws_exe client