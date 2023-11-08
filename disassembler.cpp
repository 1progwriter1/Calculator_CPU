#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "disassembler.h"
#include "Stack/stack.h"
#include <ctype.h>
#include <string.h>
#include "my_vector.h"

const int NUM_OF_SPACES = 8;

static Result ArgsRead(DisasmData *data, const char args, const char name[], int *index);
static void PrintLowName(const char name[], FILE *out);
static bool IsLabel(Vector *labels, const int index);
static void MakeSpaces(Vector *labels, int *index, FILE *output);
static int PrepareForTranslation(DisasmData *data);
static int DoCodeTranslation(DisasmData *data);
static int GetDisasmdata(DisasmData *data, const char *byte_code_file, const char *dis_file);

enum Result CodeTranslate(const char *byte_code_file, const char *dis_file) {

    assert(byte_code_file);
    assert(dis_file);

    DisasmData data = {};
    GetDisasmdata(&data, byte_code_file, dis_file);

    if (PrepareForTranslation(&data) != SUCCESS) {
        return ERROR;
    }

    bool error = 0;
    if (DoCodeTranslation(&data) != SUCCESS)
        error = 1;

    free(data.buf);
    fileclose(data.dst_file);

    if (error)
        return ERROR;

    return SUCCESS;
}

static int PrepareForTranslation(DisasmData *data) {

    assert(data);
    assert(data->byte_code_file);

    if (!data->buf)
        return NO_MEMORY;

    if (!data->dst_file) {
        free(data->buf);
        return FILEOPEN_ERROR;
    }

    FILE *input_file = fileopen(data->byte_code_file, READ);
    if (!data->dst_file) {
        free(data->buf);
        fileclose(data->dst_file);
        return FILEOPEN_ERROR;
    }

    fread(data->buf, sizeof (char), data->len_of_file + 1, input_file);
    fileclose(input_file);

    Header filedata = *((Header *) data->buf);
    data->len_of_prog = filedata.len_of_prog;
    if (FileVerify(&filedata) != SUCCESS) {
        fileclose(data->dst_file);
        free(data->buf);
        return ERROR;
    }

    VectorCtor(&data->labels, LABELS_NUM);
    for (size_t i = INDEX_OF_CODE_LEN + 2; i < (size_t) data->len_of_prog; i++)
        if (CMD_JA <= *((Elem_t *) data->buf + i) && *((Elem_t *) data->buf + i) <= CMD_CALL)
            PushBack(&data->labels, *((Elem_t *) data->buf + i + 1));

    return SUCCESS;
}

static int DoCodeTranslation(DisasmData *data) {

    int index = START_OF_PROG;
    bool error = 0;

    #define DEF_CMD(name, code, args, ...)                              \
        case (code): {                                                  \
            if (args) {                                                 \
                if (ArgsRead(data, args, #name, &index) != SUCCESS)     \
                    error = 1;                                          \
            }                                                           \
            else {                                                      \
                fprintf(data->dst_file, "\t\t");                        \
                PrintLowName(#name, data->dst_file);                    \
                fprintf(data->dst_file, "\n");                          \
            }                                                           \
            break;                                                      \
        }                                                               \

    Elem_t com_num = 0;
    do {
        com_num = *((Elem_t *) data->buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf(RED "Incorrect command number in <CodeTranslate>\n" END_OF_COLOR);
                error = 1;
                break;
            }
        }
    } while (index != data->len_of_prog && !error);

    #undef DEF_CMD

    if (error)
        return ERROR;

    return SUCCESS;
}

static void PrintLowName(const char name[], FILE *output_file) {

    size_t i = 0;
    while (name[i] != '\0') {
        fprintf(output_file, "%c", tolower(name[i++]));
    }
}

static Result ArgsRead(DisasmData *data, const char args, const char name[], int *index) {

    assert(data);
    assert(data->dst_file);
    assert(data->buf);
    assert(index);
    assert(&data->labels);

    MakeSpaces(&data->labels, index, data->dst_file);
    PrintLowName(name, data->dst_file);

    bool is_ram_access = (args & RAM_ACCESS) != 0;
    bool is_number     = (args & NUMBER) != 0;
    bool is_string     = (args & STRING) != 0;

    Elem_t arg_type = NO_ARGS;
    if (is_number + is_string + is_ram_access > 1)
        arg_type = *((Elem_t *) data->buf + (*index)++);

    if (arg_type == NUMBER || arg_type == (RAM_ACCESS | NUMBER)) {
        if ((arg_type & RAM_ACCESS) != 0)
            fprintf(data->dst_file, " [" output_id "]\n", *((Elem_t *) data->buf + (*index)++));
        else
            fprintf(data->dst_file, " " output_id "\n", *((Elem_t *) data->buf + (*index)++));
    }
    else if (arg_type == STRING || arg_type == (RAM_ACCESS | STRING)) {
        if ((arg_type & RAM_ACCESS) != 0)
            fprintf(data->dst_file, " [%s]\n" ,regs[*((Elem_t *) data->buf + (*index)++)]);
        else
            fprintf(data->dst_file, " %s\n" ,regs[*((Elem_t *) data->buf + (*index)++)]);
    }
    else if (arg_type == NO_ARGS) {
        fprintf(data->dst_file, " " output_id "\n", *((Elem_t *) data->buf + (*index)++));
    }
    else {
        printf(RED "Incorrect argument for %s" END_OF_COLOR "\n", name);
        return ERROR;
    }
    return SUCCESS;
}

static bool IsLabel(Vector *labels, const int index) {

    assert(labels);

    for (size_t i = 0; i < labels->size; i++)
        if (labels->data[i] == index)
            return 1;
    return 0;
}

static void MakeSpaces(Vector *labels, int *index, FILE *output) {

    assert(output);

    Elem_t label = 0;
    if (IsLabel(labels, *index - 1)) {
        label = *index - 1;
    }

    for (size_t i = 0; i < (size_t) NUM_OF_SPACES; i++) {
        if (i == 0 && label != 0) {
            fprintf(output, ":" output_id, label);
            continue;
        }
        if (label != 0) {
            label /= 10;
            continue;
        }
        fprintf(output, " ");
    }
}

static int GetDisasmdata(DisasmData *data, const char *byte_code_file, const char *dis_file) {

    assert(data);
    assert(byte_code_file);
    assert(dis_file);

    data->dst_file = fileopen(dis_file, WRITE);
    data->labels = {};
    data->len_of_file = filelen(byte_code_file);
    data->buf = (char *) calloc (data->len_of_file + 1, sizeof (char));
    data->len_of_prog = 0;
    data->byte_code_file = byte_code_file;

    return SUCCESS;
}