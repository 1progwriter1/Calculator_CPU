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

static Result ArgsRead(const char args, FILE *output, const char name[], int *index, char *buf, Vector *labels);
static void PrintLowName(const char name[], FILE *out);
static int IsLabel(Vector *labels, const int index);
static void MakeSpaces(Vector *labels, int *index, FILE *output);
static int PrepareForTranslation(const char *byte_code_file, const char *dis_file, FILE *output_file, Elem_t *len_of_prog, char *buf, Vector *labels, int *index, unsigned long len_of_file);
static int DoCodeTranslation(char *buf, int index, Vector *labels, FILE *disasm_file, Elem_t len_of_prog);

enum Result CodeTranslate(const char *byte_code_file, const char *dis_file) {

    assert(byte_code_file);
    assert(dis_file);

    FILE *disasm_file = fileopen(dis_file, WRITE);
    Vector labels = {};
    unsigned long len_of_file = filelen(byte_code_file);
    char *buf = (char *) calloc (len_of_file + 1, sizeof (char));
    Elem_t len_of_prog = 0;
    int index = 0;
    if (PrepareForTranslation(byte_code_file, dis_file, disasm_file, &len_of_prog, buf, &labels, &index, len_of_file) != SUCCESS) {
        return ERROR;
    }

    int error = 0;
    if (DoCodeTranslation(buf, index, &labels, disasm_file, len_of_prog) != SUCCESS)
        error = 1;

    free(buf);
    fileclose(disasm_file);

    if (error)
        return ERROR;

    return SUCCESS;
}

static void PrintLowName(const char name[], FILE *out) {

    size_t i = 0;
    while (name[i] != '\0') {
        fprintf(out, "%c", name[i++] + 'a' - 'A');
    }
}

static Result ArgsRead(const char args, FILE *output, const char name[], int *index, char *buf, Vector *labels) {

    assert(output);
    assert(buf);
    assert(index);
    assert(labels);

    int is_ram_access = (args & RAM_ACCESS) != 0;
    int is_number     = (args & NUMBER) != 0;
    int is_string     = (args & STRING) != 0;

    MakeSpaces(labels, index, output);

    if (is_number + is_string + is_ram_access > 1) {
        if (*((Elem_t *) buf + (*index)++) == NUMBER) {
            PrintLowName(name, output);
            fprintf(output, " " output_id "\n", *((Elem_t *) buf + (*index)++));
            return SUCCESS;
        }
        if (*((Elem_t *) buf + *index - 1) == STRING) {
            PrintLowName(name, output);
            fprintf(output, " %s\n", regs[*((Elem_t *) buf + (*index)++)]);
            return SUCCESS;
        }
        if (*((Elem_t *) buf + *index - 1) == (RAM_ACCESS | STRING)) {
            PrintLowName(name, output);
            fprintf(output, " [%s]\n", regs[*((Elem_t *) buf + (*index)++)]);
            return SUCCESS;
        }
        if (*((Elem_t *) buf + *index - 1) == (RAM_ACCESS | NUMBER)) {
            PrintLowName(name, output);
            fprintf(output, " [" output_id "]\n", *((Elem_t *) buf + (*index)++));
        }
        printf(RED "Missing argument in byte code file for %s\n" END_OF_COLOR, name);
        return ERROR;
    }
    if (is_number) {
        PrintLowName(name, output);
        fprintf(output, " " output_id "\n", *((Elem_t *) buf + (*index)++));
        return SUCCESS;
    }
    if (is_string) {
        PrintLowName(name, output);
        fprintf(output, " " output_id "\n", *((Elem_t *) buf + (*index)++));
        return SUCCESS;
    }
    PrintLowName(name, output);
    fprintf(output, " %s\n", regs[*((Elem_t *) buf + (*index)++)]);
    return SUCCESS;
}

static int IsLabel(Vector *labels, const int index) {

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

int GetArgsDisasm(const int argc, const char *argv[], DisasmData *data) {

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

static int PrepareForTranslation(const char *byte_code_file, const char *dis_file, FILE *output_file, Elem_t *len_of_prog, char *buf, Vector *labels, int *index, unsigned long len_of_file) {

    assert(byte_code_file);
    assert(dis_file);
    assert(len_of_prog);

    if (!buf)
        return NO_MEMORY;

    if (!output_file) {
        free(buf);
        return FILEOPEN_ERROR;
    }

    FILE *input_file = fileopen(byte_code_file, READ);
    if (!input_file) {
        free(buf);
        fileclose(output_file);
        return FILEOPEN_ERROR;
    }

    fread(buf, sizeof (char), len_of_file + 1, input_file);
    fileclose(input_file);

    *len_of_prog = *((Elem_t *) buf + INDEX_OF_CODE_LEN);

    *index = INDEX_OF_CODE_LEN + 1;
    Elem_t signature = *((Elem_t *) buf + (*index)++);
    Elem_t file_version = *((Elem_t *) buf + (*index)++);
    if (FileVerify((int) signature, (int) file_version) != SUCCESS) {
        fileclose(output_file);
        free(buf);
        return ERROR;
    }

    VectorCtor(labels, LABELS_NUM);
    for (size_t i = INDEX_OF_CODE_LEN + 2; i < (size_t) *len_of_prog; i++)
        if (CMD_JA <= *((Elem_t *) buf + i) && *((Elem_t *) buf + i) <= CMD_CALL)
            PushBack(labels, *((Elem_t *) buf + i + 1));

    return SUCCESS;
}

static int DoCodeTranslation(char *buf, int index, Vector *labels, FILE *disasm_file, Elem_t len_of_prog) {

    int error = 0;

    #define DEF_CMD(name, code, args, ...)                                                                  \
        case (code): {                                                                                      \
            if (args) {                                                                                     \
                if (ArgsRead(args, disasm_file, #name, &index, buf, labels) != SUCCESS)                     \
                    error = 1;                                                                              \
            }                                                                                               \
            else {                                                                                          \
                fprintf(disasm_file, "\t\t");                                                               \
                PrintLowName(#name, disasm_file);                                                           \
                fprintf(disasm_file, "\n");                                                                 \
            }                                                                                               \
            break;                                                                                          \
        }                                                                                                   \

    Elem_t com_num = 0;
    do {
        com_num = *((Elem_t *) buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf(RED "Incorrect command number in <CodeTranslate>\n" END_OF_COLOR);
                error = 1;
                break;
            }
        }
    } while (index != len_of_prog && !error);

    #undef DEF_CMD

    if (error)
        return ERROR;

    return SUCCESS;
}