#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "message.h"
#include <sys/mman.h>
#define ERROR -1
#define MAX_CLIENTS 5

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

void *handle_tcp(int *p_csockd, int udp_port[], int free_port[])
{
        int csockfd = *((int *)p_csockd);
        struct Message M1;
        recv(csockfd, (void *)&M1, sizeof(M1), 0);
        printf("type: %d, length: %d, message: %s\n", M1.type, M1.length, M1.message);
        // Message_1 Received

        int udp_p = -1;
        //search for a free UDP port
        while (udp_p == -1)
        {
                for (int i = 0; i < 15; i++)
                {
                        if (free_port[i] == 0)
                        {
                                free_port[i] = 1;
                                udp_p = udp_port[i];
                                break;
                        }
                }
        }
        char udps[10];
        sprintf(udps, "%d", udp_p);
        struct Message M2;
        char m2[MSG_LEN];
        strcpy(m2, udps);
        fill_message(2, strlen(m2), m2, &M2);
        send(csockfd, (void *)&M2, sizeof(M2), 0);
        // printf("type: %d, length: %d, message: %s\n", M2.type, M2.length, M2.message);
        // Message_2 Sent

        close(csockfd);
        int ussockfd = socket(AF_INET, SOCK_DGRAM, 0);
        handle_error(ussockfd, "UDP Socket: ");
        // UDP Socket Created

        struct sockaddr_in userver;
        userver.sin_family = AF_INET;
        userver.sin_port = htons(udp_p);
        userver.sin_addr.s_addr = INADDR_ANY;
        bzero(&userver.sin_zero, 8);
        // UDP Server Info Filled

        int bflag = bind(ussockfd, (struct sockaddr *)&userver, addr_size);
        handle_error(bflag, "UDP Bind: ");
        // UDP Socket Bind
        // Client Connection Closed
        struct sockaddr_in client;
        struct Message M3;
        recvfrom(ussockfd, (void *)&M3, sizeof(M3), 0, (struct sockaddr *)&client, &addr_size);
        printf("type: %d, length: %d, message: %s\n", M3.type, M3.length, M3.message);
        // Message_3 Received

        struct Message M4;
        char m4[MSG_LEN] = "Hi I am UDP. It was nice meeting you. Now, bye-bye.";
        fill_message(4, strlen(m4), m4, &M4);
        sendto(ussockfd, (void *)&M4, sizeof(M4), 0, (struct sockaddr *)&client, addr_size);
        // printf("type: %d, length: %d, message: %s\n", M4.type, M4.length, M4.message);
        // Message_4 Sent

        //close the udp port and free it in the array free_port
        close(ussockfd);
        for (int i = 0; i < 15; i++)
        {
                if (udp_p == udp_port[i])
                {
                        free_port[i] = 0;
                        break;
                }
        }
        return NULL;
}

int main(int argc, char *argv[])
{
        int ssockfd, csockfd;
        struct sockaddr_in server, client;

        if (argc != 2)
        {
                printf("Usage: ./server <PORT_NO>... \nPlease Try Again.....\n");
                exit(ERROR);
        }

        //Creating a shared memory of udp ports and to check if they are active
        int *udp_port = mmap(NULL, 15 * sizeof(int), PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        int *free_port = mmap(NULL, 15 * sizeof(int), PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        for (int i = 0; i < 15; i++)
        {
                udp_port[i] = 4000 + i;
                free_port[i] = 0;
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
                //Accept a request
                if (fork() == 0)
                {
                        //In a forked child close the tcp socket of the child process
                        // tcp socket in the parent is still running
                        close(ssockfd);
                        handle_tcp(&csockfd, udp_port, free_port);
                        break;
                }
        }
        return 0;
}
