#include <stdio.h>
#include "assert.h"
#include "hash.h"
#include <stdlib.h>

//#undef PROTECT
//#define PROTECT

unsigned long sizehash = 8;
static unsigned long long *hash_data = NULL;

enum Result HashCreate(Stack *stk) {

//     if (!hash_data)
//         hash_data = (unsigned long long *) calloc (sizehash, sizeof (unsigned long long));
//
//     if (stk->id >= (int) sizehash) {
//          sizehash *= 2;
//         hash_data = (unsigned long long *) realloc (hash_data, sizehash * sizeof (unsigned long long));
//         if (!hash_data)
//             return NO_MEMORY;
//     }
//     hash_data[stk->id] = HashCount(stk);
    return SUCCESS;
}

void clean() {
    free(hash_data);
}

void HashClean(int id) {
    hash_data[id] = 0;
}