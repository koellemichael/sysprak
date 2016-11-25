CFLAGS = -g -Wall -Wextra -Wpedantic -Werror

CC = gcc

LDFLAGS = -lpthread

bashni: play

play: client.c connectServer.c
	$(CC) $(CFLAGS) -o player client.c connectServer.c

clean:
	rm -f *.o bashni
