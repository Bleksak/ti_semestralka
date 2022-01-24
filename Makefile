CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
BIN = main.exe
OBJ = main.c parser.c automaton.c mealy.c moore.c filewriter.c dk.c string_table.c error.c

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
