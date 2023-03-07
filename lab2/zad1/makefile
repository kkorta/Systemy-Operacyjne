CC=gcc
CFLAGS=-Wall
REPORT_FILE=pomiar_zad_1.txt

lib: clean
      $(CC) $(CFLAGS) zad1lib.c -o main

sys: clean
      $(CC) $(CFLAGS) zad1sys.c -o main

time_test:
      make lib
      echo "Library time:\n" > $(REPORT_FILE)
      (time ./main bigtest.txt bigtest1.txt) 2>> $(REPORT_FILE)
      make sys
      echo "\nSystem time:\n" >> $(REPORT_FILE)
      (time ./main bigtest.txt bigtest1.txt) 2>> $(REPORT_FILE)

clean:
      rm -f main./
