#pragma once

#include "automaton.h"

// precte soubor a vytvori automat, ktery lze prepsat do jineho datoveho formatu
ErrorCode parse(const char* filename, Automaton** automaton);

// zjisti pocet stavu automatu
size_t get_state_count(char* str);


// vyplni tabulku vstupnich znaku
// vrati jejich pocet
// tabulka urcuje na ktery znak se zobrazi znak v <read> tagu
size_t get_in_characters(char* str, char ascii[]);


// spocita pocet prechodu v automatu
size_t get_transition_count(char* str);


// naplni prechodovou tabulku (z ktereho stavu, do ktereho stavu, pres ktery symbol, ktery symbol se vypise)
ErrorCode get_transitions(char* str, size_t* count, Transition** _transitions, char* in, char* out);

// najde a vrati vychozi stav automatu, pokud zadny nenajde, vychozi stav je implicitne 'A'
char get_initial_state(char* str);
