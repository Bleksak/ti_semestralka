#pragma once

#include "automaton.h"
#include "parser.h"

// nacte z XML stringu mooreho automat
ErrorCode parse_moore(char* str, Automaton** automaton);
