CC	:= gcc -std=c99 -pedantic -Wall
.PHONE: all clean
all: zad1.x zad2.x zad3.x zad4.x zad6.x

zad1.x: zad1.o
zad1.o: zad1.c

zad2.x: zad2.o
zad2.o: zad2.c

zad3.x: zad3.o
zad3.o: zad3.c

zad4.x: zad4.o
zad4.o: zad4.c

zad6.x: zad6.o
zad6.o: zad6.c

%.x: %.o
		$(CC) -o $@ $^

%.o: %.c
		$(CC) -c	-o	$@	$<
clean:
	rm -f *.o *.x *.so *.a