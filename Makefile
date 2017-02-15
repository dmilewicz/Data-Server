CC = clang
ARGS = -Wall

all: clean server 

server:
	$(CC) -o server $(ARGS) httpserver.c requestparse.c readHTML.c

clean:
	rm -rf server *.o
