CC = gcc
CFLAGS = -Wall -g -std=gnu99

all: mycat countlines

mycat: mycat.c
	$(CC) $(CFLAGS) -o $@ $<

countlines: countlines.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o mycat countlines

.PHONY: all clean
