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

const int PRECISION = 3;
const int MUL_PRES = (int) pow(10, PRECISION);
const double PI = 3.14159265;

static void push(Calc *calcdata, Elem_t num);
static void sub(Calc *calcdata);
static void div(Calc *calcdata);
static void out(Calc *calcdata);
static void mul(Calc *calcdata);
static void sqrt(Calc *calcdata);
static void sinus(Calc *calcdata);
static void cosinus(Calc *calcdata);
static void in(Calc *calcdata);
static void add(Calc *calcdata);
static void push_r(Calc *calcdata, int index);
static void pop_r(Calc *calcdata, int index);
static void clear();

enum Result Calculate(Calc *calcdata, const char *file) {

    assert(calcdata);
    assert(file);

    const int VERSION = 4;

    long long int len_of_file = filelen(file);
    FILE *fn = fileopen(file, READ);

    char *buf = (char *) calloc (len_of_file + 1, sizeof (char));
    fread(buf, sizeof (char), len_of_file, fn);

    int index = 0;
    Elem_t sign = *((Elem_t *) buf + index++);
    Elem_t file_version = *((Elem_t *) buf + index++);
    if (!FileVerify(sign, VERSION, file_version))
        return ERROR;

    Elem_t com_num = 0;

    #define DEF_CMD(name, code, args, type, ...)\
        case (code): {                          \
            __VA_ARGS__                         \
            break;                              \
        }                                       \

    do {
        com_num = *((Elem_t *) buf + index++);
        PrintStack(&calcdata->data);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf("\033[31mIncorrect command\n\033[0m");
                break;
            }
        }
    } while (com_num != CMD_HLT);

    #undef DEF_CMD

    fileclose(fn);

    return SUCCESS;
}

enum Result CalcCtor(Calc *calcdata) {

    if (!calcdata)
        return NULL_POINTER;

    STACK_CTOR(calcdata->data);
    calcdata->reg = (Elem_t *) calloc (NUM_OF_REGS, sizeof (Elem_t));

    return SUCCESS;
}

enum Result CalcDtor(Calc *calcdata) {

    if (!calcdata)
        return NULL_POINTER;
    StackDtor(&calcdata->data);
    free(calcdata->reg);

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

static void push(Calc *calcdata, Elem_t num) {

    CalcVerify(calcdata);

    StackPush(&calcdata->data, num * MUL_PRES);
}

static void sub(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0, n2 = 0;
    StackPop(&calcdata->data, &n1);
    StackPop(&calcdata->data, &n2);
    StackPush(&calcdata->data, (n2 - n1));
}

static void div(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0, n2 = 0;
    StackPop(&calcdata->data, &n1);
    StackPop(&calcdata->data, &n2);
    StackPush(&calcdata->data, (n2 / n1) * MUL_PRES);
}

static void out(Calc *calcdata) {

    assert(calcdata);
    Elem_t n1 = 0;
    StackPop(&calcdata->data, &n1);
    printf("\033[36mAnswer = \033[0m\033[33m%lg\033[0m\n", (double) n1 / MUL_PRES);
}

static void mul(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0, n2 = 0;
    StackPop(&calcdata->data, &n1);
    StackPop(&calcdata->data, &n2);
    StackPush(&calcdata->data, n2 * n1 / MUL_PRES);
}

static void sqrt(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0;
    StackPop(&calcdata->data, &n1);
    StackPush(&calcdata->data, (int) (sqrt(n1 / MUL_PRES) * MUL_PRES));
}

static void sinus(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0;
    StackPop(&calcdata->data, &n1);
    StackPush(&calcdata->data, (int) (sin((double) n1 / MUL_PRES * PI / 180) * MUL_PRES));
}

static void cosinus(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0;
    StackPop(&calcdata->data, &n1);
    StackPush(&calcdata->data, (int) (cos((double) n1 / MUL_PRES * PI / 180) * MUL_PRES));
}

static void in(Calc *calcdata) {

    CalcVerify(calcdata);
    printf("\033[35mEnter a number: \033[0m");
    Elem_t num = 0;
    int correct = 0;
    do {
        correct = 1;
        if (scanf(output_id, &num) != 1) {
            printf("\033[31mIncorrect input. Try again\033[0m\n");
            clear();
            correct = 0;
        }
    } while (!correct);
    StackPush(&calcdata->data, num * MUL_PRES);
}

static void add(Calc *calcdata) {

    CalcVerify(calcdata);
    Elem_t n1 = 0, n2 = 0;
    StackPop(&calcdata->data, &n1);
    StackPop(&calcdata->data, &n2);
    StackPush(&calcdata->data, n2 + n1);
}

static void push_r(Calc *calcdata, int index) {

    CalcVerify(calcdata);
    Elem_t n = 0;
    StackPop(&calcdata->data, &n);
    calcdata->reg[index] = n;
}

static void pop_r(Calc *calcdata, int index) {

    CalcVerify(calcdata);

    Elem_t n = 0;
    n = calcdata->reg[index];
    StackPush(&calcdata->data, n);
}

void clear() {
    while (getchar() != '\n')
        ;
}
