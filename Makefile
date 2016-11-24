CFLAGS = -g -Wall -Wextra -Wpedantic -Werror

CC = gcc

LDFLAGS = -lpthread

bashni: client

client.o: client.c performConnection.c responseHandler.c
	$(CC) $(CFLAGS) -c client.c performConnection.c responseHandler.c

client: client.o performConnection.o responseHandler.o
	$(CC) $(CFLAGS) client.o performConnection.o responseHandler.o

clean:
	rm -f *.o bashni
