CC = clang
CFLAGS = -Wall -std=c99 -pedantic

all: main

clean:
	rm -f *.o *.so main

libhclib.so: hclib.o
	$(CC) hclib.o -shared -o libhclib.so

hclib.o: hclib.c hclib.h
	$(CC) $(CFLAGS) -c hclib.c -fPIC -o hclib.o

main.o: test1.c hclib.h
	$(CC) $(CFLAGS) -c test1.c -o main.o

main: main.o libhclib.so
	$(CC) main.o -L. -lhclib -o main
