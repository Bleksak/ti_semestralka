#pragma once

#include "automaton.h"

ErrorCode parse(const char* filename, Automaton** automaton);

size_t get_state_count(char* str);
size_t get_in_characters(char* str, char ascii[]);
size_t get_transition_count(char* str);
ErrorCode get_transitions(char* str, size_t* count, Transition** _transitions);
char get_initial_state(char* str);
