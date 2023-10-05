#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define input_file "expression.txt"
#define byte_code "byte_code.txt"
#define disassem_file "dis_file.txt"

const int LEN_COMMAND = 4;

const int NUM_OF_COMMANDS = 11;
const char commands[11][5] = {"sub", "div", "out", "hlt", "add", "mul", "sqrt", "sin", "cos", "in", "push"};

#endif
