#include <stdio.h>
#include "disassembler.h"
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include "Stack/stack.h"
#include <ctype.h>
#include <string.h>
#include "my_vector.h"

static Result ArgsRead(const char args, FILE *output, const char name[], int *index, char *buf, Vector *labels);
static void PrintLowName(const char name[], FILE *out);
static int IsLabel(Vector *labels, const int index);
static void MakeSpaces(Elem_t label, const int NUM_OF_SPACES, FILE *output);

enum Result disassembler(const char *file, const char *dis_file) {

    assert(file);

    FILE *byte_code_file = fileopen(file, READ);
    FILE *disass_file = fileopen(dis_file, WRITE);

    Vector labels = {};
    VectorCtor(&labels, LABELS_NUM);

    unsigned long len_of_file = filelen(file);
    char *buf = (char *) calloc (len_of_file + 1, sizeof (char));

    size_t len_of_prog = fread(buf, sizeof (char), len_of_file, byte_code_file) / sizeof (Elem_t);

    for (size_t i = 0; i < len_of_prog; i++)
        if (CMD_JA <= *((Elem_t *) buf + i) && *((Elem_t *) buf + i) <= CMD_CALL)
            PushBack(&labels, *((Elem_t *) buf + i + 1));

    int index = 0;
    int sign = *(int *)((Elem_t *) buf + index++);
    int file_version = *(int *)((Elem_t *) buf + index++);
    if (!FileVerify(sign, VERSION, file_version)) {
        free(buf);
        fileclose(byte_code_file);
        fileclose(disass_file);
        return ERROR;
    }

    int error = 0;
    #define DEF_CMD(name, code, args, ...)                                                                  \
        case (code): {                                                                                      \
            if (args) {                                                                                     \
                if (ArgsRead(args, disass_file, #name, &index, buf, &labels) != SUCCESS)                    \
                    error = 1;                                                                              \
            }                                                                                               \
            else {                                                                                          \
                fprintf(disass_file, "\t\t");                                                               \
                PrintLowName(#name, disass_file);                                                           \
                fprintf(disass_file, "\n");                                                                 \
            }                                                                                               \
            break;                                                                                          \
        }                                                                                                   \

    Elem_t com_num = 0;
    do {
        com_num = *((Elem_t *) buf + index++);
        switch (com_num) {
            #include "commands.h"
            default: {
                printf("\033[" RED "mIncorrect command number for <disassembler>\n\033[0m");
                error = 1;
                break;
            }
        }
    } while ((size_t) index != len_of_prog && !error);

    #undef DEF_CMD
    free(buf);
    fileclose(byte_code_file);
    fileclose(disass_file);

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

    int is_number = CheckArgs(args, NUMBER);
    int is_string =  CheckArgs(args, STRING);
    int is_ram = CheckArgs(args, RAM);

    const int NUM_OF_SPACES = 8;
    Elem_t label = 0;
    if (IsLabel(labels, *index - 1)) {
        label = *index - 1;
    }
    MakeSpaces(label, NUM_OF_SPACES, output);

    if (is_number + is_string + is_ram > 1) {
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
        if (*((Elem_t *) buf + *index - 1) == (RAM | STRING)) {
            PrintLowName(name, output);
            fprintf(output, " [%s]\n", regs[*((Elem_t *) buf + (*index)++)]);
            return SUCCESS;
        }
        if (*((Elem_t *) buf + *index - 1) == (RAM | NUMBER)) {
            PrintLowName(name, output);
            fprintf(output, " [" output_id "]\n", *((Elem_t *) buf + (*index)++));
        }
        printf("\033[" RED "mMissing argument in byte_code file for %s\n\033[0m", name);
        return ERROR;
    }
    if (is_number) {
        PrintLowName(name, output);
        fprintf(output, " lable:" output_id "\n", *((Elem_t *) buf + (*index)++));
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

static void MakeSpaces(Elem_t label, const int NUM_OF_SPACES, FILE *output) {

    assert(output);

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