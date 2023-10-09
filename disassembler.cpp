#include <stdio.h>
#include "disassembler.h"
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "Stack/stack.h"

FILE *disassembler(FILE *fn) {

    assert(fn);

    const int VERSION = 2;

    if (!FileVerify(fn, MY_SIGN, VERSION))
        return NULL;

    FILE *disass_file = fileopen(disassem_file, WRITE);

    int com_num = 0;
    while (fscanf(fn, "%d", &com_num) != -1) {
        if (com_num == PUSH) {
            int num = 0;
            fscanf(fn, "%d", &num);
            fprintf(disass_file, "%s %d\n", commands[com_num], num);
        }
        else if (com_num == POP) {
            int num = 0;
            fscanf(fn, "%d", &num);
            fprintf(disass_file, "%s %s\n", commands[com_num], regs[num]);
        }
        else if (com_num == PUSH_R) {
            int num = 0;
            fscanf(fn, "%d", &num);
            fprintf(disass_file, "%s %s\n", commands[com_num], regs[num]);
        }
        else {
            fprintf(disass_file, "%s\n", commands[com_num]);
        }
    }

    return disass_file;
}
