#define VERIFY CalcVerify(calcdata)

#define PUSH_NUM(num) StackPush(&calcdata->data, num)

#define POP_NUM(num) StackPop(&calcdata->data, &num)

DEF_CMD (SUB, 0, NO_ARGS, NO_ARGS,  \
    if (!VERIFY) return ERROR;      \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n2 - n1);)             \

DEF_CMD (DIV, 1, NO_ARGS, NO_ARGS,  \
    if (!VERIFY) return ERROR;      \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n2 / n1 * MUL_PRES);)  \

DEF_CMD (OUT, 2, 0, 0,                                                              \
    if (!VERIFY) return ERROR;                                                      \
    Elem_t n = 0;                                                                   \
    POP_NUM(n);                                                                     \
    printf("\033[36mAnswer = \033[0m\033[33m%lg\033[0m\n", (double) n / MUL_PRES);) \

DEF_CMD (HLT, 3, NO_ARGS, NO_ARGS,  \
    if (!VERIFY) return ERROR;)     \

DEF_CMD (ADD, 4, NO_ARGS, NO_ARGS,  \
    if (!VERIFY) return ERROR;      \
    Elem_t n1 = 0;                  \
    Elem_t n2 = 0;                  \
    POP_NUM(n1);                    \
    POP_NUM(n2);                    \
    PUSH_NUM(n1 + n2);)             \

DEF_CMD (MUL, 5, NO_ARGS, NO_ARGS,   \
    if (!VERIFY) return ERROR;       \
    Elem_t n1 = 0;                   \
    Elem_t n2 = 0;                   \
    POP_NUM(n1);                     \
    POP_NUM(n2);                     \
    PUSH_NUM(n2 * n1 / MUL_PRES);)   \

DEF_CMD (SQRT, 6, NO_ARGS, NO_ARGS,                     \
    if (!VERIFY) return ERROR;                          \
    Elem_t n = 0;                                       \
    POP_NUM(n);                                         \
    PUSH_NUM((int) (sqrt (n / MUL_PRES) * MUL_PRES));)  \

DEF_CMD (SIN, 7, NO_ARGS, NO_ARGS,                                          \
    if (!VERIFY) return ERROR;                                              \
    Elem_t n = 0;                                                           \
    POP_NUM(n);                                                             \
    PUSH_NUM((int) (sin((double) n / MUL_PRES * PI / 180) * MUL_PRES));)    \

DEF_CMD (COS, 8, NO_ARGS, NO_ARGS,                                          \
    if (!VERIFY) return ERROR;                                              \
    Elem_t n = 0;                                                           \
    POP_NUM(n);                                                             \
    PUSH_NUM((int) (cos((double) n / MUL_PRES * PI / 180) * MUL_PRES));)

DEF_CMD (IN, 9, NO_ARGS, NO_ARGS,                                   \
    if (!VERIFY) return ERROR;                                      \
    printf("\033[35mEnter a number: \033[0m");                      \
    Elem_t num = 0;                                                 \
    int correct = 0;                                                \
    do {                                                            \
        correct = 1;                                                \
        if (scanf(output_id, &num) != 1) {                          \
            printf("\033[31mIncorrect input. Try again\033[0m\n");  \
            clear();                                                \
            correct = 0;                                            \
        }                                                           \
    } while (!correct);                                             \
    PUSH_NUM(num * MUL_PRES);)                                      \

DEF_CMD (PUSH, 10, ARGS, NUMBER,        \
    if (!VERIFY) return ERROR;          \
    Elem_t num = 0;                     \
    num = *((Elem_t *) buf + index++);  \
    num *= MUL_PRES;                    \
    PUSH_NUM(num);                      \
    break;)                             \

DEF_CMD (POP, 11, ARGS, STRING, \
    if (!VERIFY) return ERROR;  \
    Elem_t n = 0;               \
    n = calcdata->reg[index];   \
    PUSH_NUM(n);)               \

DEF_CMD (PUSH_R, 12, ARGS, STRING,\
    if (!VERIFY) return ERROR;    \
    Elem_t n = 0;                 \
    POP_NUM(n);                   \
    calcdata->reg[index] = n;)    \