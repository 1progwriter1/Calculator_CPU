#include <stdio.h>
#include "my_vector.h"
#include <assert.h>

static const int INCREASE = 2;
static const int RAISE = 1;
static const int CUT = 0;
static const int MIN_SIZE = 8;

static int Resize(Vector *vec, const int is_increase);

int VectorCtor(Vector *vec, unsigned long size) {

    assert(vec);

    vec->data = (vec_t *) calloc (size, sizeof (vec_t));
    vec->size = 0;
    vec->capacity = size;

    return 0;
}

int VectorDtor(Vector *vec) {

    assert(vec);

    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;

    return 0;
}

int PushBack(Vector *vec, vec_t num) {

    assert(vec);

    if (vec->size >= vec->capacity) {
        Resize(vec, RAISE);
    }
    vec->data[vec->size++] = num;

    return 0;
}

vec_t Pop(Vector *vec) {

    assert(vec);

    if (vec->size - 1 < 0) {
        return NULL;
    }

    vec_t num = vec->data[--vec->size];
    if (vec->capacity > MIN_SIZE && vec->capacity - vec->size > vec->capacity - vec->capacity / 2) {
        Resize(vec, CUT);
    }
    return num;

}
static int Resize(Vector *vec, const int is_increase) {

    assert(vec);

    if (is_increase) {
        vec->capacity *= INCREASE;
    }
    else {
        vec->capacity /= INCREASE;
    }

    vec->data = (vec_t *) realloc (vec->data, sizeof (vec_t) * vec->capacity);
    if (vec->data)
        return 0;

    for (size_t i = vec->size; i < vec->capacity; i++) {
        vec->data[i] = 0;
    }
    return 1;
}