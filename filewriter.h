#pragma once

#include "automaton.h"
#include "error.h"

// zapise automat do souboru
ErrorCode write(Automaton* automaton, const char* file);
