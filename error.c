#include <stdio.h>
#include "error.h"

// pole chybovych hlasek
static const char* messages[] = {
    "",
    "Out of memory !",
    "Bad file!",
    "Invalid argument count!",
};

// vypise chybovou hlasku podle kodu
void print_error_message(ErrorCode code) {
    fprintf(stderr, "%s\n", messages[code]);
}
