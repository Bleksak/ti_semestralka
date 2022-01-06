#include "moore.h"
#include <stdlib.h>
#include <string.h>

static size_t get_out_characters(char* str, char ascii[]) {
	// TODO: mozna potrebujem hashset namisto ascii tabulky
	size_t count = 0;
	size_t len = strlen(STRING_TABLE[XML_OUTPUT]);

    char* start = str;

	while ((str = strstr(str, STRING_TABLE[XML_OUTPUT]))) {
        char* end = str;

		while (end >= start) {
			if (strncmp(end, STRING_TABLE[XML_STATE], len) == 0) {
				char* id_start = end + strlen(STRING_TABLE[XML_STATE]);
				char* endptr;

				size_t id = strtoul(id_start, &endptr, 10) + 'A';

				if (!ascii[id]) {
					count += 1;
					ascii[id] = count + '0';
				}

				break;
			}
			end -= 1;
		}
        str += len;
	}

	return count;
}

Automaton* parse_moore(char* str) {
	char in[256] = {0};
	char out[256] = {0};

	size_t state_count = get_state_count(str);
	size_t in_count = get_in_characters(str, in);
	size_t out_count = get_out_characters(str, out);
	char initial_state = get_initial_state(str);

	Transition* transitions;
	size_t transition_count = get_transitions(str, &transitions);

	return automaton(TYPE_MOORE, OUT_DKAMO, state_count, in_count, out_count, transition_count, transitions, initial_state, in, out);
}
