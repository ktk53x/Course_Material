#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.h"
#include <pthread.h>
#include "myqueue.h"
#include <time.h>
#define ERROR -1
#define MAX_CLIENTS 1000
#define THREAD_POOL_SIZE 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_cond_t condition_variable = PTHREAD_COND_INITIALIZER;
socklen_t addr_size = sizeof(struct sockaddr_in);


void handle_error(int fd, char *msg)
{
        if (fd == ERROR)
        {
                perror(msg);
                exit(ERROR);
        }
}

void fill_message(int type, int length, char msg[], struct Message *M)
{
        M->type = type;
        M->length = length;
        strcpy(M->message, msg);
}

void *handle_tcp(int *p_csockfd, int num)
{
        int csockfd = *((int *)p_csockfd);
	printf("Handling %d request\n", num);
	
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	setsockopt(csockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

        struct Message M1;
        int rcvstatus = read(csockfd, (void *)&M1, sizeof(M1));
	if(rcvstatus <= 0)
	{
		printf("TCP Packet Not Recieved\n");
        	close(csockfd);
		return NULL;
	}
        printf("type: %d, length: %d, message: %s\n", M1.type, M1.length, M1.message);
        // Message_1 Received

	// Generate a random UDP Port	
	int udp_port = rand() % (9999 - 3000 + 1) + 3000;
	
	char udps[10];
        sprintf(udps, "%d", udp_port);
        struct Message M2;
        char m2[MSG_LEN];
        strcpy(m2, udps);
        fill_message(2, strlen(m2), m2, &M2);
        send(csockfd, (void *)&M2, sizeof(M2), 0);
        // Message_2 Sent

	int ussockfd;
        ussockfd = socket(AF_INET, SOCK_DGRAM, 0);
        handle_error(ussockfd, "UDP Socket: ");
        // UDP Socket Created

        struct sockaddr_in userver, client;
        userver.sin_family = AF_INET;
        userver.sin_port = htons(udp_port);
        userver.sin_addr.s_addr = INADDR_ANY;
        bzero(&userver.sin_zero, 8);
        // UDP Server Info Filled
	
        int bflag = bind(ussockfd, (struct sockaddr *)&userver, addr_size);
        handle_error(bflag, "UDP Bind: ");
        // UDP Socket Bind


        close(csockfd);
        // Client Connection Closed
        	
	
	setsockopt(ussockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
        struct Message M3;
        int rstatus = recvfrom(ussockfd, (void *)&M3, sizeof(M3), 0, (struct sockaddr *)&client, &addr_size);
	if(rstatus <= 0)
	{
		printf("UDP Packet Not Recieved\n");
        	close(ussockfd);
		return NULL;
	}
	printf("type: %d, length: %d, message: %s\n", M3.type, M3.length, M3.message);
	// Message_3 Received

	struct Message M4;
	char m4[MSG_LEN] = "Hi I am UDP. It was nice meeting you. Now, bye-bye.";
	fill_message(4, strlen(m4), m4, &M4);
	sendto(ussockfd, (void *)&M4, sizeof(M4), 0, (struct sockaddr *)&client, addr_size);
	// Message_4 Sent

        close(ussockfd);
	return NULL;
}

void *thread_function(void *args)
{
	int *p_csockfd;
	// Get the thread number
	int t_num = *((int *)args);

	while (1)
	{
		// Acquire Mutex Lock for accessing thread pool
		pthread_mutex_lock(&mutex);
		if ((p_csockfd = dequeue()) == NULL)
		{
			// Go to Sleep in the critical section if no thread available
			pthread_cond_wait(&condition_variable, &mutex);
		}
		// Release Mutex Lock
		pthread_mutex_unlock(&mutex);
		if (p_csockfd != NULL)
                {
                         handle_tcp(p_csockfd, t_num);
                         printf("DONE\n");
                }

		  
	}
}

int main(int argc, char *argv[])
{
	// Prepare seed
	srand(time(0));

	int ssockfd, csockfd;
	struct sockaddr_in server, client;

	// Create Thread Pool
	for (int i = 0; i < THREAD_POOL_SIZE; i++)
	{
		int *num = malloc(sizeof(int));
		*num = i;
		pthread_create(&thread_pool[i], NULL, thread_function, num);
	}

	if (argc != 2)
	{
		printf("Usage: ./server <PORT_NO>... \nPlease Try Again.....\n");
		exit(ERROR);
	}

	ssockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error(ssockfd, "Socket: ");
	// Socket Created

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);
	// Server Info Filled

	int bflag = bind(ssockfd, (struct sockaddr *)&server, addr_size);
	handle_error(bflag, "Bind: ");
	// Server Bind done

	int lflag = listen(ssockfd, MAX_CLIENTS);
	handle_error(lflag, "Listen: ");
	// Listening

	while (1)
	{
		csockfd = accept(ssockfd, (struct sockaddr *)&client, &addr_size);
		handle_error(csockfd, "Accept: ");
		// Connection to Client Accepted
		
		// Create a pointer to csockfd
		int *p_csockfd = malloc(sizeof(int));
		*p_csockfd = csockfd;

		// Acquire Mutex Lock for assigning a thread from thread pool
		pthread_mutex_lock(&mutex);

		
		// Enqueue the request and signal the sleeping thread to get to work
		enqueue(p_csockfd);
		pthread_cond_signal(&condition_variable);

		// Release Mutex Lock
		pthread_mutex_unlock(&mutex);

		
	}
	return 0;
}
