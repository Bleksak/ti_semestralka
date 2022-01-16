#pragma once

// popisuje prechod mezi dvema stavy automatu
typedef struct Transition {
    char from; // z jakeho stavu
    char to; // do ktereho stavu
    char read; // jakym znakem
    char transout; // vystupni symbol prechodu
} Transition;
