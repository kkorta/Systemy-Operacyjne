CC=gcc
CFLAGS= -Wall
LIBS=-lm -lrt -lpthread

all:
	make compile

compile: main.c cook.c deliver.c
	$(CC) $(CFLAGS) main.c -o main $(LIBS)
	$(CC) $(CFLAGS) cook.c -o cook $(LIBS)
	$(CC) $(CFLAGS) deliver.c -o deliver $(LIBS)

test: compile
	./main 8 8
clean:
	rm -f main *.o *.a cook deliver