CC = gcc -g

all: main.o opcje.o przetwarzanie.o
	$(CC) main.o opcje.o przetwarzanie.o -o po2

main.o: main.c naglowek.h
	$(CC) main.c -c -o main.o

opcje.o: opcje.c naglowek.h
	$(CC) opcje.c -c -o opcje.o

przetwarzanie.o: przetwarzanie.c naglowek.h
	$(CC) przetwarzanie.c -c -o przetwarzanie.o

clean:
	rm -f *.o po2