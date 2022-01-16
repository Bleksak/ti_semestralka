#include <stdlib.h>
#include <string.h>

#include "automaton.h"

// Vytvori novy automat typu DKAM (bez in/out pole, protoze vstupni a vystupni symboly jsou ocislovany a, b, c..., 1, 2, 3...)
Automaton* automaton_dk_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state) {
	Automaton* a = calloc(1, sizeof(Automaton));

	if (!a) {
		return NULL;
	}

	a->type = type;
	a->state_count = state_count;
	a->in_count = in_count;
	a->out_count = out_count;
	a->transition_count = transition_count;
	a->transitions = transitions;
	a->initial_state = initial_state;

	return a;
}

// Vytvori novy automat
Automaton* automaton_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state, char* in, char* out) {
	Automaton* a = automaton_dk_new(type, state_count, in_count, out_count, transition_count, transitions, initial_state);
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

	memcpy(a->in, in, 256 * sizeof(char));
	memcpy(a->out, out, 256 * sizeof(char));

	return a;
}

// uvolni pamet pridelenou automatu
void automaton_free(Automaton* automaton) {
	if(!automaton) {
		return;
	}

	if(automaton->transitions) {
		free(automaton->transitions);
	}

	if(automaton->in) {
		free(automaton->in);
	}

	if(automaton->out) {
		free(automaton->out);
	}

	free(automaton);
}
