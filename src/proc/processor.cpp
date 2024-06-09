#include <stdio.h>
#include "../stack/stack.h"
#include "../stack/stack_values.h"
#include <assert.h>
#include "processor.h"
#include <math.h>
#include "../lib/func.h"
#include "../configuration.h"
#include "../lib/calculator_values.h"
#include <string.h>

#define VERIFY                          \
    if (!CPUVerify(data->processor)) {  \
        return ERROR;                   \
    }                                   \

#define PUSH_NUM(num) StackPush(&data->processor->data, num)

#define POP_NUM(num) StackPop(&data->processor->data, &num)

#define PUSH_ADR(num) StackPush(&data->processor->addresses, (Elem_t) num);

#define POP_ADR(num) StackPop(&data->processor->addresses, (Elem_t *) &num);

#define INPUT(num)                                                  \
    int correct = 0;                                                \
    do {                                                            \
        correct = 1;                                                \
        if (scanf("%lg", &num) != 1) {                              \
            printf(RED "Incorrect input. Try again:\n " END_OF_COLOR);\
            clear();                                                \
            correct = 0;                                            \
        }                                                           \
    } while (!correct);                                             \

#define GET_ELEM(operation) ((Elem_t *) data->buf + index operation)

static const int MUL_PRES = (int) pow(10, PRECISION);
const double PI = 3.14159265;

static void clear();
static int PrepareForExecution(ProcData *data);
static int DoCodeExecution(ProcData *data, Elem_t *ram);
static int CPUVerify(CPU *calcdata);
static void DumpCPU(CPU *calcdata, unsigned int error, int correct_reg);
static int GetProcData(ProcData *data, const char *byte_code_file, CPU *processor);

int ExecuteProgram(CPU *processor, const char *byte_code_file) {

    assert(processor);
    assert(byte_code_file);

    ProcData data = {};
    GetProcData(&data, byte_code_file, processor);

    if (PrepareForExecution(&data) != SUCCESS)
        return ERROR;

    printf("Processor (version %d)\n" "Executable file: %s\n\n" GREEN "File verified" END_OF_COLOR "\n\n", VERSION, byte_code_file);

    Elem_t ram[RAM_SIZE] = {};
    if (DoCodeExecution(&data, ram) != SUCCESS) {
        free(data.buf);
        return ERROR;
    }

    free(data.buf);

    return SUCCESS;
}

int CPUCtor(CPU *processor) {

    if (!processor)
        return NULL_POINTER;

    STACK_CTOR(processor->data);
    processor->regs = (Elem_t *) calloc (NUM_OF_REGS, sizeof (Elem_t));
    if (!processor->regs)
        return NO_MEMORY;
    STACK_CTOR(processor->addresses);


    return SUCCESS;
}

int CPUDtor(CPU *processor) {

    if (!processor)
        return NULL_POINTER;
    StackDtor(&processor->data);
    free(processor->regs);
    StackDtor(&processor->addresses);

    return SUCCESS;
}

int CPUVerify(CPU *processor) {

    assert(processor);

    unsigned int error = 0;
    error = StackVerify(&processor->data);
    int correct_reg = 1;
    if (!processor->regs) {
        correct_reg = 0;
    }
    if (error != 0 || !correct_reg) {
        DumpCPU(processor, error, correct_reg);
        return 0;
    }
    return 1;
}

void DumpCPU(CPU *processor, unsigned int error, int correct_reg) {

    if (!processor)
        printf(RED "NULL calcdata" END_OF_COLOR "\n");

    if (!correct_reg) {
        printf("Register: {");
        for (size_t i = 0; i < NUM_OF_REGS; i++)
            printf(output_id " ", processor->regs[i]);
        printf("\b}\n");
    }
    else {
        printf(RED "NULL reg" END_OF_COLOR "\n");
    }

    dump(&processor->data, error);
}

void clear() {
    while (getchar() != '\n')
        ;
}

static int PrepareForExecution(ProcData *data) {

    assert(data);
    assert(data->filename);
    assert(data->processor);

    FILE *byte_code_file = fileopen(data->filename, READ);
    if (!byte_code_file)
        return ERROR;

    if (!data->buf) {
        fileclose(byte_code_file);
        return NO_MEMORY;
    }

    fread(data->buf, sizeof (char), data->len_of_file, byte_code_file);
    fileclose(byte_code_file);

    Header filedata = *((Header *) data->buf);
    if (FileVerify(&filedata) != SUCCESS) {
        free(data->buf);
        return ERROR;
    }

    return SUCCESS;
}

static int DoCodeExecution(ProcData *data, Elem_t *ram) {

    assert(data);
    assert(data->processor);
    assert(ram);
    assert(data->buf);

    char *buf = data->buf;
    Elem_t com_num = 0;
    int index = START_OF_PROG;

    #define DEF_CMD(name, code, args, ...)      \
        case (code): {                          \
            __VA_ARGS__                         \
            break;                              \
        }                                       \

    do {
        com_num = *((Elem_t *) buf + index++);
            switch (com_num) {
            #include "../lib/commands.h"
            default: {
                printf(RED "Incorrect command in <ExecuteProgram> " output_id "\n" END_OF_COLOR, com_num);
                return ERROR;
                break;
            }
        }
    } while (com_num != CMD_HLT);

    #undef DEF_CMD

    return SUCCESS;
}

static int GetProcData(ProcData *data, const char *byte_code_file, CPU *processor) {

    assert(data);
    assert(byte_code_file);
    assert(processor);

    data->processor = processor;
    data->len_of_file = filelen(byte_code_file);
    data->filename = byte_code_file;
    data->buf = (char *) calloc (data->len_of_file + 1, sizeof (char));

    return SUCCESS;
}