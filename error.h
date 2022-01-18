#pragma once

// typy chyb, ktere mohou v programu nastat
typedef enum ErrorCode {
    OK = 0,
    ERR_OUT_OF_MEMORY,
    BAD_FILE,
    BAD_ARGUMENT_COUNT,
} ErrorCode;

// vypise chybovou hlasku podle kodu
void print_error_message(ErrorCode code);
