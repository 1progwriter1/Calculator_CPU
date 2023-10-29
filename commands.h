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


DEF_CMD (SUB, 0, NO_ARGS,           \
    VERIFY                          \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n2 - n1);)             \

DEF_CMD (DIV, 1, NO_ARGS,           \
    VERIFY;                         \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n2 / n1 * MUL_PRES);)  \

DEF_CMD (OUT, 2, NO_ARGS,                                                           \
    VERIFY                                                                          \
    Elem_t n = 0;                                                                   \
    POP_NUM(n);                                                                     \
    printf("\033[36mAnswer = \033[0m\033[33m%lg\033[0m\n", (double) n / MUL_PRES);) \

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
    printf("\033[35mEnter a number: \033[0m");                      \
    Elem_t num = 0;                                                 \
    INPUT(num)                                                      \
    PUSH_NUM(num * MUL_PRES);)                                      \

DEF_CMD (PUSH, 10, NUMBER + STRING + RAM,           \
    VERIFY                                          \
    Elem_t type = 0;                                \
    type = *GET_ELEM(++);                           \
    if (type == NUMBER) {                           \
        Elem_t num = 0;                             \
        num = *GET_ELEM(++);                        \
        PUSH_NUM(num * MUL_PRES);                   \
    }                                               \
    else if (type == STRING) {                      \
        Elem_t ind = 0;                             \
        ind = *GET_ELEM(++);                        \
        Elem_t n = 0;                               \
        POP_NUM(n);                                 \
        calcdata->reg[ind] = n;                     \
    }                                               \
    else {                                          \
        Elem_t ind = 0;                             \
        ind = *GET_ELEM(++);                        \
        if (type == STRING +RAM)                    \
            PUSH_NUM(ram[calcdata->reg[ind]]);      \
        else                                        \
            PUSH_NUM(ram[ind]);                     \
    }                                               \
    break;)                                         \

DEF_CMD (POP, 11, STRING + RAM,         \
    VERIFY                              \
    Elem_t type = 0;                    \
    type = *GET_ELEM(++);               \
    if (type == STRING) {               \
        Elem_t n = 0;                   \
        Elem_t ind = 0;                 \
        ind = *GET_ELEM(++);            \
        n = calcdata->reg[ind];         \
        PUSH_NUM(n);                    \
    }                                   \
    else  {                             \
        Elem_t n = 0;                   \
        Elem_t ind = 0;                 \
        ind = *GET_ELEM(++);            \
        POP_NUM(n);                     \
        if (type == STRING + RAM)        \
            ram[calcdata->reg[ind] / MUL_PRES] = n;\
        else                            \
            ram[ind] = n;               \
    } break;)                           \

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
    }                                            \
    break;)                                      \

#include "jump.h"

#undef MAKE_COND_JUMP

DEF_CMD (JMP, 17, STRING,                       \
    VERIFY                                      \
    index = *(int *)GET_ELEM(++);               \
    break;)                                     \

DEF_CMD (JM, 18, STRING,                         \
    VERIFY                                       \
    if (DayNumber() == MONDAY) {                 \
        index = *(int *)GET_ELEM(++);            \
    }                                            \
    break;)

DEF_CMD(CALL, 19, STRING,                   \
    PUSH_ADR(index + 1);                    \
    index = *(int *)GET_ELEM(++);           \
    break;)

DEF_CMD(RET, 20, NO_ARGS, \
    POP_ADR(index);       \
    break;)               \

DEF_CMD(RAMOUT, 21, NO_ARGS,                            \
    for (size_t i = 0; i < (size_t) RAM_SIZE; i++) {    \
        if (ram[i] == 0)                                \
            printf("..");                               \
        else                                            \
            printf("**");                               \
        if ((i + 1) % (size_t) sqrt (RAM_SIZE) == 0)    \
            printf("\n");                               \
    }                                                   \
    break;)