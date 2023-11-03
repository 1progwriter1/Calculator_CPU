#include <stdio.h>
#include "assembler.h"
#include "calculator_values.h"
#include <assert.h>
#include "func.h"
#include "my_vector.h"
#include <string.h>
#include "Stack/stack.h"
#include "ctype.h"
#include "labels.h"

const int VEC_SIZE = 8;

static Result ReadArgs(Labels *fixups, const char args, FILE *input, Vector *buf, const char name[], const int code);

enum Result assembler(const char *file, const char *outfile) {

    assert(file);
    assert(outfile);

    FILE *fn = fileopen(file, READ);
    FILE *byte_code_file = fileopen(outfile, WRITE);

    Vector buf = {};
    VectorCtor(&buf, VEC_SIZE);

    Labels *fixups = (Labels *) calloc ((size_t) START_LABELS_COL, sizeof (Labels));

    if (!fixups) {
        fileclose(fn);
        fileclose(byte_code_file);
        return NO_MEMORY;
    }
    PushBack(&buf, MY_SIGN);
    PushBack(&buf, VERSION);

    int error = 0;

    #define DEF_CMD(name_cmd, code, args, ...)                                      \
        if (strcasecmp(#name_cmd, str) == 0) {                                      \
            PushBack(&buf, code);                                                   \
            if (args) {                                                             \
                if (ReadArgs(fixups, args, fn, &buf, #name_cmd, code) != SUCCESS) { \
                    error = 1;                                                      \
                    break;                                                          \
                }                                                                   \
            }                                                                       \
            continue;                                                               \
        }                                                                           \

    char str[MAX_STRLEN] = "";
    while (fscanf(fn, "%s", str) == 1 && !error) {
        #include "commands.h"
        if (str[0] == ':') {
            SetLabel(fixups, (char *) str + 1, -1, (int) buf.size);
            continue;
        }
        else {
            printf("\033[" RED "mIncorrect command for <assembler>:\033[0m %s\n", str);
            error = 1;
        }
    }

    size_t index = 0;
    while (fixups[index].name) {
        for (size_t i = 0; i < (size_t) fixups[index].currant_address; i++)
            if (fixups[index].program_address + i) {
                buf.data[fixups[index].program_address[i]] = fixups[index].address;
            }
        free(fixups[index].name);
        free(fixups[index].program_address);
        index++;
    }

    #undef DEF_CMD
    if (!error) fwrite((Elem_t *) buf.data, sizeof (Elem_t), buf.size, byte_code_file);
    fileclose(byte_code_file);
    fileclose(fn);
    free(fixups);
    VectorDtor(&buf);

    if (error)
        return ERROR;
    return SUCCESS;
}

int RegIndex(char *namereg) {

    assert(namereg);

    for (size_t i = 0; i < NUM_OF_REGS; i++) {
        if (strcmp(namereg, regs[i]) == 0)
            return (int) i;
    }
    return -1;
}

static Result ReadArgs(Labels *fixups, const char args, FILE *input, Vector *buf, const char name_cmd[], const int code) {

    assert(input);
    assert(buf);

    int is_number = CheckArgs(args, NUMBER);
    int is_string = CheckArgs(args, STRING);
    int is_ram    = CheckArgs(args, RAM);

    if (is_number + is_string + is_ram > 1) {
        Elem_t num = 0;
        char str[MAX_STRLEN] = "";
        if (fscanf(input, output_id, &num) == 1) {
            PushBack(buf, NUMBER);
            PushBack(buf, num);
            return SUCCESS;
        }
        if (fscanf(input, "[" output_id "]", &num) == 1) {
            PushBack(buf, RAM | NUMBER);
            PushBack(buf, num);
            return SUCCESS;
        }
        if (fscanf(input, "%s", str) == 1) {
            if (str[strlen(str) - 1] == ']') {
                sscanf(str, "%[^]]", str);
                PushBack(buf, RAM | STRING);
                PushBack(buf, RegIndex(str));
                return SUCCESS;
            } else {
                PushBack(buf, STRING);
                PushBack(buf, RegIndex(str));
                return SUCCESS;
            }
        }
        printf("\033[" RED "mMissing arguments for %s\n\033[0m", name_cmd);
        return ERROR;
    }
    if (is_number) {
        Elem_t num = 0;
        if (fscanf(input, output_id, &num) != 1) {
            printf("\033[" RED "mIncorrect argument for %s\033[0m\n", name_cmd);
            return ERROR;
        }
        PushBack(buf, num);
        return SUCCESS;
    }
    if (is_string) {
        char str[MAX_STRLEN] = "";
        if (fscanf(input, "%s", str) != 1) {
            printf("\033[" RED "mIncorrect argument for %s\033[0m\n", name_cmd);
            return ERROR;
        }

        if (CMD_JA <= code && code <= CMD_CALL) {
            SetLabel(fixups, str, (int) buf->size, 0);
            PushBack(buf, 0);
        }
        return SUCCESS;
    }
    printf("\033[" RED "mMissing arguments for %s\n\033[0m", name_cmd);
    return ERROR;
}

int GetArgsAsm(const int argc, const char *argv[], AsmData *data) {

    assert(argv);
    assert(data);

    if (argc == 1) {
        printf("\033[" RED "mFile name expected\n\033[0m");
        return ERROR;
    }
    if (argc == 2) {
        data->input_file = 1;
        return SUCCESS;
    }
    data->input_file = 1;
    data->output_file = 2;

    return SUCCESS;
}