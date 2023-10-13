#include <stdio.h>
#include "assembler.h"
#include "calculator_values.h"
#include <assert.h>
#include "func.h"
#include <string.h>
#include "Stack/stack.h"
#include "ctype.h"

enum Result translator(FILE *fn) {

    assert(fn);

    Stack buf = {};
    STACK_CTOR(buf);

    const int VERSION = 3;

    FILE *byte_code_file = fileopen(byte_code, WRITE);

    StackPush(&buf, MY_SIGN);
    StackPush(&buf, VERSION);

    char str[MAX_STRLEN] = "";
    while (fscanf(fn, "%s", str) == 1) {
        int com_num = CommandNumber(str);
        if (com_num == 10) {
            Elem_t num = 0;
            if (fscanf(fn, output_id, &num) == 1) {
                StackPush(&buf, com_num);
                StackPush(&buf, num);
            }
            else {
                fscanf(fn, "%s", str);
                StackPush(&buf, PUSH_R);
                StackPush(&buf, RegIndex(str));
            }
        }
        else if (com_num == 11) {
            fscanf(fn, "%s", str);
            StackPush(&buf, com_num);
            StackPush(&buf, RegIndex(str));
        }
        else {
            StackPush(&buf, CommandNumber(str));
        }
    }

    fwrite((int *) buf.data, sizeof (Elem_t), buf.size, byte_code_file);
    fileclose(byte_code_file);
    StackDtor(&buf);

    return SUCCESS;
}

int CommandNumber(char *line) {

    assert(line);

    for (size_t i = 0; i < NUM_OF_COMMANDS; i++) {
        if (i == NUM_OF_COMMANDS - 2) {
            if (strncmp(line, commands[i], LEN_COMMAND_PUSH) == 0)
                return i;
        }
        if (i == NUM_OF_COMMANDS - 1) {
            if (strncmp(line, commands[i], LEN_COMMAND_POP_R) == 0)
                return i;
        }
        if (strcmp(line, commands[i]) == 0)
            return i;
    }
    return -1;
}

int RegIndex(char *namereg) {

    assert(namereg);

    for (size_t i = 0; i < NUM_OF_REGS; i++) {
        if (strcmp(namereg, regs[i]) == 0)
            return i;
    }
    return -1;
}
