#pragma once

#include "automaton.h"

// nacte z XML stringu mealyho automat
ErrorCode parse_mealy(char* str, Automaton** automaton);
