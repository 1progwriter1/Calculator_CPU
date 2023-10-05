#include <stdio.h>
#include "disassembler.h"
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "Stack/stack.h"

FILE *disassembler(FILE *fn) {

    assert(fn);

    long long int len_of_file = filelen(byte_code);
    /* char *bytecode = readfile(fn, len_of_file);
    char *curr_code = bytecode; */

    FILE *disass_file = fileopen(disassem_file, WRITE);

    int com_num = 0;
    while (fscanf(fn, "%d", &com_num) != -1) {
        if (com_num == 10) {
            int num = 0;
            fscanf(fn, "%d", &num);
            fprintf(disass_file, "%s %d\n", commands[com_num], num);
        }
        else {
            fprintf(disass_file, "%s\n", commands[com_num]);
        }
    }

    return disass_file;
}
