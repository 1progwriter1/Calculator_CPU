#include <stdio.h>
#include "disassembler.h"
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "Stack/stack.h"

enum Result disassembler(const char *file) {

    assert(file);

    const int VERSION = 4;

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

    #define DEF_CMD(name, code, args, type, ...)                                        \
        case (code): {                                                                  \
            if (args) {                                                                 \
                if (type == NUMBER) {                                                   \
                    Elem_t n = 0;                                                       \
                    n = *((Elem_t *) buf + index++);                                    \
                        fprintf(disass_file, "%s " output_id "\n", commands[code], n);  \
                    }                                                                   \
                else {                                                                  \
                    Elem_t n = 0;                                                       \
                    n = *((Elem_t *) buf + index++);                                    \
                    fprintf(disass_file, "%s %s\n", commands[code], regs[n]);\
                }                                                                       \
            }                                                                           \
            else {                                                                      \
                fprintf(disass_file, "%s\n", commands[code]);                           \
            }                                                                           \
            break;                                                                      \
        }                                                                               \

    int com_num = 0;
    do {
        com_num = *((Elem_t *) buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf("\033[31mIncorrect command number\n\033[0m");
                break;
            }
        }
    } while (com_num != CMD_HLT);

    #undef DEF_CMD
    free(buf);
    fileclose(byte_code_file);
    fileclose(disass_file);

    return SUCCESS;
}
