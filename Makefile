CC = /usr/bin/gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread

OBJFILES = client.o parameter.o connectServer.o performConnection.o responseHandler.o processRequest.o config.o format.o sharedMemory.o processGame.o

bashni: play

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

parameter.o: parameter.c parameter.h
	$(CC) $(CFLAGS) -c parameter.c

connectServer.o: connectServer.c connectServer.h
	$(CC) $(CFLAGS) -c connectServer.c

performConnection.o: performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

responseHandler.o: responseHandler.c responseHandler.h
	$(CC) $(CFLAGS) -c responseHandler.c

processRequest.o: processRequest.c processRequest.h
	$(CC) $(CFLAGS) -c processRequest.c

config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c

format.o: format.c format.h
	$(CC) $(CFLAGS)	-c format.c

sharedMemory.o: sharedMemory.c sharedMemory.h
	$(CC) $(CFLAGS)	-c sharedMemory.c

processGame.o: processGame.c processGame.h
	$(CC) $(CFLAGS)	-c processGame.c

client: $(OBJFILES)
	$(CC) $(CFLAGS) -o client $(OBJFILES)

play: ./client
	./client $(GAME_ID) $(PLAYER) $(CONF_FL)

clean:
	rm -f $(OBJFILES) play
