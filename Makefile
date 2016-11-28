CFLAGS = -g -Wall -Wextra -Wpedantic -Werror

CC = gcc

LDFLAGS = -lpthread

bashni: play

play: client.c connectServer.c performConnection.c responseHandler.c
	$(CC) $(CFLAGS) -o play client.c connectServer.c performConnection.c responseHandler.c

clean:
	rm -f *.o bashni
