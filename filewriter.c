#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "filewriter.h"

static void autoclose(FILE** fp) {
	if (*fp) {
		fclose(*fp);
	}
}

static void write_dkam(Automaton* automaton, FILE* fp) {
	fprintf(fp, "%zu\n%zu\n", automaton->state_count, automaton->in_count);

	for (size_t i = 0; i < automaton->state_count; ++i) {
		fprintf(fp, "%c: ", (char)i + 'A');

		for (size_t j = 0; j < automaton->in_count; ++j) {
			bool found = false;

			for (size_t k = 0; k < automaton->transition_count; ++k) {
				if (automaton->transitions[k].from == (char)i && automaton->in[automaton->transitions[k].read] == (char)j + 'a') {
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
}

static void write_mealy_dkame(Automaton* automaton, FILE* fp) {
	fprintf(fp, "DKAME\n");

	write_dkam(automaton, fp);

	for (size_t i = 0; i < automaton->state_count; ++i) {
		fprintf(fp, "%c: ", (char)i + 'A');
		for (size_t j = 0; j < automaton->in_count; ++j) {
			bool found = false;

			for (size_t k = 0; k < automaton->transition_count; ++k) {
				if (automaton->transitions[k].from == (char)i && automaton->in[automaton->transitions[k].read] == (char)j + 'a') {
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
}

static void write_moore_dkamo(Automaton* automaton, FILE* fp) {
	fprintf(fp, "DKAMO\n");
	write_dkam(automaton, fp);

	for(size_t i = 0; i < automaton->out_count; ++i) {
		fprintf(fp, "%c ", automaton->out[i + 'A']);
	}

	fprintf(fp, "\n");
}

// TODO: Return error code
void write(Automaton* automaton, const char* file) {
	__attribute__((cleanup(autoclose))) FILE* fp = fopen(file, "w");

	if (!fp) {
		return;
	}

	switch (automaton->fstype) {
		case OUT_DKAME: {
			write_mealy_dkame(automaton, fp);
		} break;

		case OUT_DKAMO: {
			write_moore_dkamo(automaton, fp);
		} break;

		case OUT_XML: {

		} break;
	}
}