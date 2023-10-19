#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define input_file "expression.txt"
#define byte_code "byte_code.bin"
#define disassem_file "dis_file.txt"

#include "Stack/stack_values.h"

const int REG_LEN = 3;
const int MAX_STRLEN = 10;
const int MY_SIGN = 'V' + 'L' + 'I';
const int ARGS = 1;
const int NO_ARGS = 0;
const int NUMBER = 1;
const int STRING = 0;
const int MONDAY = 1;

const int REGNAME_LEN = 4;
const int NUM_OF_REGS = 4;
const char regs[NUM_OF_REGS][REGNAME_LEN] = {"rax", "rbx", "rcx", "rdx"};

const int NUM_OF_COMMANDS = 13;
const int COMMAND_LEN = 5;

const char commands[NUM_OF_COMMANDS][COMMAND_LEN] = {"sub", "div", "out", "hlt", "add", "mul", "sqrt", "sin", "cos", "in", "push", "pop", "push"};

#define DEF_CMD(name, code, ...) CMD_##name = code,

enum Commands {
    #include "commands.h"
};

#undef DEF_CMD

struct Calc {
    Stack data;
    Elem_t *reg;
};

#endif