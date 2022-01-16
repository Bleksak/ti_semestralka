#pragma once

// tabulka stringu pro vyhledavani v XML souboru
extern const char* STRING_TABLE[];

// enum pro indexaci v tabulce stringu
enum STR_TABLE {
	XML_HEADER = 0,
	XML_TYPE,
	XML_STATE_END,
	XML_READ,
	XML_INITIAL,
	XML_STATE,
	XML_TRANSITION,
	XML_FROM,
	XML_TO,
	XML_TRANSOUT,
	XML_OUTPUT,
};
