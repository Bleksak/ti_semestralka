#pragma once

enum STR_TABLE {
	XML_HEADER = 0,
	XML_TYPE,
	XML_STATE_END,
	XML_READ,
	XML_INITIAL,
	XML_STATE,
};

extern const char* STRING_TABLE[];

void parse(const char* filename);
