CFLAGS=-Wall -g
LDFLAGS=-lsystemd

logwatch: logwatch.o
	$(CC) -o $@ $^ $(LDFLAGS)

all: logwatch

clean:
	rm -f *.o logwatch

install:
	install -m 0755 logwatch /usr/bin/logwatch
	install -m 0644 logwatch.service /etc/systemd/system/logwatch.service
