CC = clang
ARGS = -Wall

all: clean server 

server:
	$(CC) -o server $(ARGS) httpserver.c requestparse.c parse_v2.c readHTML.c 

clean:
	rm -rf server parse_v2 *.o
