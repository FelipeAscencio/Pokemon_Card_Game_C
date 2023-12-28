VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc

all: clean valgrind-ejemplo

valgrind-ejemplo: pruebas_ejemplo
	valgrind $(VALGRIND_FLAGS) ./pruebas_ejemplo

pruebas_ejemplo: src/*.c pruebas_ejemplo.c
	$(CC) $(CFLAGS) src/*.c pruebas_ejemplo.c -o pruebas_ejemplo

clean:
	rm -f pruebas_ejemplo