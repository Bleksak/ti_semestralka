#pragma once

#include <stddef.h>

#include "automaton.h"
#include "error.h"

// precte string a vytvori z nej mealyho automat
// predpoklad - vstupni string neobsahuje komentare na zacatku radky
// predpoklad - vstupni string neobsahuje prazdne radky
ErrorCode parse_dkame(char* str, Automaton** automaton);

// precte string a vytvori z nej mooreho automat
// predpoklad - vstupni string neobsahuje komentare na zacatku radky
// predpoklad - vstupni string neobsahuje prazdne radky
ErrorCode parse_dkamo(char* str, Automaton** automaton);
