CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Wconversion

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
OBJS := $(filter-out server.o, $(OBJS))
OBJS := $(filter-out client.o, $(OBJS))

.PHONY: all clean

all: server client

server: $(OBJS) server.o
	$(CC) $(CFLAGS) -o $@ $^

client: $(OBJS) client.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) server.o client.o server client 2>/dev/null
