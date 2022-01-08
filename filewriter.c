#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "filewriter.h"

#include "string_table.h"

static void autoclose(FILE** fp) {
	if (*fp) {
		fclose(*fp);
	}
}

static ErrorCode write_dkam(Automaton* automaton, FILE* fp) {
	fprintf(fp, "%zu\n%zu\n", automaton->state_count, automaton->in_count);

	for (size_t i = 0; i < automaton->state_count; ++i) {
		fprintf(fp, "%c: ", (char)i + 'A');

		for (size_t j = 0; j < automaton->in_count; ++j) {
			bool found = false;

			for (size_t k = 0; k < automaton->transition_count; ++k) {
				if (automaton->transitions[k].from == (char)i && automaton->in[(size_t)automaton->transitions[k].read] == (char)j + 'a') {
					fprintf(fp, "%c ", automaton->transitions[k].to + 'A');
					found = true;
					break;
				}
			}

			if (!found) {
				fprintf(fp, "%c ", (char)i + 'A');
			}
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "%c\n%zu\n", automaton->initial_state, automaton->out_count);

	return OK;
}

static ErrorCode write_mealy_dkame(Automaton* automaton, FILE* fp) {
	fprintf(fp, "DKAME\n");

	write_dkam(automaton, fp);

	for (size_t i = 0; i < automaton->state_count; ++i) {
		fprintf(fp, "%c: ", (char)i + 'A');
		for (size_t j = 0; j < automaton->in_count; ++j) {
			bool found = false;

			for (size_t k = 0; k < automaton->transition_count; ++k) {
				if (automaton->transitions[k].from == (char)i && automaton->in[(size_t)automaton->transitions[k].read] == (char)j + 'a') {
					fprintf(fp, "%d ", automaton->transitions[k].transout);
					found = true;
					break;
				}
			}

			if (!found) {
				fprintf(fp, "%zu ", i + 1);
			}
		}
		fprintf(fp, "\n");
	}

	return OK;
}

static ErrorCode write_moore_dkamo(Automaton* automaton, FILE* fp) {
	fprintf(fp, "DKAMO\n");
	write_dkam(automaton, fp);

	for (size_t i = 0; i < automaton->out_count; ++i) {
		fprintf(fp, "%c ", automaton->out[i + 'A']);
	}

	fprintf(fp, "\n");

	return OK;
}

static ErrorCode write_xml(Automaton* automaton, FILE* fp) {
	fprintf(fp, "%s\n<structure>\n", STRING_TABLE[XML_HEADER]);

	fprintf(fp, "\t<type>%s</type>\n", automaton->type == TYPE_DKAME ? "mealy" : "moore");
	fprintf(fp, "\t<automaton>\n");

	for (size_t i = 0; i < automaton->state_count; ++i) {
		fprintf(fp, "\t\t<state id=\"%zu\" name=\"%c\">", i, (char)i + 'A');

		
		if(automaton->initial_state == (char) i + 'A') {
			fprintf(fp, "\n\t\t\t<initial/>");
		}

		if(automaton->type == TYPE_DKAMO) {
			char out = '1';
			for(size_t j = 0; j < automaton->transition_count; ++j) {
				if(automaton->transitions[j].to == (char) i + 'A') {
					out = automaton->transitions[j].transout;
				}
			}

			fprintf(fp, "\n\t\t\t<output>%c</output>", out);
		}

		fprintf(fp, "\n\t\t</state>\n");
	}

	for(size_t i = 0; i < automaton->transition_count; ++i) {
		fprintf(fp, "\t\t<transition>\n");

		fprintf(fp, "\t\t\t<from>%c</from>\n", automaton->transitions[i].from - 'A' + '0');
		fprintf(fp, "\t\t\t<to>%c</to>\n", automaton->transitions[i].to - 'A' + '0');
		fprintf(fp, "\t\t\t<read>%c</read>\n", automaton->transitions[i].read);
		fprintf(fp, "\t\t\t<transout>%c</transout>\n", automaton->transitions[i].transout);

		fprintf(fp, "\t\t</transition>\n");
	}

	fprintf(fp, "\t</automaton>\n");
	fprintf(fp, "</structure>");

	return OK;
}

ErrorCode write(Automaton* automaton, const char* file) {
	__attribute__((cleanup(autoclose))) FILE* fp = fopen(file, "w");

	if (!fp) {
		return BAD_FILE;
	}

	switch (automaton->type) {
		case TYPE_MEALY: return write_mealy_dkame(automaton, fp);
		case TYPE_MOORE: return write_moore_dkamo(automaton, fp);
	}

	return write_xml(automaton, fp);
}
