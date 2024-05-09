CC = clang
CFLAGS = -Wall -pedantic -std=c99
PYTHON = python3.11
SWIG = swig

all: a4

phylib.o: phylib.c phylib.h
	$(CC) $(CFLAGS) -fPIC -c phylib.c -o phylib.o

libphylib.so: phylib.o
	$(CC) -shared -o libphylib.so phylib.o -lm

phylib_wrap.c phylib.py: phylib.i
	$(SWIG) -python phylib.i

phylib_wrap.o: phylib_wrap.c
	$(CC) $(CFLAGS) -c phylib_wrap.c -I/usr/include/$(PYTHON)/ -fPIC -o phylib_wrap.o

_phylib.so: phylib_wrap.o 
	$(CC) phylib_wrap.o -shared -L. -L/usr/lib/$(PYTHON) -l$(PYTHON) -lphylib -o _phylib.so

a4: libphylib.so _phylib.so 

clean:
	rm -f *.o *.so a4 phylib_wrap.c phylib.py *.svg phylib.db
	rm -f GeneratedSVGs/*.svg
