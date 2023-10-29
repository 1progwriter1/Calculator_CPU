typedef long long int vec_t;

struct Vector {
    vec_t *data;
    unsigned long size;
    unsigned long capacity;
};

int VectorCtor(Vector *vec, unsigned long size);
int VectorDtor(Vector *vec);
int PushBack(Vector *vec, vec_t num);
vec_t Pop(Vector *vec);