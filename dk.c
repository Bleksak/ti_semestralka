#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dk.h"

static size_t line_count(char* str) {
	size_t count = 1;

	do {
		if (*str == '\n') {
			count += 1;
		}
	} while (*str++);

	return count;
}

static ErrorCode getlines(char* str, size_t* count, char*** lines) {
	*count = line_count(str);

	*lines = calloc(*count, sizeof(*lines));

	if (!*lines) {
		return ERR_OUT_OF_MEMORY;
	}

	size_t i = 0;

	char* start = str;

	bool replaced;

	do {
		replaced = false;

		if (*str == '\n') {
			(*lines)[i] = start;
			start = str + 1;
			i += 1;
			*str = 0;
			replaced = true;
		}
	} while (*str++ || replaced);

	(*lines)[*count - 1] = start;

	return OK;
}

#define LINE_STATE_COUNT 1
#define LINE_IN_COUNT 2
#define LINE_IN 3
#define LINE_INITIAL_STATE 3
#define LINE_OUT_COUNT 4
#define LINE_OUT 5

ErrorCode parse_dkame(char* str, Automaton** automaton) {
	char** lines;
	size_t line_count;
	ErrorCode code = getlines(str, &line_count, &lines);

	if(code) {
		return code;
	}


	char* end;
	size_t state_count = strtoull(lines[LINE_STATE_COUNT], &end, 10);
	size_t in_count = strtoull(lines[LINE_IN_COUNT], &end, 10);

	Transition* transitions = calloc(state_count * in_count, sizeof(Transition));

	for (size_t i = 0; i < state_count; ++i) {
		char* line = lines[i + LINE_IN];
		char* split = strtok(line, " ");

		char from = line[0];
		size_t from_index = (size_t)(from - 'A');

		for (size_t j = 0; j < in_count; ++j) {
			split = strtok(NULL, " ");
			char to = *split;

			transitions[from_index * in_count + j].from = from;
			transitions[from_index * in_count + j].to = to;
			transitions[from_index * in_count + j].read = (char)j + 'a';
		}
	}

	char initial_state = *lines[state_count + LINE_INITIAL_STATE];

	size_t out_count = strtoull(lines[state_count + LINE_OUT_COUNT], &end, 10);

	for (size_t i = 0; i < state_count; ++i) {
		char* line = lines[state_count + LINE_OUT + i];
		char* split = strtok(line, " ");
		char from = line[0];
		size_t from_index = (size_t)(from - 'A');

		for (size_t j = 0; j < out_count; ++j) {
			split = strtok(NULL, " ");

			if(!split) {
				free(transitions);
				free(lines);
				return BAD_FILE;
			}

			transitions[from_index * out_count + j].transout = strtoul(split, &end, 10);
		}
	}

	free(lines);

	*automaton = automaton_dk_new(TYPE_DKAME, state_count, in_count, out_count, state_count * in_count, transitions, initial_state);

	return OK;
}

ErrorCode parse_dkamo(char* str, Automaton** automaton) {
	char** lines;
	size_t line_count;

	ErrorCode code = getlines(str, &line_count, &lines);

	if(code) {
		return code;
	}

	char* end;
	size_t state_count = strtoull(lines[LINE_STATE_COUNT], &end, 10);
	size_t in_count = strtoull(lines[LINE_IN_COUNT], &end, 10);

	Transition* transitions = calloc(state_count * in_count, sizeof(Transition));

	if (!transitions) {
		free(lines);
		return ERR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < state_count; ++i) {
		char* line = lines[i + LINE_IN];
		char* split = strtok(line, " ");

		char from = line[0];
		size_t from_index = (size_t)(from - 'A');

		for (size_t j = 0; j < in_count; ++j) {
			split = strtok(NULL, " ");
			char to = *split;

			transitions[from_index * in_count + j].from = from;
			transitions[from_index * in_count + j].to = to;
			transitions[from_index * in_count + j].read = (char)j + 'a';
		}
	}

	char initial_state = *lines[state_count + LINE_INITIAL_STATE];
	size_t out_count = strtoull(lines[state_count + LINE_OUT_COUNT], &end, 10);

	char* line = strtok(lines[state_count + LINE_OUT], " ");

	for (size_t i = 0; i < state_count; ++i) {
		for (size_t j = 0; j < in_count; ++j) {

			if(!line) {
				free(transitions);
				free(lines);
				return BAD_FILE;
			}

			transitions[i * in_count + j].transout = strtoul(line, &end, 10);
		}

		line = strtok(NULL, " ");
	}

	free(lines);

	*automaton = automaton_dk_new(TYPE_DKAMO, state_count, in_count, out_count, state_count * in_count, transitions, initial_state);

	return OK;
}
