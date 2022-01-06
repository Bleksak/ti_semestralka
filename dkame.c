#include "dkame.h"
#include <string.h>
#include <stdlib.h>

#define GETLINE() fgets(buffer, sizeof(buffer), fp)

Automaton* parse_dkame(FILE* fp) {
    char buffer[512];
    char* end;

    GETLINE();

    char* line = GETLINE();
    size_t state_count = strtoul(line, &end, 10);

    line = GETLINE();
    size_t in_count = strtoul(line, &end, 10);

    // Transition* transitions = 

    for(size_t i = 0; i < state_count; ++i) {
        line = GETLINE();
        char* split = strtok(line, " ");

        for(size_t j = 0; j < in_count; ++j) {
            split = strtok(NULL, " ");
            printf("%c ", split[0]);
        }
    }


    return 0;
}
