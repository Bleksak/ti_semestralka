#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "mealy.h"
#include "transition.h"

static size_t get_out_characters(char* str, char ascii[]) {
    // TODO: mozna potrebujem hashset namisto ascii tabulky
    size_t count = 0;
    size_t len = strlen(STRING_TABLE[XML_TRANSOUT]);

    while((str = strstr(str, STRING_TABLE[XML_TRANSOUT]))) {
        str += len;
        if(!ascii[*str]) {
            count += 1;
            ascii[*str] = count;
        }
    }

    return count;
}

Automaton* parse_mealy(char* str) {
    char in[256] = {0};
    char out[256] = {0};

    size_t state_count = get_state_count(str);
    size_t in_count = get_in_characters(str, in);
    size_t out_count = get_out_characters(str, out);
    char initial_state = get_initial_state(str);

    Transition* transitions;
    size_t transition_count = get_transitions(str, &transitions);

    return automaton(TYPE_MEALY, OUT_DKAME, state_count, in_count, out_count, transition_count, transitions, initial_state, in, out);
}
