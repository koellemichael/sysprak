CC = /usr/bin/gcc
CFLAGS = -c -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lpthread
PLAYER ?= ""
CONF_FL ?= ""
SOURCES = client.c parameter.c connectServer.c performConnection.c responseHandler.c processRequest.c config.c sharedMemory.c think.c field.c
OBJFILES = $(SOURCES:.c=.o)
EXECUTABLE = client

all: $(EXECUTABLE) clean
play: $(EXECUTABLE) clean bashni
vplay: $(EXECUTABLE) clean bashnivalgrind

.c.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) -o $@

bashni: ./$(EXECUTABLE)
	-./$(EXECUTABLE) -p $(PLAYER) -f $(CONF_FL) $(GAME_ID)

bashnivalgrind: ./$(EXECUTABLE)
	-valgrind --track-origins=yes --disable-gtk --leak-check=full --trace-children=yes --show-leak-kinds=all ./$(EXECUTABLE) -p $(PLAYER) -f $(CONF_FL) $(GAME_ID)

.PHONY: clean
clean:
	-rm -f *.o
