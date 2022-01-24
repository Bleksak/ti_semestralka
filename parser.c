#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automaton.h"
#include "dk.h"
#include "mealy.h"
#include "moore.h"
#include "parser.h"
#include "string_table.h"

#define DKAME "DKAME"
#define DKAMO "DKAMO"
#define MEALY "mealy"
#define MOORE "moore"

typedef ErrorCode (*parser)(char*, Automaton**);

// pole parseru pro ruzne datove formaty
static const parser parsers[] = {
	NULL,
	parse_mealy,
	parse_moore,
	parse_dkame,
	parse_dkamo,
};

// precte cely soubor do jednoho retezce a z kazdeho radku umaze zleva vsechny bile znaky
static ErrorCode readfile(FILE* fp, char** str) {
	fseek(fp, 0, SEEK_END);
	long file_len = ftell(fp);
	rewind(fp);

	if(file_len == 0) {
		return BAD_FILE;
	}

	*str = calloc(file_len + 1, sizeof(char));

	if (!*str) {
		return ERR_OUT_OF_MEMORY;
	}

	// kam zapisovat zacatek radky
	char* start = *str;

	// kolik znaku bylo doposud nacteno
	long count = 0;

	// Cyklus ktery cte soubor radek po radce a maze ze zacatku radku whitespace znaky
	while (fgets(start, file_len - count + 1, fp) && file_len - count > 0) {
		size_t move_back = 0;

		// newline je whitespace, zaroven znak 0 neni, takze tento cyklus maze i prazdne radky
		while (isspace(start[move_back])) {
			move_back += 1;
		}

		size_t read = strlen(start + move_back);

		memmove(start, start + move_back, read);

		count += read;
		start += read;
	}

	// soubor obsahoval pouze bile znaky
	if(count == 0) {
		free(*str);
		return BAD_FILE;
	}

	// shrink to fit
	void* old = *str;
	*str = realloc(*str, count + 1);

	// tohle by se nikdy nemelo stat, ale valgrindu to vadilo
	if (!*str) {
		free(old);
		return ERR_OUT_OF_MEMORY;
	}

	return OK;
}

// zjisti, zda je soubor typu XML
static bool is_xml(const char* str) {
	return *str == '<';
}

// zjisti ze stringu typ souboru (automatu)
static AutomatonType get_type(const char* str) {
	if (is_xml(str)) {
		char* s = strstr(str, STRING_TABLE[XML_TYPE]) + strlen(STRING_TABLE[XML_TYPE]);

		if (strncmp(s, MEALY, sizeof(MEALY) - 1) == 0) {
			return TYPE_MEALY;
		}

		if (strncmp(s, MOORE, sizeof(MOORE) - 1) == 0) {
			return TYPE_MOORE;
		}
	}

	if (strncmp(str, DKAME, sizeof(DKAME) - 1) == 0) {
		return TYPE_DKAME;
	}

	if (strncmp(str, DKAMO, sizeof(DKAMO) - 1) == 0) {
		return TYPE_DKAMO;
	}

	return TYPE_NONE;
}

// precte soubor a vytvori automat, ktery lze prepsat do jineho datoveho formatu
ErrorCode parse(const char* filename, Automaton** automaton) {
	FILE* fp = fopen(filename, "r");

	if (!fp) {
		return BAD_FILE;
	}

	char* str;

	ErrorCode code = readfile(fp, &str);

	fclose(fp);

	if (code) {
		return code;
	}

	// ziskame typ souboru a podle toho parsujeme dal
	AutomatonType type = get_type(str);
	code = parsers[type](str, automaton);

	free(str);
	return code;
}

// zjisti pocet stavu automatu
size_t get_state_count(char* str) {
	size_t count = 0;
	size_t len = strlen(STRING_TABLE[XML_STATE_END]);

	// dokud najde string </state> pricitej 1
	while ((str = strstr(str, STRING_TABLE[XML_STATE_END]))) {
		count += 1;
		str += len;
	}

	return count;
}

