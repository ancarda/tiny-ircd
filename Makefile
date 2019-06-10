CFLAGAS=-Wall -Werror
LDFLAGS=-pthread

all: tinyircd

tinyircd: chan.o chanpool.o ircconnpool.o ircconn.o irc.o tcp.o main.o
	gcc $(CFLAGS) chan.o chanpool.o ircconnpool.o ircconn.o irc.o tcp.o main.o -o tinyircd $(LDFLAGS)

chan.o: chan.c
	gcc $(CFLAGS) -c chan.c -o chan.o

chanpool.o: chan.c
	gcc $(CFLAGS) -c chanpool.c -o chanpool.o

ircconnpool.o: ircconnpool.c
	gcc $(CFLAGS) -c ircconnpool.c -o ircconnpool.o

ircconn.o: ircconn.c
	gcc $(CFLAGS) -c ircconn.c -o ircconn.o

irc.o: irc.c
	gcc $(CFLAGS) -c irc.c -o irc.o

tcp.o: tcp.c
	gcc $(CFLAGS) -c tcp.c -o tcp.o

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

clean:
	$(RM) *.o tinyircd
