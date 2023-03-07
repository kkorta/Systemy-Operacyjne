all:
	gcc -Wall -Wno-unused-variable main.c -o main
	gcc -Wall -Wno-unused-variable testy.c -o testy

tests: all
	for lib in ignore handler mask pending ; do \
		for o in fork exec ; do \
			printf "$$lib $$o\n" >> raport2.txt ; \
			./main $$o $$lib >> raport2.txt ; \
		done ; \
	done

clean:
	rm -rf main testy