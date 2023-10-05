#include <stdio.h>
#include "Stack/stack.h"
#include <assert.h>
#include "calculate.h"
#include <math.h>

const int PRECISION = 4;
const int MUL_PRES = pow(10, PRECISION);
const double PI = 3.14159265;

static void push(Stack *stk, int num);
static void sub(Stack *stk);
static void div(Stack *stk);
static void out(Stack *stk);
static void mul(Stack *stk);
static void sqrt(Stack *stk);
static void sinus(Stack *stk);
static void cosinus(Stack *stk);
static void in(Stack *stk);
static void add(Stack *stk);

void Calculate(Stack *example, FILE *fn) {

    assert(example);
    assert(fn);

    int com_num = 0;
    int escape = 0;
    while (fscanf(fn, "%d", &com_num) != -1 && !escape) {
        switch (com_num) {
            case 10: {
                int num = 0;
                if (fscanf(fn, "%d", &num) != 1) {
                    printf("Incorrect argument for push");
                    break;
                }
                push(example, num);
                break;
            }
            case 0: {
                sub(example);
                break;
            }
            case 1: {
                div(example);
                break;
            }
            case 2: {
                out(example);
                break;
            }
            case 3: {
                escape = 1;
                break;
            }
            case 4: {
                add(example);
                break;
            }
            case 5: {
                mul(example);
                break;
            }
            case 6: {
                sqrt(example);
                break;
            }
            case 7: {
                sinus(example);
                break;
            }
            case 8: {
                cosinus(example);
                break;
            }
            case 9: {
                in(example);
                break;
            }

            default: {
                printf("Incorrect command\n");
                break;
            }
        }
    }
}

static void push(Stack *stk, int num) {

    assert(stk);

    StackPush(stk, num * MUL_PRES);
}

static void sub(Stack *stk) {
    assert(stk);
    printf("sub\n)");
    int n1 = 0, n2 = 0;
    StackPop(stk, &n1);
    StackPop(stk, &n2);
    StackPush(stk, (n2 - n1));
}

static void div(Stack *stk) {
    assert(stk);
    int n1 = 0, n2 = 0;
    StackPop(stk, &n1);
    StackPop(stk, &n2);
    StackPush(stk, (n2 / n1) * MUL_PRES);
}

static void out(Stack *stk) {
    assert(stk);
    int n1 = 0;
    StackPop(stk, &n1);
    printf("Answer = %lg", (double) n1 / MUL_PRES);
}

static void mul(Stack *stk) {
    assert(stk);
    int n1 = 0, n2 = 0;
    StackPop(stk, &n1);
    StackPop(stk, &n2);
    StackPush(stk, n2 * n1 / MUL_PRES);
}

static void sqrt(Stack *stk) {
    assert(stk);
    int n1 = 0;
    StackPop(stk, &n1);
    StackPush(stk, (int) (sqrt(n1 / MUL_PRES) * MUL_PRES));
}

static void sinus(Stack *stk) {
    assert(stk);
    int n1 = 0;
    StackPop(stk, &n1);
    StackPush(stk, (int) (sin((double) n1 / MUL_PRES * PI / 180) * MUL_PRES));
}

static void cosinus(Stack *stk) {
    assert(stk);
    int n1 = 0;
    StackPop(stk, &n1);
    StackPush(stk, (int) (cos((double) n1 / MUL_PRES * PI / 180) * MUL_PRES));
}

static void in(Stack *stk) {
    assert(stk);
    printf("Enter a number: ");
    int num = 0;
    scanf("%d", &num);
    StackPush(stk, num * MUL_PRES);
}

static void add(Stack *stk) {
    assert(stk);
    int n1 = 0, n2 = 0;
    StackPop(stk, &n1);
    StackPop(stk, &n2);
    StackPush(stk, n2 + n1);
}
