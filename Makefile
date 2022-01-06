CC = clang
CFLAGS = -Wall -pedantic -Weverything -std=c11 -D _CRT_SECURE_NO_WARNINGS=1
BIN = main.exe
OBJ = main.c parser.c automaton.c mealy.c moore.c filewriter.c dkame.c dkamo.c

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJ)
	$(CC) $^ -o $@
