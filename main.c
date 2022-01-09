#include <stdio.h>

#include "automaton.h"
#include "parser.h"
#include "filewriter.h"

int main(int argc, const char** argv) {
    const char* input = "data/moore.jff";
    
    const char* in = argv[1];
    const char* out = argv[2];

    Automaton* automaton;

    ErrorCode code = parse(input, &automaton);

    if(code) {
        return code;
    }

    code = write(automaton, "test.jff");

    if(code) {
        return code;
    }

    automaton_free(automaton);

    return 0;
}
