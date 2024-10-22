CC = clang
CFLAGS = -Wall -std=c99 -pedantic -fPIC
PYTHON_INCLUDE = /usr/include/python3.11
PYTHON_LIB = -L/usr/lib/python3.11/config-3.11-x86_64-linux-gnu -lpython3.11 -lm
SWIG = swig

all: test1 _hclib.so

clean:
	rm -f *.o *.so test1 hclib_wrap.c hclib.py

hclib_wrap.c hclib.py: hclib.i
	$(SWIG) -python hclib.i

hclib_wrap.o: hclib_wrap.c hclib.h
	$(CC) $(CFLAGS) -c hclib_wrap.c -I$(PYTHON_INCLUDE) -o hclib_wrap.o

hclib.o: hclib.c hclib.h
	$(CC) $(CFLAGS) -c hclib.c -o hclib.o

_hclib.so: hclib_wrap.o hclib.o
	$(CC) -shared hclib_wrap.o hclib.o -o _hclib.so $(PYTHON_LIB)

test1.o: test1.c hclib.h
	$(CC) $(CFLAGS) -c test1.c -o test1.o

test1: test1.o libhclib.so
	$(CC) test1.o -L. -lhclib -o test1

libhclib.so: hclib.o
	$(CC) hclib.o -shared -o libhclib.so

