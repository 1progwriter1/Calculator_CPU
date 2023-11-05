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

static Result ReadArgs(Label *labels, const char args, FILE *input, Vector *buf, const char name[]);
static int PrepareForCompilation(Vector *buf, Label *labels, FILE *input_file, FILE *output_file);
static int FillLabels(Vector *buf, Label *labels);
static int EndCompilation(FILE *code_file, FILE *byte_code_file, Vector *buf, Label *labels);
static int DoCodeCompilation(FILE *byte_code_file, Vector *buf, Label *labels);
static int RegIndex(char *namereg, const size_t len_of_str);

enum Result CodeCompile(const char *input_file, const char *output_file) {

    assert(input_file);
    assert(output_file);

    Vector buf = {};
    Label *labels = (Label *) calloc ((size_t) START_LABELS_COL, sizeof (Label));
    FILE *code_file = fileopen(input_file, READ);
    FILE *byte_code_file = fileopen(output_file, WRITE);

    if (PrepareForCompilation(&buf, labels, code_file, byte_code_file) != SUCCESS)
        return ERROR;

    assert(labels);

    int error = 0;
    if (DoCodeCompilation(code_file, &buf, labels) != SUCCESS)
        error = 1;

    if (!error) {
        buf.data[INDEX_OF_CODE_LEN] = (Elem_t) buf.size;
        fwrite((Elem_t *) buf.data, sizeof (Elem_t), buf.size, byte_code_file);
    }

    EndCompilation(code_file, byte_code_file, &buf, labels);

    if (error)
        return ERROR;

    return SUCCESS;
}

static int RegIndex(char *namereg, const size_t len_of_str) {

    assert(namereg);

    for (size_t i = 0; i < NUM_OF_REGS; i++) {
        if (strncmp(namereg, regs[i], len_of_str) == 0)
            return (int) i;
    }
    return -1;
}

static Result ReadArgs(Label *fixups, const char args, FILE *input, Vector *buf, const char name_cmd[]) {

    assert(input);
    assert(buf);

    int is_ram_access = (args & RAM_ACCESS) != 0;
    int is_number     = (args & NUMBER) != 0;
    int is_string     = (args & STRING) != 0;

    char str[MAX_STRLEN] = "";
    size_t len_of_str = 0;
    Elem_t num = 0;
    int ind_of_reg = 0;
    if ((is_number + is_string + is_ram_access) > 1) {
        if (fscanf(input, output_id, &num) == 1) {
            PushBack(buf, NUMBER);
            PushBack(buf, num);
            return SUCCESS;
        }
        if (fscanf(input, "[" output_id, &num) == 1) {
            PushBack(buf, RAM_ACCESS | NUMBER);
            PushBack(buf, num);
            return SUCCESS;
        }
        if (fscanf(input, "%s", str) == 1) {
            len_of_str = strlen(str);
            if (str[len_of_str - 1] == ']') {
                PushBack(buf, RAM_ACCESS | STRING);
                if ((ind_of_reg = RegIndex(str, len_of_str- 1)) == -1) {
                    printf(RED "3Incorrect argument for %s" END_OF_COLOR "\n", name_cmd);
                    return ERROR;
                }
                PushBack(buf, ind_of_reg);
                return SUCCESS;
            } else {
                if ((ind_of_reg = RegIndex(str, len_of_str)) == -1) {
                    printf(RED "Incorrect argument for %s" END_OF_COLOR "\n", name_cmd);
                    return ERROR;
                }
                PushBack(buf, STRING);
                PushBack(buf, ind_of_reg);
                return SUCCESS;
            }
        }
        printf(RED "Missing arguments for %s" END_OF_COLOR "\n", name_cmd);
        return ERROR;
    }
    if (is_number) {
        if (fscanf(input, output_id, &num) != 1) {
            printf(RED "Incorrect argument for %s" END_OF_COLOR "\n", name_cmd);
            return ERROR;
        }
        PushBack(buf, num);
        return SUCCESS;
    }
    if (is_string) {
        if (fscanf(input, "%s", str) != 1) {
            printf(RED "Incorrect argument for %s" END_OF_COLOR "\n", name_cmd);
            return ERROR;
        }
        len_of_str = strlen(str);
        if (RegIndex(str, len_of_str) == -1) {
            SetLabel(fixups, str, (int) buf->size, 0);
            PushBack(buf, 0);
        }
        return SUCCESS;
    }
    printf(RED "Missing arguments for %s" END_OF_COLOR "\n", name_cmd);
    return ERROR;
}

int GetArgsAsm(const int argc, const char *argv[], AsmData *data) {

    assert(argv);
    assert(data);

    if (argc == 1) {
        printf(RED "File name expected" END_OF_COLOR "\n");
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

static int PrepareForCompilation(Vector *buf, Label *labels, FILE *code_file, FILE *byte_code_file) {

    assert(buf);

    labels = (Label *) calloc ((size_t) START_LABELS_COL, sizeof (Label));
    if (!labels)
        return NO_MEMORY;

    if (!code_file) {
        fileclose(byte_code_file);
        return FILEOPEN_ERROR;
    }

    if (!byte_code_file) {
        fileclose(code_file);
        return FILEOPEN_ERROR;
    }

    VectorCtor(buf, VEC_SIZE);

    PushBack(buf, 0);
    PushBack(buf, BYTE_CODE_SIGNATURE);
    PushBack(buf, VERSION);

    return SUCCESS;
}

static int FillLabels(Vector *buf, Label *labels) {

    assert(buf);
    assert(labels);

    size_t index = 0;
    while (labels[index].name) {
        for (size_t i = 0; i < (size_t) labels[index].currant_address; i++)
            if (labels[index].program_address + i) {
                buf->data[labels[index].program_address[i]] = labels[index].address;
            }
        free(labels[index].name);
        free(labels[index].program_address);
        index++;
    }

    return SUCCESS;
}

static int EndCompilation(FILE *code_file, FILE *byte_code_file, Vector *buf, Label *labels) {

    assert(code_file);
    assert(byte_code_file);
    assert(buf);
    assert(labels);

    fileclose(byte_code_file);
    fileclose(code_file);
    free(labels);
    VectorDtor(buf);

    return SUCCESS;
}

static int DoCodeCompilation(FILE *code_file, Vector *buf, Label *labels) {

    assert(code_file);
    assert(buf);

    int error = 0;

    #define DEF_CMD(name_cmd, code, args, ...)                                      \
        if (strcasecmp(#name_cmd, str) == 0) {                                      \
            PushBack(buf, code);                                                    \
            if (args) {                                                             \
                if (ReadArgs(labels, args, code_file, buf, #name_cmd) != SUCCESS) { \
                    error = 1;                                                      \
                    break;                                                          \
                }                                                                   \
            }                                                                       \
            continue;                                                               \
        }                                                                           \

    char str[MAX_STRLEN] = "";
    while (fscanf(code_file, "%s", str) == 1 && !error) {
        #include "commands.h"
        if (str[0] == ':') {
            SetLabel(labels, (char *) str + 1, -1, (int) buf->size);
            continue;
        }
        else {
            printf(RED "Incorrect command in <CodeCompile>: %s" END_OF_COLOR "\n", str);
            error = 1;
        }
    }

    #undef DEF_CMD

    if (error)
        return ERROR;

    FillLabels(buf, labels);

    return SUCCESS;
}