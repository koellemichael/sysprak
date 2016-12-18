CC = /usr/bin/gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread
<<<<<<< HEAD
OBJFILES = client.o connectServer.o performConnection.o responseHandler.o processRequest.o config.o
=======
OBJFILES = client.o connectServer.o performConnection.o responseHandler.o processRequest.o format.o sharedMemory.o
>>>>>>> master

bashni: play

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

connectServer.o: connectServer.c connectServer.h
	$(CC) $(CFLAGS) -c connectServer.c

performConnection.o: performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

responseHandler.o: responseHandler.c responseHandler.h
	$(CC) $(CFLAGS) -c responseHandler.c

processRequest.o: processRequest.c processRequest.h
	$(CC) $(CFLAGS) -c processRequest.c

<<<<<<< HEAD
config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c
=======
format.o: format.c format.h
	$(CC) $(CFLAGS)	-c format.c

sharedMemory.o: sharedMemory.c sharedMemory.h
	$(CC) $(CFLAGS)	-c sharedMemory.c
>>>>>>> master

client: $(OBJFILES)
	$(CC) $(CFLAGS) -o client $(OBJFILES)

play: ./client
	./client $(GAME_ID) $(PLAYER) $(CONF_FL)

clean:
	rm -f $(OBJFILES) play
