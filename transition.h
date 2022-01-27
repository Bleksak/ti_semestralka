#pragma once
#include <stdbool.h>

// popisuje prechod mezi dvema stavy automatu
typedef struct Transition {
    char from; // z jakeho stavu
    char to; // do ktereho stavu
    char read; // jakym znakem
    char transout; // vystupni symbol prechodu
    bool filled; // zda je prechod platny
} Transition;
