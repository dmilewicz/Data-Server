CC = clang
ARGS = -Wall

all: clean server 

server:
	$(CC) -o server -lpthread $(ARGS) httpserver.c requestparse.c readHTML.c parse_v2.c sort.c

clean:
	rm -rf server *.o
