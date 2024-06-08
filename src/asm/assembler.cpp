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

static Result ReadArgs(AsmData *data, const char args, const char name_cmd[]);
static int PrepareForCompilation(AsmData *data);
static int FillLabels(Vector *buf, Labels *lbls);
static int EndCompilation(AsmData *data);
static int DoCodeCompilation(AsmData *data);
static int RegIndex(char *namereg, const size_t len_of_str);
static void SkipLine(FILE *fn);
static int GetAsmData(AsmData *data, const char *input_file, const char *output_file);

enum Result CodeCompile(const char *input_file, const char *output_file) {

    assert(input_file);
    assert(output_file);

    AsmData data = {};
    GetAsmData(&data, input_file, output_file);

    if (PrepareForCompilation(&data) != SUCCESS) {
        return ERROR;
    }

    bool error = 0;
    if (DoCodeCompilation(&data) != SUCCESS) {
        error = 1;
    }

    if (!error) {
        data.buf.data[INDEX_OF_CODE_LEN] = (Elem_t) data.buf.size;
        fwrite((Elem_t *) data.buf.data, sizeof (Elem_t), data.buf.size, data.byte_code_file);
    }

    EndCompilation(&data);

    if (error)
        return ERROR;

    return SUCCESS;
}

static int PrepareForCompilation(AsmData *data) {

    assert(data);

    if (LabelsCtor(&data->lbls) != SUCCESS)
        return ERROR;

    if (!data->code_file) {
        LabelsDtor(&data->lbls);
        fileclose(data->byte_code_file);
        return FILEOPEN_ERROR;
    }

    if (!data->byte_code_file) {
        LabelsDtor(&data->lbls);
        fileclose(data->code_file);
        return FILEOPEN_ERROR;
    }

    if (VectorCtor(&data->buf, VEC_SIZE) != SUCCESS) {
        LabelsDtor(&data->lbls);
        fileclose(data->code_file);
        fileclose(data->byte_code_file);
    }

    PushBack(&data->buf, 0);
    PushBack(&data->buf, BYTE_CODE_SIGNATURE);
    PushBack(&data->buf, VERSION);

    return SUCCESS;
}

static int DoCodeCompilation(AsmData *data) {

    assert(data);
    assert(&data->buf);
    assert(data->code_file);

    bool error = 0;

    #define DEF_CMD(name_cmd, code, args, ...)                      \
        if (strcasecmp(#name_cmd, str) == 0) {                      \
            PushBack(&data->buf, code);                             \
            if (args != NO_ARGS) {                                  \
                if (ReadArgs(data, args, #name_cmd) != SUCCESS) {   \
                    error = 1;                                      \
                    break;                                          \
                }                                                   \
            }                                                       \
            continue;                                               \
        }                                                           \

    char str[MAX_STRLEN] = "";
    while (fscanf(data->code_file, "%s", str) == 1 && !error) {
        if (str[0] == ':') {
            SetLabel(&data->lbls, (char *) str + 1, NO_ADDRESS, (int) (data->buf).size);
            continue;
        }
        if (str[0] == ';')  {
            SkipLine(data->code_file);
            continue;
        }

        #include "commands.h"

        printf(RED "Incorrect command in <CodeCompile>: %s" END_OF_COLOR "\n", str);
        error = 1;
    }

    #undef DEF_CMD

    if (error)
        return ERROR;

    FillLabels(&data->buf, &data->lbls);

    return SUCCESS;
}

static int EndCompilation(AsmData *data) {

    assert(data);
    assert(data->code_file);
    assert(data->byte_code_file);
    assert(&data->buf);
    assert(&data->lbls);

    fileclose(data->byte_code_file);
    fileclose(data->code_file);
    LabelsDtor(&data->lbls);
    VectorDtor(&data->buf);

    return SUCCESS;
}

static void SkipLine(FILE *fn) {

    assert(fn);

    while (fgetc(fn) != '\n')
        ;
}

static int RegIndex(char *namereg, const size_t len_of_str) {

    assert(namereg);

    for (size_t i = 0; i < NUM_OF_REGS; i++) {
        if (strncmp(namereg, regs[i], len_of_str) == 0)
            return (int) i;
    }
    return -1;
}

static Result ReadArgs(AsmData *data, const char args, const char name_cmd[]) {

    assert(data);
    assert(data->code_file);
    assert(&data->buf);
    assert(&data->lbls);

    bool is_ram_access = (args & RAM_ACCESS) != 0;
    bool is_number     = (args & NUMBER) != 0;
    bool is_string     = (args & STRING) != 0;

    char str[MAX_STRLEN] = "";
    size_t len_of_str = 0;
    Elem_t type_of_arg = NO_ARGS;
    Elem_t arg = 0;
    bool incorrect_arg = 0;

    if (is_number + is_string + is_ram_access > 1) {
        if (fscanf(data->code_file, output_id, &arg) == 1) {
            type_of_arg = NUMBER;
        }
        else if (fscanf(data->code_file, "[" output_id, &arg) == 1) {
            type_of_arg = RAM_ACCESS | NUMBER;
            char c = 0;
            fscanf(data->code_file, "%c", &c);
        }
        else if (fscanf(data->code_file, "%s", str) == 1) {
            len_of_str = strlen(str);
            if (str[len_of_str - 1] == ']') {
                type_of_arg = RAM_ACCESS | STRING;
                if ((arg = RegIndex(str, len_of_str- 1)) == -1)
                    incorrect_arg = 1;
            } else {
                if ((arg = RegIndex(str, len_of_str)) == -1)
                    incorrect_arg = 1;
                type_of_arg = STRING;
            }
        }
    }
    else if (is_number) {
        if (fscanf(data->code_file, output_id, &arg) != 1)
            incorrect_arg = 1;
    }
    else if (is_string) {
        if (fscanf(data->code_file, "%s", str) != 1) {
            incorrect_arg = 1;
        }
        else {
            SetLabel(&data->lbls, str, (int) data->buf.size, NO_ADDRESS);
            arg = 0;
        }
    }
    else {
        printf(RED "Missing arguments for %s" END_OF_COLOR "\n", name_cmd);
        return ERROR;
    }

    if (incorrect_arg) {
        printf(RED "Incorrect argument for %s" END_OF_COLOR "\n", name_cmd);
        return ERROR;
    }

    if (type_of_arg != NO_ARGS)
        PushBack(&data->buf, type_of_arg);
    PushBack(&data->buf, arg);

    return SUCCESS;
}

static int FillLabels(Vector *buf, Labels *lbls) {

    assert(buf);
    assert(lbls);

    for (size_t i = 0; i < (size_t) lbls->current; i++) {
        for (size_t j = 0; j < (size_t) (lbls->data + i)->current; j++)
            buf->data[(lbls->data + i)->addresses_to_fill[j]] = (lbls->data + i)->address;
    }

    return SUCCESS;
}

static int GetAsmData(AsmData *data, const char *input_file, const char *output_file) {

    assert(data);
    assert(input_file);
    assert(output_file);

    data->buf = {};
    data->lbls = {};
    data->code_file = fileopen(input_file, READ);
    data->byte_code_file = fileopen(output_file, WRITE);

    return SUCCESS;
}
