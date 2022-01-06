#include <stdio.h>

#include "automaton.h"
#include "parser.h"
#include "filewriter.h"

int main(int argc, const char** argv) {
    const char* input = "data/mealy.dkame";

    Automaton* automaton = parse(input);

    // write(automaton, "out.f");

    return 0;
}