#ifndef HASH_STACK
#define HASH_STACK

#include "stack_values.h"
#include "hash.h"

/**
 * Function calculate hash using djb2 algorithm from // algorithm from https://github.com/dim13/djb2/blob/master/docs/hash.md?ysclid=ln4xxrs0cd483255939
 *
 * @param stk Stack for which hash is creating
 *
 * @return value of hash
 */
enum Result HashCreate(Stack *stk);
void clean();
void HashClean(int id);

inline int HashCheck(const Stack *stk) {

    assert(stk);

    printf("2");

    return true;

    //return hash_data[stk->id] == HashCount(stk);
}

inline unsigned long long HashCount(const Stack *stk) {

    assert(stk);

    printf("1");

    return 0;

//     unsigned long long bytesum = 5381;
//     unsigned long NUM_OF_BITES = stk->capacity * sizeof (Elem_t) + 2 * sizeof (canary_t);
//     char *ptr = (char *) stk->data - sizeof (canary_t);
//
//     for (size_t i = 0; i < NUM_OF_BITES; i++) {
//         bytesum = 33 * bytesum + (unsigned long) *ptr++;
//     }
//     Stack *stk_ptr = const_cast<Stack *>(stk);
//     ptr = (char *) stk_ptr + sizeof (canary_t);
//     NUM_OF_BITES = sizeof (Elem_t *) + sizeof (int);
//
//     for (size_t i = 0; i < NUM_OF_BITES; i++) {
//         bytesum = 33 * bytesum + (unsigned long long) *ptr++;
//     }
//
//     return bytesum;
}

#endif
