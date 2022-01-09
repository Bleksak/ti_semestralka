#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "mealy.h"
#include "transition.h"
#include "string_table.h"

static size_t get_out_characters(char* str, char ascii[]) {
    // TODO: mozna potrebujem hashset namisto ascii tabulky
    size_t count = 0;
    size_t len = strlen(STRING_TABLE[XML_TRANSOUT]);

    while((str = strstr(str, STRING_TABLE[XML_TRANSOUT]))) {
        str += len;
        if(!ascii[(size_t) *str]) {
            count += 1;
            ascii[(size_t) *str] = (char) count;
        }
    }

    return count;
}

ErrorCode parse_mealy(char* str, Automaton** automaton) {
    char in[256] = {0};
    char out[256] = {0};

    size_t state_count = get_state_count(str);
    size_t in_count = get_in_characters(str, in);
    size_t out_count = get_out_characters(str, out);
    char initial_state = get_initial_state(str);

    Transition* transitions;
    size_t transition_count;

    ErrorCode code = get_transitions(str, &transition_count, &transitions);

    if(code) {
        return code;
    }

    *automaton = automaton_new(TYPE_MEALY, state_count, in_count, out_count, transition_count, transitions, initial_state, in, out);

    if(!*automaton) {
        return ERR_OUT_OF_MEMORY;
    }

    return OK;
}
