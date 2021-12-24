#pragma once

typedef enum AutomatonType {
    TYPE_NONE = 0,
    TYPE_MEALY,
    TYPE_MOORE
} AutomatonType;

AutomatonType automaton_get_type(const char* str);
