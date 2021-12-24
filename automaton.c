#include "automaton.h"
#include <stdlib.h>
#include <string.h>

const char* automaton_type_string[] = {
    "",
    "mealy",
    "moore"
};

AutomatonType automaton_get_type(const char* str) {
    for(size_t i = 1; i < sizeof(automaton_type_string) / sizeof(automaton_type_string[0]); ++i) {
        if(strncmp(str, automaton_type_string[i], strlen(automaton_type_string[i])) == 0) {
            return i;
        }
    }

    return TYPE_NONE;
}