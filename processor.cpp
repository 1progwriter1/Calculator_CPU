#include <stdio.h>
#include "Stack/stack.h"
#include "Stack/stack_values.h"
#include <assert.h>
#include "processor.h"
#include <math.h>
#include "func.h"
#include "Stack/config.h"
#include "calculator_values.h"
#include <string.h>

#define VERIFY                      \
    if (!CalcVerify(calcdata)) {    \
        free(buf);                  \
        return ERROR;               \
    }                               \

#define PUSH_NUM(num) StackPush(&calcdata->data, num)

#define POP_NUM(num) StackPop(&calcdata->data, &num)

#define PUSH_ADR(num) StackPush(&calcdata->addresses, (Elem_t) num);

#define POP_ADR(num) StackPop(&calcdata->addresses, (Elem_t *) &num);

#define INPUT(num)                                                  \
    int correct = 0;                                                \
    do {                                                            \
        correct = 1;                                                \
        if (scanf(output_id, &num) != 1) {                          \
            printf("\033[31mIncorrect input. Try again\033[0m\n");  \
            clear();                                                \
            correct = 0;                                            \
        }                                                           \
    } while (!correct);                                             \

#define GET_ELEM(operation) ((Elem_t *) buf + index operation)

static const int PRECISION = 3;
static const int MUL_PRES = (int) pow(10, PRECISION);
const double PI = 3.14159265;
const int RAM_SIZE = 10000;

static void clear();

enum Result processor(Calc *calcdata, const char *file) {

    assert(calcdata);
    assert(file);

    Elem_t ram[RAM_SIZE] = {};

    unsigned long len_of_file = filelen(file);
    FILE *fn = fileopen(file, READ);

    char *bufptr = (char *) calloc (len_of_file + 1, sizeof (char));
    char *buf = bufptr;
    fread(buf, sizeof (char), len_of_file, fn);
    fileclose(fn);
    if (!bufptr)
        return NO_MEMORY;

    int index = 0;
    Elem_t sign = *((Elem_t *) buf + index++);
    Elem_t file_version = *((Elem_t *) buf + index++);
    if (!FileVerify((int) sign, VERSION, (int) file_version)) {
        free(bufptr);
        return ERROR;
    }
    Elem_t com_num = 0;

    printf("Processor version %d\nExecutable file: %s\n\n\033[" GREEN "mFile verified\033[0m\n\n", VERSION, file);

    #define DEF_CMD(name, code, args, ...)      \
        case (code): {                          \
            __VA_ARGS__                         \
            break;                              \
        }                                       \

    do {
        buf = bufptr;
        com_num = *((Elem_t *) buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf("\033[" RED "mIncorrect command for <processor> " output_id " %d\n\033[0m", com_num, index - 1);
                return ERROR;
                break;
            }
        }
    } while (com_num != CMD_HLT);

    #undef DEF_CMD

    free(bufptr);

    return SUCCESS;
}

enum Result CalcCtor(Calc *calcdata) {

    if (!calcdata)
        return NULL_POINTER;

    STACK_CTOR(calcdata->data);
    calcdata->reg = (Elem_t *) calloc (NUM_OF_REGS, sizeof (Elem_t));
    STACK_CTOR(calcdata->addresses);


    return SUCCESS;
}

enum Result CalcDtor(Calc *calcdata) {

    if (!calcdata)
        return NULL_POINTER;
    StackDtor(&calcdata->data);
    free(calcdata->reg);
    StackDtor(&calcdata->addresses);

    return SUCCESS;
}

int CalcVerify(Calc *calcdata) {

    assert(calcdata);

    unsigned int error = 0;
    error = StackVerify(&calcdata->data);
    int correct_reg = 1;
    if (!calcdata->reg)
        correct_reg = 0;
    if (error != 0 || !correct_reg) {
        DumpCalc(calcdata, error, correct_reg);
        return 0;
    }
    return 1;
}

void DumpCalc(Calc *calcdata, unsigned int error, int correct_reg) {

    if (!calcdata)
        printf("NULL calcdata\n");

    if (!correct_reg) {
        printf("Register: {");
        for (size_t i = 0; i < NUM_OF_REGS; i++)
            printf(output_id " ", calcdata->reg[i]);
        printf("\b}\n");
    }
    else {
        printf("NULL reg\n");
    }

    dump(&calcdata->data, error);
}

void clear() {
    while (getchar() != '\n')
        ;
}

int GetFileName(const int argc, const char *argv[], int *file_num) {

    assert(argv);
    assert(file_num);

    if (argc == 1) {
        printf("/033[" RED "mFile name expected\n\033[0m");
        return ERROR;
    }
    *file_num = 1;
    if (argc > 2)
        printf("/033[" RED "mUnused arguments\033[0m\n");

    return SUCCESS;
}