#pragma once

#include "automaton.h"
#include "error.h"

ErrorCode parse_dkame(char* str, Automaton** automaton);
ErrorCode parse_dkamo(char* str, Automaton** automaton);
