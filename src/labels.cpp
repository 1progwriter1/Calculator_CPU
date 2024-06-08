#include <stdio.h>
#include "labels.h"
#include <assert.h>
#include <string.h>
#include "calculator_values.h"
#include <stdlib.h>

const int INCREASE = 2;
const int START_ADDR_COL = 2;
const int START_LABELS_COL = 8;

static int ResizeLabels(Labels *lbls);
static int ResizeAddresses(Label *data);

int SetLabel(Labels *lbls, char *name, int current_address, int address) {

    assert(lbls);
    assert(name);

    for (size_t i = 0; i < (size_t) lbls->current; i++) {
        if (strcmp(name, (lbls->data + i)->name) == 0) {
            if (current_address != NO_ADDRESS) {
                if ((lbls->data + i)->current + 1 >= (lbls->data + i)->size)
                    if (ResizeAddresses(lbls->data) != SUCCESS)
                        return NO_MEMORY;
                (lbls->data + i)->addresses_to_fill[(lbls->data + i)->current++] = current_address;
            }
            if (address != NO_ADDRESS)
                (lbls->data + i)->address = address;
            return SUCCESS;
        }
    }
    if (lbls->current + 1 >= lbls->size)
        if (ResizeLabels(lbls) != SUCCESS)
            return NO_MEMORY;

    Label *ptr = lbls->data + lbls->current++;
    ptr->name = (char *) calloc (strlen(name) + 1, sizeof (char));
    memcpy (ptr->name, name, strlen (name));
    if (address != NO_ADDRESS)
        ptr->address = address;
    ptr->current = 0;
    ptr->size = START_ADDR_COL;
    ptr->addresses_to_fill = (int *) calloc (START_ADDR_COL, sizeof (int));
    if (!ptr->addresses_to_fill)
        return NO_MEMORY;

    if (current_address != NO_ADDRESS)
        ptr->addresses_to_fill[ptr->current++] = current_address;

    return SUCCESS;
}

static int ResizeLabels(Labels *lbls) {

    assert(lbls);

    int tmp = lbls->size;
    lbls->size *= INCREASE;

    lbls->data = (Label *) realloc (lbls->data, sizeof (Label) * (unsigned long) lbls->size);
    if (!lbls->data)
        return NO_MEMORY;

    for (size_t i = tmp; i < lbls->data->size; i++)
        lbls->data[i].name = NULL;

    return SUCCESS;
}

static int ResizeAddresses(Label *data) {

    assert(data);

    data->addresses_to_fill = (int *) realloc (data->addresses_to_fill, sizeof (int) * (unsigned long) data->size * INCREASE);
    if (!data->addresses_to_fill)
        return NO_MEMORY;

    for (size_t i = (size_t) data->size; i < (size_t) data->size * INCREASE; i++)
        data->addresses_to_fill[i] = 0;

    data->size *= INCREASE;

    return SUCCESS;
}

int LabelsCtor(Labels *lbls) {

    assert(lbls);

    lbls->data = (Label *) calloc (START_LABELS_COL, sizeof (Label));
    if (!lbls->data)
        return NO_MEMORY;

    lbls->current = 0;
    lbls->size = START_LABELS_COL;

    for (size_t i = 0; i < lbls->data->size; i++)
        lbls->data[i].name = NULL;

    return SUCCESS;
}

int LabelsDtor(Labels *lbls) {

    assert(lbls);
    assert(lbls->data);

    size_t index = 0;
    while (index < lbls->current && lbls->data[index].name) {
        free(lbls->data[index].addresses_to_fill);
        (lbls->data + index)->address = -1;
        (lbls->data + index)->current = -1;
        free((lbls->data + index)->name);
        (lbls->data + index++)->size = -1;
    }

    free(lbls->data);
    lbls->current = -1;
    lbls->size = -1;

    return SUCCESS;
}