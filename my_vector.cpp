#include <stdio.h>
#include "my_vector.h"
#include <assert.h>
#include "calculator_values.h"
#include <stdlib.h>

const int INCREASE = 2;
const int RAISE_VECTOR = 1;
const int CUT_VECTOR = 0;
const int MIN_SIZE = 8;

static int Resize(Vector *vec, const int is_increase);

int VectorCtor(Vector *vec, unsigned long size) {

    assert(vec);

    vec->data = (Vec_t *) calloc (size, sizeof (Vec_t));
    if (!vec->data)
        return NO_MEMORY;

    vec->size = 0;
    vec->capacity = size;

    return SUCCESS;
}

int VectorDtor(Vector *vec) {

    assert(vec);

    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;

    return SUCCESS;
}

int PushBack(Vector *vec, Vec_t num) {

    assert(vec);

    if (vec->size >= vec->capacity) {
        if (Resize(vec, RAISE_VECTOR) != SUCCESS)
            return NO_MEMORY;
    }
    vec->data[vec->size++] = num;

    return SUCCESS;
}

int Pop(Vector *vec, Vec_t *num) {

    assert(vec);

    if (vec->size - 1 < 0) {
        return ERROR;
    }

    *num = vec->data[--vec->size];
    if (vec->capacity > MIN_SIZE && vec->capacity - vec->size > vec->capacity - vec->capacity / INCREASE) {
        if (Resize(vec, CUT_VECTOR) != SUCCESS)
            return NO_MEMORY;
    }
    return SUCCESS;

}
static int Resize(Vector *vec, const int is_increase) {

    assert(vec);

    if (is_increase) {
        vec->capacity *= INCREASE;
    }
    else {
        vec->capacity /= INCREASE;
    }

    vec->data = (Vec_t *) realloc (vec->data, sizeof (Vec_t) * vec->capacity);
    if (!vec->data)
        return NO_MEMORY;

    for (size_t i = vec->size; i < vec->capacity; i++) {
        vec->data[i] = 0;
    }
    return SUCCESS;
}