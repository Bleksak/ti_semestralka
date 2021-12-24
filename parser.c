#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automaton.h"
#include "parser.h"
#include "mealy.h"
#include "moore.h"

typedef void(*parser)(char*);

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
};

static void closefile(FILE** fp) {
	if (*fp) {
		fclose(*fp);
	}
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
	return automaton_get_type(strstr(str, STRING_TABLE[XML_TYPE]) + strlen(STRING_TABLE[XML_TYPE]));
}

void parse(const char* filename) {
	__attribute__((cleanup(closefile))) FILE* fp = fopen(filename, "r");
	if (!fp) {
		return;
	}

	fseek(fp, 0, SEEK_END);
	size_t len = (size_t)ftell(fp);
	rewind(fp);

	char* str = readlines(fp, len);

	AutomatonType type = get_type(str);

	if(type == TYPE_NONE) {
		// TODO: asdfasdfsdf
		return;
	}

	parsers[type](str);
}
