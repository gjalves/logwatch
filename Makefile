CFLAGS=-Wall -g
LDFLAGS=-lsystemd

logwatch: logwatch.o
	$(CC) -o $@ $^ $(LDFLAGS)

all: logwatch

clean:
	rm -f *.o logwatch
