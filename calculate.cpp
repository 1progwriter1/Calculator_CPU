#include <stdio.h>
#include "Stack/stack.h"
#include "Stack/stack_values.h"
#include <assert.h>
#include "calculate.h"
#include <math.h>
#include "func.h"
#include "Stack/config.h"
#include "calculator_values.h"
#include <string.h>

const int PRECISION = 3;
const int MUL_PRES = pow(10, PRECISION);
const double PI = 3.14159265;

static void push(Calc *calcdata, int num);
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

enum Result Calculate(Calc *calcdata, FILE *fn) {

    assert(calcdata);
    assert(fn);

    const int VERSION = 2;

    if (!FileVerify(fn, MY_SIGN, VERSION))
        return ERROR;

    int com_num = 0;
    int escape = 0;
    while (fscanf(fn, "%d", &com_num) != -1 && !escape) {
        switch (com_num) {
            case PUSH: {
                int num = 0;
                if (fscanf(fn, "%d", &num) != 1) {
                    printf("\033[31mIncorrect argument for push\n\033[0m");
                    break;
                }
                push(calcdata, num);
                break;
            }
            case SUB: {
                sub(calcdata);
                break;
            }
            case DIV: {
                div(calcdata);
                break;
            }
            case OUT: {
                out(calcdata);
                break;
            }
            case HLT: {
                escape = 1;
                break;
            }
            case ADD: {
                add(calcdata);
                break;
            }
            case MUL: {
                mul(calcdata);
                break;
            }
            case SQRT: {
                sqrt(calcdata);
                break;
            }
            case SIN: {
                sinus(calcdata);
                break;
            }
            case COS: {
                cosinus(calcdata);
                break;
            }
            case IN: {
                in(calcdata);
                break;
            }

            case PUSH_R: {
                int index = 0;
                if (fscanf(fn, "%d", &index) != 1 || index == -1) {
                    printf("\033[31mIncorrect argument for register push\n\033[0m");
                    break;
                }
                push_r(calcdata, index);
                break;
            }

            case POP: {
                int index = 0;
                if (fscanf(fn, "%d", &index) != 1 || index == -1) {
                    printf("\033[31mIncorrect argument for register pop\n\033[0m");
                    break;
                }
                pop_r(calcdata, index);
                break;
            }

            default: {
                printf("Incorrect command\n");
                break;
            }
        }
    }
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

void CalcVerify(Calc *calcdata) {

    assert(calcdata);

    unsigned int error = 0;
    error = StackVerify(&calcdata->data);
    int correct_reg = 1;
    if (!calcdata->reg)
        correct_reg = 0;
    if (error != 0 || !correct_reg)
        DumpCalc(calcdata, error, correct_reg);

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

static void push(Calc *calcdata, int num) {

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
    printf("\033[36mAnswer = \033[0m\033[33m%lg\033[0m", (double) n1 / MUL_PRES);
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
        if (scanf("%d", &num) != 1) {
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
