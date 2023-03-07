CC=gcc
CFLAGS= -Wall

all:
	make compile

compile: main.c cook.c deliver.c
	$(CC) $(CFLAGS) main.c -o main -lm
	$(CC) $(CFLAGS) cook.c -o cook -lm
	$(CC) $(CFLAGS) deliver.c -o deliver -lm

test: compile
	./main 8 8
clean:
	rm -f main *.o *.a cook deliver