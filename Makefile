CC = gcc
CF =-Wall -Werror -Wextra

all: cat

cat : cat.c
	$(CC) $(CF) cat.c -o cat
	make clean

clean:
	rm -rf *.o

rebuild: all clean
