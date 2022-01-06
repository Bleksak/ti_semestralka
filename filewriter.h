#pragma once

enum OutputFileType {
    OUT_XML,
    OUT_DKAME,
    OUT_DKAMO,
};

typedef enum OutputFileType OutputFileType;

#include "automaton.h"

void write(Automaton* automaton, const char* file);
