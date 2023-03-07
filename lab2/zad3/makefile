cc = gcc -Wall

all:
	@printf "" > ../zad3/pomiar_zad_3.txt
	make clean
	make main_dir

main_dir:
	$(cc) 3a.c -o 3a.o
	./3a.o  ../zad3 >> ../zad3/pomiar_zad_3.txt
	@printf "________________________\n" >> ../zad3/pomiar_zad_3.txt

clean:
	rm -f *.o