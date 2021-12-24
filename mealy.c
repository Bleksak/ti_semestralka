#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "mealy.h"

static size_t get_state_count(char* str) {
    size_t count = 0;
    size_t len = strlen(STRING_TABLE[XML_STATE_END]);

    while((str = strstr(str, STRING_TABLE[XML_STATE_END]))) {
        count += 1;
        str += len;
    }

    return count;
}

static size_t get_in_count(char* str) {
    // TODO: mozna potrebujem hashset
    bool ascii[256] = {0};
    size_t count = 0;
    size_t len = strlen(STRING_TABLE[XML_READ]);

    while((str = strstr(str, STRING_TABLE[XML_READ]))) {
        str += len;
        if(!ascii[*str]) {
            ascii[*str] = true;
            count += 1;
        }
    }

    return count;
}

static char get_initial_state(char* str) {
    size_t len = strlen(STRING_TABLE[XML_STATE]);
    char* end = strstr(str, STRING_TABLE[XML_INITIAL]);

    while(end >= str) {
        if(strncmp(end, STRING_TABLE[XML_STATE], len) == 0) {
            char* id_start = end + strlen(STRING_TABLE[XML_INITIAL]) + 1;
            char* endptr;

            return strtoul(id_start, &endptr, 10) + 'A';
        }
        end -= 1;
    }

    return 'A';
}

void parse_mealy(char* str) {
    size_t state_count = get_state_count(str);
    size_t in_count = get_in_count(str);
    char initial_state = get_initial_state(str);
    
}
