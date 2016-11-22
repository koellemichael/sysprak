CFLAGS = -g -Wall -Wextra -Wpedantic -Werror

CC = gcc

LDFLAGS = -lpthread

bashni: client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

client: client.o
	$(CC) $(CFLAGS) client.o

clean:
	rm -f *.o bashni
