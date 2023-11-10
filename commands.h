DEF_CMD (SUB, 0, NO_ARGS,
    VERIFY
    Elem_t num1 = 0;
    Elem_t num2 = 0;
    POP_NUM(num1);
    POP_NUM(num2);
    PUSH_NUM(num2 - num1);
    )

DEF_CMD (DIV, 1, NO_ARGS,
    VERIFY;
    Elem_t num1 = 0;
    Elem_t num2 = 0;
    POP_NUM(num1);
    POP_NUM(num2);
    if (num1 == 0) {
        printf(RED "Division by zero" END_OF_COLOR "\n");
        return ERROR;
    }
    PUSH_NUM(num2 / num1 * MUL_PRES);
    )

DEF_CMD (OUT, 2, NO_ARGS,
    VERIFY
    Elem_t num = 0;
    POP_NUM(num);
    printf(BLUE "Answer = " END_OF_COLOR YELLOW "%lg" END_OF_COLOR "\n", (double) num / MUL_PRES);
    )

DEF_CMD (HLT, 3, NO_ARGS,
    VERIFY
    )

DEF_CMD (ADD, 4, NO_ARGS,
    VERIFY
    Elem_t num1 = 0;
    Elem_t num2 = 0;
    POP_NUM(num1);
    POP_NUM(num2);
    PUSH_NUM(num1 + num2);
    )

DEF_CMD (MUL, 5, NO_ARGS,
    VERIFY
    Elem_t num1 = 0;
    Elem_t num2 = 0;
    POP_NUM(num1);
    POP_NUM(num2);
    PUSH_NUM(num2 * num1 / MUL_PRES);
    )

DEF_CMD (SQRT, 6, NO_ARGS,
    VERIFY
    Elem_t num = 0;
    POP_NUM(num);
    PUSH_NUM((int) (sqrt (num / MUL_PRES) * MUL_PRES));
    )

DEF_CMD (SIN, 7, NO_ARGS,
    VERIFY
    Elem_t num = 0;
    POP_NUM(num);
    PUSH_NUM((int) (sin((double) num / MUL_PRES * PI / 180) * MUL_PRES));
    )

DEF_CMD (COS, 8, NO_ARGS,
    VERIFY
    Elem_t num = 0;
    POP_NUM(num);
    PUSH_NUM((int) (cos((double) num / MUL_PRES * PI / 180) * MUL_PRES));
    )

DEF_CMD (IN, 9, NO_ARGS,
    VERIFY
    printf(VIOLET "Enter the number: " END_OF_COLOR);
    Elem_t num = 0;
    INPUT(num)
    PUSH_NUM(num * MUL_PRES);
    )

DEF_CMD (PUSH, 10, (NUMBER | STRING | RAM_ACCESS),
    VERIFY
    Elem_t type = 0;
    Elem_t num = 0;
    Elem_t arg = 0;
    type = *GET_ELEM(++);
    arg = *GET_ELEM(++);
    if (type == NUMBER) {
        num = arg * MUL_PRES;
    }
    else if (type == STRING) {
        num = data->processor->regs[arg];
    }
    else {
        if (type == (STRING | RAM_ACCESS))
            num = ram[data->processor->regs[arg]];
        else
            num = ram[arg];
    }
    PUSH_NUM(num);
    )

DEF_CMD (POP, 11, (STRING | RAM_ACCESS),
    VERIFY
    Elem_t type = 0;
    Elem_t arg = 0;
    Elem_t num = 0;
    type = *GET_ELEM(++);
    arg = *GET_ELEM(++);
    if (type == STRING) {
        POP_NUM(num);
        data->processor->regs[arg] = num;
    }
    else  {
        POP_NUM(num);
        if (type == (STRING | RAM_ACCESS))
            ram[data->processor->regs[arg] / MUL_PRES] = num;
        else
            ram[arg] = num;

    }
    )

#define MAKE_COND_JUMP(name, code, case)         \
DEF_CMD (name, code, STRING,                     \
    VERIFY                                       \
    Elem_t num1 = 0;                             \
    Elem_t num2 = 0;                             \
    POP_NUM(num1);                               \
    POP_NUM(num2);                               \
    if (num1 case num2) {                        \
        index = *(int *)GET_ELEM(++);            \
    }                                            \
    else {                                       \
        index++;                                 \
    }                                            \
    )                                            \

#include "jump.h"

#undef MAKE_COND_JUMP

DEF_CMD (JMP, 17, STRING,
    VERIFY
    index = *(int *)GET_ELEM(++);
    )

DEF_CMD (JM, 18, STRING,
    VERIFY
    if (DayNumber() == MONDAY) {
        index = *(int *)GET_ELEM(++);
    }
    )

DEF_CMD(CALL, 19, STRING,
    PUSH_ADR(index + 1);
    index = *(int *)GET_ELEM(++);
    )

DEF_CMD(RET, 20, NO_ARGS,
    POP_ADR(index);
    )

DEF_CMD(RAMOUT, 21, NO_ARGS,
    for (size_t i = 0; i < (size_t) RAM_SIZE; i++) {
        if (ram[i] == 0)
            printf("..");
        else
            printf("**");
        if ((i + 1) % (size_t) sqrt (RAM_SIZE) == 0)
            printf("\n");
    }
    )

DEF_CMD(OUTC, 22, NO_ARGS,
    VERIFY;
    Elem_t sym = 0;
    POP_NUM(sym);
    printf("%c", (char) (sym / MUL_PRES));
    )