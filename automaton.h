#pragma once

struct Automaton;
typedef struct Automaton Automaton;

#include "transition.h"
#include "filewriter.h"

typedef enum AutomatonType {
    TYPE_NONE = 0,
    TYPE_MEALY,
    TYPE_MOORE,
    TYPE_DKAME,
    TYPE_DKAMO,
} AutomatonType;

struct Automaton {
    char* in;
    char* out;
    Transition* transitions;
    size_t transition_count;
    size_t state_count;
    size_t in_count;
    size_t out_count;
    AutomatonType type;
    char initial_state;
};

Automaton* automaton_dk_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state);
Automaton* automaton_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state, char* in, char* out);
void automaton_free(Automaton* automaton);
