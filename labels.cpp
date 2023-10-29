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
static int Resize(Labels *fixups, const int aim, const int number);

int SetLabel(Labels *fixups, char *name, int current_address, int address) {

    assert(fixups);
    assert(name);

    if (current > 0) {
        for (size_t i = 0; i < (size_t) current; i++) {
            if (strcmp(name, fixups[i].name) == 0) {
                if (address != 0)
                    fixups[i].address = address;
                if (current_address != -1)
                    fixups[i].program_address[fixups[i].currant_address++] = current_address;
                return 0;
            }
        }
    }
    if (current >= size) {
        Resize(fixups, LABELS, 0);
    }
    fixups[current].name = (char *) calloc (strlen(name), sizeof (char));
    memcpy (fixups[current].name, name, strlen (name));
    fixups[current].address = address;
    fixups[current].prog_adr_size = NUM_OF_ADR;
    fixups[current].currant_address = 0;
    fixups[current].program_address = (int *) calloc (NUM_OF_ADR, sizeof (int));
    if (current_address != -1) {
        if (fixups[current].currant_address >= fixups[current].prog_adr_size)
            Resize(fixups, PROG_ADR, fixups[current].currant_address);
        fixups[current].program_address[fixups[current].currant_address++] = current_address;
    }
    current++;

    return 0;
}

static int Resize(Labels *fixups, const int aim, const int number) {

    assert(fixups);

    if (aim == LABELS) {
        size *= INCREASE;
        fixups = (Labels *) realloc (fixups, sizeof (Labels ) * (unsigned long) size);
        if (!fixups)
            return 1;
        return 0;
    }

    fixups[number].prog_adr_size *= INCREASE;
    fixups[number].program_address = (int *) realloc (fixups[number].program_address, sizeof (int) * (unsigned long) fixups[number].prog_adr_size);
    if (!fixups[number].currant_address)
        return 1;
    for (int i = fixups[number].currant_address + 1; i < fixups[number].prog_adr_size; i++)
        fixups[number].program_address[i] = 0;

    return 0;
}