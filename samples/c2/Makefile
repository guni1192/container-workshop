CC := gcc
CFLAGS := -g -Wall -Wextra
TARGET := bin/c2

c2: c2.o
	mkdir -p bin
	$(CC) -o $(TARGET) $(CFLAGS) c2.o

c2.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o c2.o

clean:
	rm -rf bin *.o
