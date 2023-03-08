CC	:= gcc -std=c99 -pedantic -Wall
.PHONE: all clean
all: zad5.x zad6.x

zad5.x: zad5.o
zad5.o: zad5.c

zad6.x: zad6.o
zad6.o: zad6.c

%.x: %.o
		$(CC) -o $@ $^

%.o: %.c
		$(CC) -c	-o	$@	$<
clean:
	rm -f *.o *.x *.so *.a