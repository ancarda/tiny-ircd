CFLAGAS=-Wall -Werror
LDFLAGS=-pthread

all: tinyircd

tinyircd: irc.o tcp.o main.o
	gcc $(CFLAGS) irc.o tcp.o main.o -o tinyircd -pthread

irc.o: irc.c
	gcc $(CFLAGS) -c irc.c -o irc.o

tcp.o: tcp.c
	gcc $(CFLAGS) -c tcp.c -o tcp.o

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

clean:
	$(RM) *.o tinyircd
