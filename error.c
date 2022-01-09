#include <stdio.h>
#include "error.h"

static const char* messages[] = {
    "",
    "Out of memory !",
    "Bad file!",
    "Invalid argument count!",
};

void print_error_message(ErrorCode code) {
    fprintf(stderr, "%s\n", messages[code]);
}
