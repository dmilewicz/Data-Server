CC = clang
ARGS = -Wall

all: clean server 

server:
	$(CC) -o server $(ARGS) httpserver.c

clean:
	rm -rf server *.o
