#ifndef CALC_VALUES
#define CALC_VALUES

#include "Stack/stack_values.h"

#define READ "r"
#define WRITE "w"

#define RED "\033[31m"
#define BLUE "\033[36m"
#define VIOLET "\033[35m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define END_OF_COLOR "\033[0m"

const int MAX_STRLEN = 20;
const int BYTE_CODE_SIGNATURE = 'V' + 'L' + 'I';

const int MONDAY = 1;
const int LABELS_NUM = 10;
const int VERSION = 8;
const int INDEX_OF_CODE_LEN = 0;
const int START_OF_PROG = 3;

const int NUM_OF_REGS = 4;
const int REG_LEN = 4;
const char regs[NUM_OF_REGS][REG_LEN] = {"rax", "rbx", "rcx", "rdx"};

#define DEF_CMD(name, code, ...) CMD_##name = code,

enum Commands {
    #include "commands.h"
};

#undef DEF_CMD

enum ArgType
{
    NO_ARGS     = 0,
    NUMBER      = 1 << 0,
    STRING      = 1 << 1,
    RAM_ACCESS  = 1 << 2,
    NO_ARG_TYPE = -1,
};

struct Header {
    Elem_t len_of_prog;
    Elem_t signature;
    Elem_t version;
};

struct CPU {
    Stack data;
    Elem_t *regs;
    Stack addresses;
};

#endif