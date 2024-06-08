#include <stdio.h>
#include "../lib/func.h"
#include <assert.h>
#include "../lib/calculator_values.h"
#include "disassembler.h"
#include "../stack/stack.h"
#include <ctype.h>
#include <string.h>
#include "../vector/my_vector.h"
#include <stdlib.h>

const int NUM_OF_SPACES = 8;

static int ArgsRead(DisasmData *data, const char args, const char name[], int *index);
static void PrintLowName(const char name[], FILE *out);
static bool IsLabel(Vector *labels, const int index);
static void MakeSpaces(Vector *labels, const int index, FILE *output);
static int PrepareForTranslation(DisasmData *data);
static int DoCodeTranslation(DisasmData *data);
static int GetDisasmdata(DisasmData *data, const char *byte_code_file, const char *dis_file);
static int GetLabels(const Elem_t *buf, Vector *labels, const Elem_t len_of_prog);

int CodeTranslate(const char *byte_code_file, const char *dis_file) {

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
        return FILE_OPEN_ERROR;
    }

    FILE *input_file = fileopen(data->byte_code_file, READ);
    if (!data->dst_file) {
        free(data->buf);
        fileclose(data->dst_file);
        return FILE_OPEN_ERROR;
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
    GetLabels((Elem_t *) data->buf, &data->labels, data->len_of_prog);

    return SUCCESS;
}

static int DoCodeTranslation(DisasmData *data) {

    int index = START_OF_PROG;
    bool error = 0;

    #define DEF_CMD(name, code, args, ...)                              \
        case (code): {                                                  \
            MakeSpaces(&data->labels, index - 1, data->dst_file);       \
            PrintLowName(#name, data->dst_file);                        \
            if (args) {                                                 \
                if (ArgsRead(data, args, #name, &index) != SUCCESS)     \
                    error = 1;                                          \
            }                                                           \
            else {                                                      \
                fprintf(data->dst_file, "\n");                          \
            }                                                           \
            break;                                                      \
        }                                                               \

    Elem_t com_num = 0;
    do {
        com_num = *((Elem_t *) data->buf + index++);
        switch (com_num) {
            #include "../lib/commands.h"
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

static int ArgsRead(DisasmData *data, const char args, const char name[], int *index) {

    assert(data);
    assert(data->dst_file);
    assert(data->buf);
    assert(index);
    assert(&data->labels);

    bool is_ram_access = (args & RAM_ACCESS) != 0;
    bool is_number     = (args & NUMBER) != 0;
    bool is_string     = (args & STRING) != 0;

    Elem_t arg_type = NO_ARG_TYPE;
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
    else if (arg_type == NO_ARG_TYPE) {
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

static void MakeSpaces(Vector *labels, const int index, FILE *output) {

    assert(output);

    Elem_t label = 0;
    if (IsLabel(labels, index)) {
        label = index;
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

static int GetLabels(const Elem_t *buf, Vector *labels, const Elem_t len_of_prog) {

    assert(buf);
    assert(labels);

    #define DEF_CMD(name, code, args, ...)                                      \
        case (code):                                                            \
            if (args == NO_ARGS)                                                \
                index++;                                                        \
            else if (args == NUMBER || args == STRING || args == RAM_ACCESS)    \
                index += 2;                                                     \
            else                                                                \
                index += 3;                                                     \
            break;                                                              \

    size_t index = START_OF_PROG;
    Elem_t com_num = 0;
    for (size_t i = 0; i < (size_t) len_of_prog; i++) {
        com_num = *(buf + index);
        if (CMD_JA <= *(buf + index) && *(buf + index) <= CMD_CALL)
            PushBack(labels, *(buf + index + 1));
        switch (com_num) {
            #include "../lib/commands.h"
        }
    }
    return SUCCESS;
}