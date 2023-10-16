#include <stdio.h>
#include "assembler.h"
#include "calculator_values.h"
#include <assert.h>
#include "func.h"
#include <string.h>
#include "Stack/stack.h"
#include "ctype.h"

enum Result assembler(const char *file) {

    assert(file);
    FILE *fn = fileopen(file, READ);

    Stack buf = {};
    STACK_CTOR(buf);

    const int VERSION = 4;

    FILE *byte_code_file = fileopen(byte_code, WRITE);

    StackPush(&buf, MY_SIGN);
    StackPush(&buf, VERSION);

    #define DEF_CMD(name, code, args, type, ...)                                \
        if (strcasecmp(str, #name) == 0) {                                      \
            if (args) {                                                         \
                Elem_t num = 0;                                                 \
                int result = fscanf(fn, output_id, &num);                       \
                if ((result == 1) && (type == NUMBER)) {                        \
                    StackPush(&buf, code);                                      \
                    StackPush(&buf, num);                                       \
                }                                                               \
                else if (result == -1 && type == STRING) {                      \
                    StackPush(&buf, code);                                      \
                    fscanf(fn, "%s", str);                                      \
                    int index = RegIndex(str);                                  \
                    if (index == -1) {                                          \
                        printf("\033[031mIncorrect register name\n\033[0m");    \
                        return ERROR;                                           \
                    }                                                           \
                    StackPush(&buf, index);                                     \
                }                                                               \
            }                                                                   \
            else {                                                              \
                StackPush(&buf, code);                                          \
            }                                                                   \
        }                                                                       \

    char str[MAX_STRLEN] = "";
    while (fscanf(fn, "%s", str) == 1) {
        #include "commands.h"
    }
    #undef DEF_CMD
    fwrite((int *) buf.data, sizeof (Elem_t), buf.size, byte_code_file);
    fileclose(byte_code_file);
    fileclose(fn);
    StackDtor(&buf);

    return SUCCESS;
}

int RegIndex(char *namereg) {

    assert(namereg);

    for (size_t i = 0; i < NUM_OF_REGS; i++) {
        if (strcmp(namereg, regs[i]) == 0)
            return i;
    }
    return -1;
}

void CheckArgsType(const int type, FILE *input, FILE *output) {

    assert(input);
    assert(output);

    Elem_t n = 0;
    int result = fscanf(input, output_id, &n);
    if (result == 1 && type == NUMBER) {
        return NUMBER;
    }
    if (result == -1 && type = STRING) {

    }
}