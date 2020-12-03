CC=gcc
CFLAGS=-g
BINS=server client multi_client
OBJS=server.o myqueue.o

all: $(BINS)

server: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

client:  client.c
	$(CC) $(CFLAGS) -o $@ $^

multi_client: multi_client.c
	$(CC) $(CFLAG) -o $@ $^

clean:
	rm -rf server server.o myqueue.o client multi_client
