CC = clang
ARGS = -Wall

all: clean server 

server:
	$(CC) -o server $(ARGS) httpserver.c requestparse.c readHTML.c parse_v2.c sort.c

clean:
	rm -rf server parse_v2 *.o
