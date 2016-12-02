CC = /usr/bin/gcc
CFLAGS = -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread
OBJFILES = client.o connectServer.o performConnection.o responseHandler.o processRequest.o

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


play: $(OBJFILES)
	$(CC) $(CFLAGS) -o play $(OBJFILES)

clean:
	rm -f $(OBJFILES) play
