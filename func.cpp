#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include <sys/stat.h>
#include <string.h>
#include <time.h>

static const int MAX_INPUT_LEN = 50;

int FileVerify(const int signature, const int version_given) {

    if (signature != BYTE_CODE_SIGNATURE) {
        printf(RED "Unable to read file\n" END_OF_COLOR);
        return ERROR;
    }

    if (version_given != VERSION) {
        printf(RED "Incompatible version\n" END_OF_COLOR);
        return ERROR;
    }
    return SUCCESS;
}

FILE *fileopen(const char *filename, const char mode[]) {

    assert(filename);

    FILE *fn = fopen(filename, mode);

    if (fn == nullptr)
        perror("");

    return fn;
}

unsigned long filelen(const char *filename) {

    assert(filename);

    struct stat buf = {};
    if (stat(filename, &buf) == - 1)
        return NULL;

    return (unsigned long) buf.st_size;
}

char *readfile(FILE *fn, unsigned long length) {

    assert(fn);

    char *buf = (char *) calloc (length + 1, sizeof (char));
    fread(buf, sizeof (char), length, fn);

    return buf;
}

int DayNumber() {
    time_t tm = time(NULL);
    struct tm *current_time = localtime(&tm);
    return current_time->tm_yday % 7;
}

int CheckArgs(const char args, const char arg) {

    return (args & arg) != 0;
}

int MainArgs(int argc, const char *argv[], maindata *data) {

    assert(argv);
    assert(data);

    char line[MAX_INPUT_LEN] = "";
    for (size_t i = 1; i < (size_t) argc; i++) {
        if (strcmp(argv[i], "--run") == 0) {
            data->processor = 1;
            i++;
            if (i < (size_t) argc && sscanf(argv[i], "%[^-]", line) != 1) {
                printf("\033[31mFile name for processor expected\033[0m\n");
                return 0;
            }
            data->proc_file = (int) i;
            continue;
        }
        if (strcmp(argv[i], "--asm") == 0) {
            data->assembler = 1;
            i++;
            if (i < (size_t) argc && sscanf(argv[i], "%[^-]", line) != 1) {
                printf("\033[31mFile name for assembler expected\033[0m\n");
                return 0;
            }
            data->asm_file_in = (int) i;
            i++;
            if (i < (size_t) argc && sscanf(argv[i], "%[^-]", line) == 1)
                data->asm_file_out = (int) i;
            continue;
        }
        if (strcmp(argv[i], "--disasm") == 0) {
            data->disassembler = 1;
            i++;
            if (i < (size_t) argc && sscanf(argv[i], "%[^-]", line) != 1) {
                printf("\033[31mFile name for disassembler expected\033[0m\n");
                return 0;
            }
            data->dis_file_in = (int) i;
            i++;
            if (i < (size_t) argc && sscanf(argv[i], "%[^-]", line) == 1)
                data->dis_file_out = (int) i;
            continue;
        }
        printf("\033[31mCommand \"%s\" not found\033[0m\n", argv[i]);
        return 0;
    }
    return 1;
}