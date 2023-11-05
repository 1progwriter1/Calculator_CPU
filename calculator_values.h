#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define byte_code "byte_code.bin"
#define disassem_file "dis_file.txt"

#define RED "\033[31m"
#define BLUE "\033[36m"
#define VIOLET "\033[35m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define END_OF_COLOR "\033[0m"

#include "Stack/stack_values.h"

const int REG_LEN = 3;
const int MAX_STRLEN = 20;
const int BYTE_CODE_SIGNATURE = 'V' + 'L' + 'I';
const int ARGS = 1;
const char NO_ARGS = 0;
const char NUMBER = 1;
const char STRING = 2;
const int RAM_ACCESS = 4;
const int MONDAY = 1;
const int LABELS_NUM = 10;
const int VERSION = 8;
const int INDEX_OF_CODE_LEN = 0;

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