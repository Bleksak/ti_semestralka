#include <stdio.h>
#include "parser.h"

int main(int argc, const char** argv) {
    const char* input = "data/mealy.jff";

    parse(input);

    return 0;
}