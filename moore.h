#pragma once

#include "automaton.h"
#include "parser.h"

ErrorCode parse_moore(char* str, Automaton** automaton);
