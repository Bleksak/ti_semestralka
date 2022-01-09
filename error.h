#pragma once

typedef enum ErrorCode {
    OK = 0,
    ERR_OUT_OF_MEMORY,
    BAD_FILE,
} ErrorCode;

void print_error_message(ErrorCode code);
