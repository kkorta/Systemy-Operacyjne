C=gcc
CFLAGS=-Wall
CHARACTER=o
REPORT_FILE=pomiar_zad_2.txt

lib: clean
      $(CC) $(CFLAGS) zad2lib.c -o main

sys: clean
      $(CC) $(CFLAGS) zad2sys.c -o main


time_test:
      make lib
      echo "Library time:\n" > $(REPORT_FILE)
      (time ./main $(CHARACTER) bigtest.txt) 2>> $(REPORT_FILE)
      make sys
      echo "\nSystem time:\n" >> $(REPORT_FILE)
      (time ./main $(CHARACTER) bigtest.txt) 2>> $(REPORT_FILE)

clean:
      rm -f main