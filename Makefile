CC = /usr/bin/gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread


bashni: play


connectServer.o: connectServer.c connectServer.h
	$(CC) $(CFLAGS) -c connectServer.c

performConnection.o: performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

responseHandler.o: responseHandler.c responseHandler.h
	$(CC) $(CFLAGS) -c responseHandler.c

processRequest.o: processRequest.c processRequest.h
	$(CC) $(CFLAGS) -c processRequest.c


play: client.c connectServer.o performConnection.o responseHandler.o processRequest.o
	$(CC) $(CFLAGS) -o play client.c connectServer.o performConnection.o responseHandler.o processRequest.o

clean:
	rm -f *.o connectServer.o performConnection.o responseHandler.o processRequest.o play
