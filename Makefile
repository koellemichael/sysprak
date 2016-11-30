CFLAGS = -g -Wall -Wextra -Wpedantic -Werror

CC = gcc

LDFLAGS = -lpthread

bashni: play

play: client.c connectServer.c performConnection.c responseHandler.c processRequest.c
	$(CC) $(CFLAGS) -o play client.c connectServer.c performConnection.c responseHandler.c processRequest.c

clean:
	rm -f *.o bashni
