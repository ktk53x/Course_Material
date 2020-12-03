#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "message.h"

#define ERROR -1

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

int main(int argc, char *argv[])
{
	int ssockfd, ussockfd;
	struct sockaddr_in server, userver;
	socklen_t addr_size = sizeof(struct sockaddr_in);

	if (argc != 3)
	{
		printf("Usage: ./client <IP_ADDR>... <PORT_NO>... \nPlease Try Again.....\n");
		exit(ERROR);
	}

	ssockfd = socket(AF_INET, SOCK_STREAM, 0);
	handle_error(ssockfd, "Socket: ");
	// Socket Created

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&server.sin_zero, 8);
	// Server Info Filled

	int conn = connect(ssockfd, (struct sockaddr *)&server, addr_size);
	handle_error(conn, "Connection: ");
	// Connection Established

	struct Message M1;
	char m1[MSG_LEN] = "Send The UDP Port.";
	fill_message(1, strlen(m1), m1, &M1);
	send(ssockfd, (void *)&M1, sizeof(M1), 0);
	// Message_1 Sent

	struct Message M2;
	recv(ssockfd, (void *)&M2, sizeof(M2), 0);
	printf("type: %d, length: %d, message: %s\n", M2.type, M2.length, M2.message);
	// Message_2 Received

	close(ssockfd);
	// Connection Closed

	ussockfd = socket(AF_INET, SOCK_DGRAM, 0);
	handle_error(ussockfd, "UDP Socket: ");
	// UDP Socket Created

	userver.sin_family = AF_INET;
	userver.sin_port = htons(atoi(M2.message));
	userver.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&userver.sin_zero, 8);
	// UDP Server Info Filled

	printf("UDP sent\n");
	struct Message M3;
	char m3[MSG_LEN] = "Hello UDP. I want to connect to you.";
	fill_message(3, strlen(m3), m3, &M3);
	sendto(ussockfd, (void *)&M3, sizeof(M3), 0, (struct sockaddr *)&userver, addr_size);
	// Message_3 Sent

	struct Message M4;
	recvfrom(ussockfd, (void *)&M4, sizeof(M4), 0, (struct sockaddr *)&userver, &addr_size);
	printf("type: %d, length: %d, message: %s\n", M4.type, M4.length, M4.message);
	// Message_4 Received

	close(ussockfd);
	// UDP Connection Closed
	return 0;
}
