#pragma once

#include "automaton.h"

enum STR_TABLE {
	XML_HEADER = 0,
	XML_TYPE,
	XML_STATE_END,
	XML_READ,
	XML_INITIAL,
	XML_STATE,
	XML_TRANSITION,
	XML_FROM,
	XML_TO,
	XML_TRANSOUT,
	XML_OUTPUT,
};

extern const char* STRING_TABLE[];

Automaton* parse(const char* filename);

size_t get_state_count(char* str);
size_t get_in_characters(char* str, char ascii[]);
size_t get_transition_count(char* str);
size_t get_transitions(char* str, Transition** _transitions);
char get_initial_state(char* str);
