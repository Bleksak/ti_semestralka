#pragma once

struct Automaton;
typedef struct Automaton Automaton;

#include "transition.h"
#include "filewriter.h"

// typ automatu + souboru
// MEALY, MOORE => JFLAP XML
// DKAME, DKAMO => DKAM MEALY, MOORE
typedef enum AutomatonType {
    TYPE_NONE = 0,
    TYPE_MEALY,
    TYPE_MOORE,
    TYPE_DKAME,
    TYPE_DKAMO,
} AutomatonType;

// automat
struct Automaton {
    char* in; // vstupni symboly (ascii tabulka)
    char* out; // vystupni symboly (ascii tabulka)
    Transition* transitions; // tabulka prechodu
    size_t transition_count; // pocet prechodu
    size_t state_count; // pocet stavu
    size_t in_count; // pocet vstupnich symbolu
    size_t out_count; // pocet vystupnich symbolu
    AutomatonType type; // typ automatu(a souboru)
    char initial_state; // pocatecni stav automatu
};

// vytvori novy automat typu DKAM
Automaton* automaton_dk_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state);

// vytvori novy automat se vstupnimi a vystupnimi symboly
Automaton* automaton_new(AutomatonType type, size_t state_count, size_t in_count, size_t out_count, size_t transition_count, Transition* transitions, char initial_state, char* in, char* out);

// uvolni pamet pridelenou automatu
void automaton_free(Automaton* automaton);