// vyplni tabulku vstupnich znaku
// vrati jejich pocet
// tabulka urcuje na ktery znak se zobrazi znak v <read> tagu
size_t get_in_characters(char* str, char ascii[]) {
	// TODO: mozna potrebujem hashset namisto ascii tabulky
	size_t count = 0;
	size_t len = strlen(STRING_TABLE[XML_READ]);

	// najde <read>, pokud je znak v tabulce, uz ho neprirazujeme
	// jinak mu nastavime hodnotu count + 'a' (vstupni symboly jsou pismena a, b, c, d...)
	// a inkrementujeme count
	while ((str = strstr(str, STRING_TABLE[XML_READ]))) {
		str += len;
		if (!ascii[(size_t)*str]) {
			ascii[(size_t)*str] = (char)count + 'a';
			count += 1;
		}
	}

	return count;
}

// najde a vrati vychozi stav automatu, pokud zadny nenajde, vychozi stav je implicitne 'A'
char get_initial_state(char* str) {
	size_t len = strlen(STRING_TABLE[XML_STATE]);
	// hledame <initial/>
	char* end = strstr(str, STRING_TABLE[XML_INITIAL]);

	// pokud nebyl nalezen tato podminka neni splnena (str > NULL)
	// jinak prochazime od <initial/> zpet, az dokud nenajdeme <state ... id="ID">
	// a extrahujeme ID
	while (end >= str) {
		if (strncmp(end, STRING_TABLE[XML_STATE], len) == 0) {
			char* id_start = end + strlen(STRING_TABLE[XML_INITIAL]) + 1;
			char* endptr;

			return (char)strtoul(id_start, &endptr, 10) + 'A';
		}
		end -= 1;
	}

	// pokud vychozi stav nebyl nalezen, vracime 'A'
	return 'A';
}

// spocita pocet prechodu v automatu
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

// naplni prechodovou tabulku (z ktereho stavu, do ktereho stavu, pres ktery symbol, ktery symbol se vypise)
ErrorCode get_transitions(char* str, size_t* count, Transition** _transitions, char* in, char* out) {
	*count = get_transition_count(str);

	if(*count == 0) {
		return OK;
	}

	*_transitions = calloc(*count, sizeof(Transition));

	Transition* transitions = *_transitions;
	if (!transitions) {
		return ERR_OUT_OF_MEMORY;
	}

	// hledame <transition>
	const char* transition = STRING_TABLE[XML_TRANSITION];
	size_t len = strlen(transition);
	char* last_transition = str;

	// od <transition> hledame <from>, <to>, <transout>, <read>
	size_t from_strlen = strlen(STRING_TABLE[XML_FROM]);
	size_t to_strlen = strlen(STRING_TABLE[XML_TO]);
	size_t transout_strlen = strlen(STRING_TABLE[XML_TRANSOUT]);
	size_t read_strlen = strlen(STRING_TABLE[XML_READ]);

	for (size_t i = 0; i < *count; ++i) {
		last_transition = strstr(last_transition, transition);

		if(!last_transition) {
			free(transitions);
			return BAD_FILE;
		}

		char* dummy;

		char* fromstr = strstr(last_transition, STRING_TABLE[XML_FROM]);
		char* tostr = strstr(last_transition, STRING_TABLE[XML_TO]);
		char* transoutstr = strstr(last_transition, STRING_TABLE[XML_TRANSOUT]);
		char* readstr = strstr(last_transition, STRING_TABLE[XML_READ]);

		// pokud jedna z techto veci nebyla nalezena, soubor je neplatny
		if (!fromstr || !tostr || !transoutstr || !readstr) {
			free(transitions);
			return BAD_FILE;
		}

		transitions[i].from = (char)strtoul(fromstr + from_strlen, &dummy, 10);
		transitions[i].to = (char)strtoul(tostr + to_strlen, &dummy, 10);
		transitions[i].read = in[ (size_t) *(readstr + read_strlen)];

		// TODO: zjistit jestli tohle je spravne
		// FIXME: tady asi musime pouzit out[] pole
		// transout je vzdy 1 znak.. mozna staci to co mame
		// transitions[i].transout = *(transoutstr + transout_strlen) - 'a' + 1;
		transitions[i].transout = out[ (size_t) *(transoutstr + transout_strlen)];

		last_transition += len;
	}

	return OK;
}
