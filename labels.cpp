#include <stdio.h>
#include "labels.h"
#include <assert.h>
#include <string.h>

static const int INCREASE = 2;
int current = 0;
int size = START_LABELS_COL;
const int NUM_OF_ADR = 1;
const int LABELS = 0;
const int PROG_ADR = 1;

static int Resize(Label *labels, const int aim, const int number);

int SetLabel(Label *labels, char *name, int current_address, int address) {

    assert(labels);
    assert(name);

    if (current > 0) {
        for (size_t i = 0; i < (size_t) current; i++) {
            if (strcmp(name, labels[i].name) == 0) {
                if (address != 0)
                    labels[i].address = address;
                if (current_address != -1)
                    labels[i].program_address[labels[i].currant_address++] = current_address;
                return 0;
            }
        }
    }
    if (current >= size) {
        Resize(labels, LABELS, 0);
    }
    labels[current].name = (char *) calloc (strlen(name), sizeof (char));
    memcpy (labels[current].name, name, strlen (name));
    labels[current].address = address;
    labels[current].prog_adr_size = NUM_OF_ADR;
    labels[current].currant_address = 0;
    labels[current].program_address = (int *) calloc (NUM_OF_ADR, sizeof (int));
    if (current_address != -1) {
        if (labels[current].currant_address >= labels[current].prog_adr_size)
            Resize(labels, PROG_ADR, labels[current].currant_address);
        labels[current].program_address[labels[current].currant_address++] = current_address;
    }
    current++;

    return 0;
}

static int Resize(Label *labels, const int aim, const int number) {

    assert(labels);

    if (aim == LABELS) {
        size *= INCREASE;
        labels = (Label *) realloc (labels, sizeof (Label ) * (unsigned long) size);
        if (!labels)
            return 1;
        return 0;
    }

    labels[number].prog_adr_size *= INCREASE;
    labels[number].program_address = (int *) realloc (labels[number].program_address, sizeof (int) * (unsigned long) labels[number].prog_adr_size);
    if (!labels[number].currant_address)
        return 1;
    for (int i = labels[number].currant_address + 1; i < labels[number].prog_adr_size; i++)
        labels[number].program_address[i] = 0;

    return 0;
}