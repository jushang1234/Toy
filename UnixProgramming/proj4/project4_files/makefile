CC = gcc
CFLAGS = -g -D_REENTRANT
LDFLAGS = -lpthread

all: web_server_http

web_server_http: server.o util.o
	${CC} ${CFLAGS} ${LDFLAGS} -o web_server_http server.o util.o

server.o: server.c
	${CC} ${CFLAGS} ${LDFLAGS} -c server.c

clean:
	rm server.o web_server_http

