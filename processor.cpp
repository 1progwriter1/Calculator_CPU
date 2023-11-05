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
    if (!CalcVerify(processor)) {   \
        free(buf);                  \
        return ERROR;               \
    }                               \

#define PUSH_NUM(num) StackPush(&processor->data, num)

#define POP_NUM(num) StackPop(&processor->data, &num)

#define PUSH_ADR(num) StackPush(&processor->addresses, (Elem_t) num);

#define POP_ADR(num) StackPop(&processor->addresses, (Elem_t *) &num);

#define INPUT(num)                                                  \
    int correct = 0;                                                \
    do {                                                            \
        correct = 1;                                                \
        if (scanf(output_id, &num) != 1) {                          \
            printf(RED "Incorrect input. Try again:\n " END_OF_COLOR);\
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
static int PrepareForExecution(const char *filename, char *buf, int *index, unsigned long len_of_file);
static int DoCodeExecution(Calc *processor, char *buf, int index, Elem_t *ram);

enum Result ExecuteProgram(Calc *processor, const char *byte_code_file) {

    assert(processor);
    assert(byte_code_file);

    unsigned long len_of_file = filelen(byte_code_file);
    char *buf = (char *) calloc (len_of_file + 1, sizeof (char));
    int index = INDEX_OF_CODE_LEN + 1;

    if (PrepareForExecution(byte_code_file, buf, &index, len_of_file) != SUCCESS)
        return ERROR;

    printf("Processor (version %d)\n" "Executable file: %s\n\n" GREEN "File verified" END_OF_COLOR "\n\n", VERSION, byte_code_file);

    Elem_t ram[RAM_SIZE] = {};
    if (DoCodeExecution(processor, buf, index, ram) != SUCCESS) {
        free(buf);
        return ERROR;
    }

    free(buf);

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
        printf(RED "File name expected\n" END_OF_COLOR);
        return ERROR;
    }
    *file_num = 1;
    if (argc > 2)
        printf(RED "Unused arguments\n" END_OF_COLOR);

    return SUCCESS;
}

static int PrepareForExecution(const char *filename, char *buf, int *index, unsigned long len_of_file) {

    assert(filename);

    FILE *byte_code_file = fileopen(filename, READ);
    if (!byte_code_file)
        return ERROR;

    if (!buf) {
        fileclose(byte_code_file);
        return NO_MEMORY;
    }

    fread(buf, sizeof (char), len_of_file, byte_code_file);
    fileclose(byte_code_file);

    Elem_t signature = *((Elem_t *) buf + (*index)++);
    Elem_t file_version = *((Elem_t *) buf + (*index)++);
    if (FileVerify((int) signature, (int) file_version) != SUCCESS) {
        free(buf);
        return ERROR;
    }

    return SUCCESS;
}

static int DoCodeExecution(Calc *processor, char *buf_ptr, int index, Elem_t *ram) {

    assert(processor);
    assert(buf_ptr);

    char *buf = buf_ptr;
    Elem_t com_num = 0;

    #define DEF_CMD(name, code, args, ...)      \
        case (code): {                          \
            __VA_ARGS__                         \
            break;                              \
        }                                       \

    do {
        com_num = *((Elem_t *) buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf(RED "Incorrect command for in <ExecuteProgram> " output_id " %d\n" END_OF_COLOR, com_num, index - 1);
                return ERROR;
                break;
            }
        }
    } while (com_num != CMD_HLT);

    #undef DEF_CMD

    return SUCCESS;
}