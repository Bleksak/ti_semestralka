CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11 -D _CRT_SECURE_NO_WARNINGS=1 -g
BIN = main.exe
OBJ = main.c parser.c automaton.c mealy.c moore.c filewriter.c dk.c string_table.c error.c

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
