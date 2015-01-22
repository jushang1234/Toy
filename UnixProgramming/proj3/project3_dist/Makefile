CFLAGS = -g -Wall 

all: packet_sender.c packet_receiver.c packet.h main_mm.c main_malloc.c mm.o 
	gcc $(CFLAGS) -o main_mm main_mm.c mm.o
	gcc $(CFLAGS) -o main_malloc main_malloc.c mm.o
	gcc $(CFLAGS) -o packet_sender packet_sender.c mm.o
	gcc $(CFLAGS) -o packet_receiver packet_receiver.c mm.o

mm.o: mm.c mm.h
	gcc $(CFLAGS) -o mm.o -c mm.c

clean:
	rm -rf *.o packet_sender packet_receiver main_mm main_malloc
