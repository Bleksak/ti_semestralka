#include <stdio.h>

#include "automaton.h"
#include "parser.h"
#include "filewriter.h"

int main(int argc, const char** argv) {
    const char* input = "data/empty.txt";
    Automaton* automaton;

    ErrorCode code = parse(input, &automaton);

    if(code) {
        return code;
    }

    write(automaton, "test.jff");

    automaton_free(automaton);

    return 0;
}
