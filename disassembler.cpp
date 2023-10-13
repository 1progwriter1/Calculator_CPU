#include <stdio.h>
#include "disassembler.h"
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "Stack/stack.h"

enum Result disassembler(const char *file) {

    assert(file);

    const int VERSION = 3;

    FILE *byte_code_file = fileopen(file, READ);
    FILE *disass_file = fileopen(disassem_file, WRITE);

    int len_of_file = filelen(file);
    char *buf = (char *) calloc (len_of_file + 1, sizeof (char));

    fread(buf, sizeof (char), len_of_file, byte_code_file);

    int index = 0;
    int sign = *((Elem_t *) buf + index++);
    int file_version = *((Elem_t *) buf + index++);
    if (!FileVerify(sign, VERSION, file_version))
        return ERROR;

    int com_num = 0;
    do {
        com_num = *((Elem_t *) buf + index++);
        if (com_num == PUSH) {
            int num = 0;
            num = *((Elem_t *) buf + index++);
            fprintf(disass_file, "%s %d\n", commands[com_num], num);
        }
        else if (com_num == POP) {
            int num = 0;
            num = *((Elem_t *) buf + index++);
            fprintf(disass_file, "%s %s\n", commands[com_num], regs[num]);
        }
        else if (com_num == PUSH_R) {
            int num = 0;
            num = *((Elem_t *) buf + index++);
            fprintf(disass_file, "%s %s\n", commands[com_num], regs[num]);
        }
        else {
            fprintf(disass_file, "%s\n", commands[com_num]);
        }
    } while (com_num != HLT);
    free(buf);

    return SUCCESS;
}
