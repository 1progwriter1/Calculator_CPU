#ifndef STACK_STACK
#define STACK_STACK

#include "stack_values.h"

#define dump(stk, error) StackDump(error, (const char *)__FILE__, __LINE__, (const char *)__func__, stk);

enum Result StackCtor(Stack *stk, const char *name, const char *file, const int line, const char *func);
enum Result StackPop(Stack *stk, Elem_t *n);
enum Result StackPush(Stack *stk, Elem_t n);
enum Result StackDtor(Stack *stk);
void PrintStack(const Stack *stk);
void Detor();
void fileclose(FILE *fn);
FILE *fileopen(const char *filename);
unsigned int StackVerify(const Stack *stk);
void StackDump(unsigned int error, const char *file, const int line, const char *func, const Stack *stk);

#endif
