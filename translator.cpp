#include <stdio.h>
#include "translator.h"
#include "calculator_values.h"
#include <assert.h>
#include "func.h"
#include <string.h>
#include "Stack/stack.h"
#include "ctype.h"

FILE *translator(FILE *fn) {

    assert(fn);

    const int VERSION = 2;

    FILE *byte_code_file = fileopen(byte_code, WRITE);
    fprintf(byte_code_file, "VLI\n");
    fprintf(byte_code_file, "%d\n", VERSION);
    char str[MAX_STRLEN] = "";
    while (fscanf(fn, "%s", str) == 1) {
        int com_num = CommandNumber(str);
        if (com_num == 10) {
            Elem_t num = 0;
            if (fscanf(fn, output_id, &num) == 1) {
                fprintf(byte_code_file, "%d %d\n", com_num, num);
            }
            else {
                fscanf(fn, "%s", str);
                fprintf(byte_code_file, "%d %d\n", PUSH_R, RegIndex(str));
            }
        }
        else if (com_num == 11) {
            fscanf(fn, "%s", str);
            fprintf(byte_code_file, "%d %d\n", com_num, RegIndex(str));
        }
        else {
            fprintf(byte_code_file, "%d\n", CommandNumber(str));
        }
    }
    return byte_code_file;
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
