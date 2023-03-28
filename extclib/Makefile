CC=gcc
LN=ld

# CC=i686-w64-mingw32-gcc
# LN=ld -m i386pe

CFLAGS=-Wall -std=c99 -g

HEADERS=net.h hashtab.h tree.h type.h http.h db.h
SOURCES=net.c hashtab.c tree.c type.c http.c db.c
OBJECTS=net.o hashtab.o tree.o type.o http.o db.o

.PHONY: default
default: build link
	
build: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES) 

link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
