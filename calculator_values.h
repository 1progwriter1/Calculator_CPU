#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define byte_code "byte_code.bin"
#define disassem_file "dis_file.txt"

#include "Stack/stack_values.h"

const int REG_LEN = 3;
const int MAX_STRLEN = 20;
const int MY_SIGN = 'V' + 'L' + 'I';
const int ARGS = 1;
const char NO_ARGS = 0;
const char NUMBER = 1;
const char STRING = 2;
const int RAM = 4;
const int MONDAY = 1;
const int LABELS_NUM = 10;

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
    Stack addresses;
};

struct maindata {
    int assembler;
    int processor;
    int disassembler;
    int asm_file_in;
    int asm_file_out;
    int proc_file;
    int dis_file_in;
    int dis_file_out;
};
#endif