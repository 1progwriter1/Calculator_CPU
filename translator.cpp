#include <stdio.h>
#include "translator.h"
#include "calculator_values.h"
#include <assert.h>
#include "func.h"
#include <string.h>
#include "Stack/stack.h"

const int MAX_STRLEN = 3;

FILE *translator(FILE *fn) {

    assert(fn);

    long long int len_of_file = filelen(input_file);
    char *expr = readfile(fn, len_of_file);
    char *curr_word_ptr = expr;

    FILE *byte_code_file = fileopen(byte_code, WRITE);

    for (size_t i = 0; i < len_of_file; i++) {
        if (expr[i] == '\n') {
            expr[i] = '\0';

            int com_num = CommandNumber(curr_word_ptr);
            if (com_num == 10)
                fprintf(byte_code_file, "%d %s\n", CommandNumber(curr_word_ptr), curr_word_ptr + LEN_COMMAND + 1);
            else
                fprintf(byte_code_file, "%d\n", CommandNumber(curr_word_ptr));

            if (i < len_of_file - 1)
                curr_word_ptr = expr + i + 1;
        }
    }
    free(expr);
    return byte_code_file;
}

int CommandNumber(char *line) {

    assert(line);

    for (size_t i = 0; i < NUM_OF_COMMANDS; i++) {
        if (i == NUM_OF_COMMANDS - 1) {
            if (strncmp(line, commands[i], LEN_COMMAND) == 0)
                return i;
        }
        if (strcmp(line, commands[i]) == 0)
            return i;
    }
    return -1;
}
