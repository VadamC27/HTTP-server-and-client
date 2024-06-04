# Makefile for simple server compilation
CC = gcc

CFLAGS = -Wall -pthread

SRCS = server.c HTTPprocessing.c

HDRS = HTTPprocessing.h

OBJS = $(SRCS:.c=.o)

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)