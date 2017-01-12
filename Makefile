CC = /usr/bin/gcc
CFLAGS = -c -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread
SOURCES = client.c parameter.c connectServer.c performConnection.c responseHandler.c processRequest.c config.c sharedMemory.c think.c field.c
OBJFILES = $(SOURCES:.c=.o)
EXECUTABLE = client

play: $(EXECUTABLE) clean bashni

.c.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) -o $@

bashni: ./$(EXECUTABLE)
	./$(EXECUTABLE) $(GAME_ID) $(PLAYER) $(CONF_FL)

.PHONY: clean
clean:
	rm -f *.o
