CC=gcc
LIBS=`pkg-config --libs gio-2.0` -lm
CFLAGS=`pkg-config --cflags gio-2.0`

SRC=main.c
OBJ=main.o dop.o init.o

build: $(OBJ)
	$(CC) $(LIBS) $(CFLAGS) $(OBJ) -o vls

%.o: %.c
	$(CC) -c $(LIBS) $(CFLAGS) -o $@ $<

clear: 
	rm $(OBJ)
