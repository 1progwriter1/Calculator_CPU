#ifndef VALUES_STACK
#define VALUES_STACK

#include "config.h"

// #define info(stk) PrintInfo(&stk, (char *)__FILE__, (char *)__func__, __LINE__);
#define STACK_CTOR(stk) StackCtor(&stk, #stk, (const char *)__FILE__, __LINE__, (const char *)__func__);

typedef unsigned long long canary_t;

const int RAISE = 1;
const int CUT = 0;

struct StackInfo {
    const char *name;
    const char *file;
    int line;
    const char *func;
};

struct Stack {
    canary_t canary_left;
    Elem_t *data;
    unsigned long size;
    unsigned long capacity;
    int id;
    StackInfo info;
    canary_t canary_right;
};


enum Result {
    SUCCESS,
    NO_MEMORY,
    INCORRECT_CAPACITY,
    INCORRECT_SIZE,
    OVERFLOW,
    CANARY_FAULT_STACK_LEFT,
    CANARY_FAULT_STACK_RIGHT,
    CANARY_FAULT_DATA_LEFT,
    CANARY_FAULT_DATA_RIGHT,
    HASH_ERROR,
    ID_ERROR,
    NULL_POINTER,
    ERROR,
    EMPTY,
    FILEOPEN_ERROR,
};

#endif
