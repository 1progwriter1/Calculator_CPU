DEF_CMD (SUB, 0, NO_ARGS,           \
    VERIFY                          \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n2 - n1);)             \

DEF_CMD (DIV, 1, NO_ARGS,                                   \
    VERIFY;                                                 \
    Elem_t n1 = 0;                                          \
    Elem_t n2 = 0;                                          \
    POP_NUM(n1);                                            \
    POP_NUM(n2);                                            \
    if (n1 == 0) {                                          \
        printf("\033[" RED "mDivision by zero\033[0m\n");   \
        return ERROR;                                       \
    }                                                       \
    PUSH_NUM(n2 / n1 * MUL_PRES);)                          \

DEF_CMD (OUT, 2, NO_ARGS,                                                                           \
    VERIFY                                                                                          \
    Elem_t n = 0;                                                                                   \
    POP_NUM(n);                                                                                     \
    printf("\033[" BLUE "mAnswer = \033[0m\033[" YELLOW "m%lg\033[0m\n", (double) n / MUL_PRES);)   \

DEF_CMD (HLT, 3, NO_ARGS,           \
    VERIFY)                         \

DEF_CMD (ADD, 4, NO_ARGS,           \
    VERIFY                          \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n1 + n2);)             \

DEF_CMD (MUL, 5, NO_ARGS,            \
    VERIFY                           \
    Elem_t n1 = 0;                   \
    Elem_t n2 = 0;                   \
    POP_NUM(n1);                     \
    POP_NUM(n2);                     \
    PUSH_NUM(n2 * n1 / MUL_PRES);)   \

DEF_CMD (SQRT, 6, NO_ARGS,                              \
    VERIFY                                              \
    Elem_t n = 0;                                       \
    POP_NUM(n);                                         \
    PUSH_NUM((int) (sqrt (n / MUL_PRES) * MUL_PRES));)  \

DEF_CMD (SIN, 7, NO_ARGS,                                                   \
    VERIFY                                                                  \
    Elem_t n = 0;                                                           \
    POP_NUM(n);                                                             \
    PUSH_NUM((int) (sin((double) n / MUL_PRES * PI / 180) * MUL_PRES));)    \

DEF_CMD (COS, 8, NO_ARGS,                                                   \
    VERIFY                                                                  \
    Elem_t n = 0;                                                           \
    POP_NUM(n);                                                             \
    PUSH_NUM((int) (cos((double) n / MUL_PRES * PI / 180) * MUL_PRES));)    \

DEF_CMD (IN, 9, NO_ARGS,                                            \
    VERIFY                                                          \
    printf("\033[" VIOLET "mEnter a number: \033[0m");              \
    Elem_t num = 0;                                                 \
    INPUT(num)                                                      \
    PUSH_NUM(num * MUL_PRES);)                                      \

DEF_CMD (PUSH, 10, NUMBER + STRING + RAM,           \
    VERIFY                                          \
    Elem_t type = 0;                                \
    Elem_t num = 0;                                 \
    Elem_t n = 0;                                   \
    type = *GET_ELEM(++);                           \
    n = *GET_ELEM(++);                              \
    if (type == NUMBER) {                           \
        num = n * MUL_PRES;                         \
    }                                               \
    else if (type == STRING) {                      \
        POP_NUM(num);                               \
        calcdata->reg[n] = num;                     \
        break;                                      \
    }                                               \
    else {                                          \
        if (type == (STRING | RAM))                 \
            num = ram[calcdata->reg[n]];            \
        else                                        \
            num = ram[n];                           \
    }                                               \
    PUSH_NUM(num);)                                 \

DEF_CMD (POP, 11, STRING + RAM,         \
    VERIFY                              \
    Elem_t type = 0;                    \
    Elem_t n = 0;                       \
    Elem_t num = 0;                     \
    type = *GET_ELEM(++);               \
    n = *GET_ELEM(++);                  \
    if (type == STRING) {               \
        PUSH_NUM(calcdata->reg[n]);     \
    }                                   \
    else  {                             \
        POP_NUM(num);                   \
        if (type == STRING + RAM)       \
            ram[calcdata->reg[n] / MUL_PRES] = num;\
        else                            \
            ram[n] = num;               \
                                        \
    })                                  \

#define MAKE_COND_JUMP(name, code, case)         \
DEF_CMD (name, code, STRING,                     \
    VERIFY                                       \
    Elem_t n1 = 0;                               \
    Elem_t n2 = 0;                               \
    POP_NUM(n1);                                 \
    POP_NUM(n2);                                 \
    if (n1 case n2) {                            \
        index = *(int *)GET_ELEM(++);            \
    }                                            \
    else {                                       \
        index++;                                 \
    })                                           \

#include "jump.h"

#undef MAKE_COND_JUMP

DEF_CMD (JMP, 17, STRING,                       \
    VERIFY                                      \
    index = *(int *)GET_ELEM(++);)              \

DEF_CMD (JM, 18, STRING,                         \
    VERIFY                                       \
    if (DayNumber() == MONDAY) {                 \
        index = *(int *)GET_ELEM(++);            \
    })                                           \

DEF_CMD(CALL, 19, STRING,                   \
    PUSH_ADR(index + 1);                    \
    index = *(int *)GET_ELEM(++);)          \

DEF_CMD(RET, 20, NO_ARGS, \
    POP_ADR(index);)      \

DEF_CMD(RAMOUT, 21, NO_ARGS,                            \
    for (size_t i = 0; i < (size_t) RAM_SIZE; i++) {    \
        if (ram[i] == 0)                                \
            printf("..");                               \
        else                                            \
            printf("**");                               \
        if ((i + 1) % (size_t) sqrt (RAM_SIZE) == 0)    \
            printf("\n");                               \
    })                                                  \

DEF_CMD(OUTC, 22, NO_ARGS,              \
    VERIFY;                             \
    Elem_t sym = 0;                     \
    POP_NUM(sym);                       \
    printf("%c", sym / MUL_PRES);)