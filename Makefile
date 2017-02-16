CC = clang
ARGS = -Wall

all: clean server parse_v2 

server:
	$(CC) -o server $(ARGS) httpserver.c requestparse.c readHTML.c

parse_v2:
	$(CC) -o parse_v2 $(ARGS) parse_v2.c

clean:
	rm -rf server parse_v2 *.o
