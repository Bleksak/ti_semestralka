#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automaton.h"
#include "dkame.h"
#include "dkamo.h"
#include "mealy.h"
#include "moore.h"
#include "parser.h"

#define DKAME "DKAME"
#define DKAMO "DKAMO"

typedef Automaton* (*parser)(char*);

const parser parsers[] = {
	NULL,
	parse_mealy,
	parse_moore,
};

const char* STRING_TABLE[] = {
	"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>",
	"<type>",
	"</state>",
	"<read>",
	"<initial/>",
	"<state id=\"",
	"<transition>",
	"<from>",
	"<to>",
	"<transout>",
	"<output>",
};

static void closefile(FILE** fp) {
	if (*fp) {
		fclose(*fp);
	}
}

static bool is_xml(char* str) {
	return *str == '<';
}

static char* readlines(FILE* fp, size_t file_len) {
	char* str = calloc(file_len + 1, sizeof(char));
	char* start = str;

	size_t count = 0;

	while (fgets(start, file_len - count, fp)) {
		size_t move_back = 0;

		while (isspace(start[move_back])) {
			move_back += 1;
		}

		size_t read = strlen(start + move_back);

		memmove(start, start + move_back, read);

		if (start[read - 1] == '\n') {
			read -= 1;
		}

		count += read;
		start += read;
	}

	str = realloc(str, count + 1);

	return str;
}

static AutomatonType get_type(const char* str) {
	char* s = strstr(str, STRING_TABLE[XML_TYPE]);

	if (s != NULL) {
		return automaton_get_type(s + strlen(STRING_TABLE[XML_TYPE]));
	}

	if (strncmp(str, DKAME, sizeof(DKAME) - 1) == 0) {
		return TYPE_DKAME;
	}

	if (strncmp(str, DKAMO, sizeof(DKAMO) - 1) == 0) {
		return TYPE_DKAMO;
	}

	return TYPE_NONE;
}

Automaton* parse_xml(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	size_t len = (size_t)ftell(fp);
	rewind(fp);

	char* str = readlines(fp, len);

	AutomatonType type = get_type(str);

	if (type == TYPE_NONE) {
		// TODO: asdfasdfsdf
		return NULL;
	}

	return parsers[type](str);
}

Automaton* parse_dk(FILE* fp, AutomatonType type) {
	rewind(fp);
	switch (type) {
		case TYPE_DKAME: {
			return parse_dkame(fp);
		} break;

		case TYPE_DKAMO: {
			return parse_dkamo(fp);
		} break;
	}
}

Automaton* parse(const char* filename) {
	__attribute__((cleanup(closefile))) FILE* fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}

	char buffer[512];
	fgets(buffer, sizeof(buffer), fp);
	if (is_xml(buffer)) {
		return parse_xml(fp);
	}

	return parse_dk(fp, get_type(buffer));
}

size_t get_state_count(char* str) {
	size_t count = 0;
	size_t len = strlen(STRING_TABLE[XML_STATE_END]);

	while ((str = strstr(str, STRING_TABLE[XML_STATE_END]))) {
		count += 1;
		str += len;
	}

	return count;
}

size_t get_in_characters(char* str, char ascii[]) {
	// TODO: mozna potrebujem hashset namisto ascii tabulky
	size_t count = 0;
	size_t len = strlen(STRING_TABLE[XML_READ]);

	while ((str = strstr(str, STRING_TABLE[XML_READ]))) {
		str += len;
		if (!ascii[*str]) {
			ascii[*str] = count + 'a';
			count += 1;
		}
	}

	return count;
}

char get_initial_state(char* str) {
	size_t len = strlen(STRING_TABLE[XML_STATE]);
	char* end = strstr(str, STRING_TABLE[XML_INITIAL]);

	while (end >= str) {
		if (strncmp(end, STRING_TABLE[XML_STATE], len) == 0) {
			char* id_start = end + strlen(STRING_TABLE[XML_INITIAL]) + 1;
			char* endptr;

			return strtoul(id_start, &endptr, 10) + 'A';
		}
		end -= 1;
	}

	return 'A';
}

size_t get_transition_count(char* str) {
	size_t count = 0;
	const char* transition = STRING_TABLE[XML_TRANSITION];
	size_t len = strlen(transition);
	char* last_transition = str;

	while ((last_transition = strstr(last_transition, transition)) != NULL) {
		count += 1;
		last_transition += len;
	}

	return count;
}

// TODO: Return error code
size_t get_transitions(char* str, Transition** _transitions) {
	size_t count = get_transition_count(str);
	*_transitions = calloc(count, sizeof(Transition));

	Transition* transitions = *_transitions;
	if (!transitions) {
		return 0;
	}

	const char* transition = STRING_TABLE[XML_TRANSITION];
	size_t len = strlen(transition);
	char* last_transition = str;

	size_t from_strlen = strlen(STRING_TABLE[XML_FROM]);
	size_t to_strlen = strlen(STRING_TABLE[XML_TO]);
	size_t transout_strlen = strlen(STRING_TABLE[XML_TRANSOUT]);
	size_t read_strlen = strlen(STRING_TABLE[XML_READ]);

	for (size_t i = 0; i < count; ++i) {
		last_transition = strstr(last_transition, transition);
		char* dummy;

		char* fromstr = strstr(last_transition, STRING_TABLE[XML_FROM]);
		char* tostr = strstr(last_transition, STRING_TABLE[XML_TO]);
		char* transoutstr = strstr(last_transition, STRING_TABLE[XML_TRANSOUT]);
		char* readstr = strstr(last_transition, STRING_TABLE[XML_READ]);

		if (!fromstr || !tostr || !transoutstr || !readstr) {
			// TODO: RETURN ERROR
			return 0;
		}

		transitions[i].from = strtoul(fromstr + from_strlen, &dummy, 10);
		transitions[i].to = strtoul(tostr + to_strlen, &dummy, 10);

		transitions[i].read = *(readstr + read_strlen);
		transitions[i].transout = *(transoutstr + transout_strlen) - 'a' + 1;

		last_transition += len;
	}

	return count;
}
