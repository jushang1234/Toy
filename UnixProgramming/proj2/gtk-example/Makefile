CC=gcc
CFLAGS=-g -Wall `pkg-config --cflags gtk+-2.0 webkit-1.0`
LDFLAGS+=`pkg-config --libs gtk+-2.0 webkit-1.0`

LIB=/usr/lib/
SOURCES=gtk-example.c
OBJ=gtk-example

all:  $(SOURCES) $(OBJ)

$(OBJ): $(SOURCES)
	$(CC) -L ${LIB} $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(OBJ)

clean:
	rm -rf $(OBJ)

