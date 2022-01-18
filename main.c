#include <stdio.h>

#include "automaton.h"
#include "filewriter.h"
#include "parser.h"

// vstupni funkce programu
// program precte soubor zadany jako prvni argument a pokusi se ho transforomvat do jineho souboru (ruzneho typu) zadaneho jako druhy argument
int main(int argc, const char** argv) {

	if(argc != 3) {
		print_error_message(BAD_ARGUMENT_COUNT);
		return BAD_ARGUMENT_COUNT;
	}

	const char* in = argv[1];
	const char* out = argv[2];

	Automaton* automaton;

	ErrorCode code = parse(in, &automaton);

	if (code) {
		print_error_message(code);
		return code;
	}

	code = write(automaton, out);
	automaton_free(automaton);

	if (code) {
		print_error_message(code);
		return code;
	}
}
