#include <stdlib.h>
#include <string.h>

#include "automaton.h"

const char* automaton_type_string[] = {
	"",
	"mealy",
	"moore"};

AutomatonType automaton_get_type(const char* str) {
	for (size_t i = 1; i < sizeof(automaton_type_string) / sizeof(automaton_type_string[0]); ++i) {
		if (strncmp(str, automaton_type_string[i], strlen(automaton_type_string[i])) == 0) {
			return i;
		}
	}

	return TYPE_NONE;
}

Automaton* automaton(AutomatonType type, OutputFileType fstype, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state, char* in, char* out) {
	Automaton* a = calloc(1, sizeof(Automaton));
	if (!a) {
		return NULL;
	}

	a->in = calloc(256, sizeof(char));

	if (!a->in) {
		free(a);
		return NULL;
	}

	a->out = calloc(256, sizeof(char));

	if (!a->out) {
		free(a->in);
		free(a);
		return NULL;
	}

    a->type = type;
	a->state_count = state_count;
	a->in_count = in_count;
	a->out_count = out_count;
	a->transition_count = transition_count;
	a->transitions = transitions;
	a->initial_state = initial_state;
	a->fstype = fstype;

	memcpy(a->in, in, 256 * sizeof(char));
	memcpy(a->out, out, 256 * sizeof(char));

	return a;
}