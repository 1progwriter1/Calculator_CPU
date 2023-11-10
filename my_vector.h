#ifndef VECTOR_CPU
#define VECTOR_CPU

typedef long long int Vec_t;

struct Vector {
    Vec_t *data;
    unsigned long size;
    unsigned long capacity;
};

int VectorCtor(Vector *vec, unsigned long size);
int VectorDtor(Vector *vec);
int PushBack(Vector *vec, Vec_t num);
int Pop(Vector *vec, Vec_t *num);

#endif