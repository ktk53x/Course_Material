CC=gcc
BINS=server client

all: $(BINS)

server: server.c
	$(CC) -o $@ $^ -lpthread

clinet: client.c
	$(CC) -o $@ $^

clean:
	rm -rf server client