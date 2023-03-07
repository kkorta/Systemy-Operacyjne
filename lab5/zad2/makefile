cc = gcc
flags = -Wall -w

all: main clean

main:
        $(cc) $(flags) main.c -o main

test: main
        ./main kkorta@studnet.agh.edu.pl tresc esss

clean:
        rm -f *.o *.so *.a main