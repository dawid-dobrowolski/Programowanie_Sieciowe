CC	:= gcc -std=c99 -pedantic -Wall
.PHONE: all clean
all: zad5.x zad7.x zad9_serwer.x zad9_klient.x zad4.x

zad4.x: zad4.o
zad4.o: zad4.c

zad5.x: zad5.o
zad5.o: zad5.c

zad7.x: zad7.o
zad7.o: zad7.c

zad9_serwer.x: zad9_serwer.o
zad9_serwer.o: zad9_serwer.c

zad9_klient.x: zad9_klient.o
zad9_klient.o: zad9_klient.c

%.x: %.o
		$(CC) -o $@ $^ -lm

%.o: %.c
		$(CC) -c	-o	$@	$<
clean:
	rm -f *.o *.x *.so *.a