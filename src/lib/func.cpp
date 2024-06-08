#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int FileVerify(Header *filedata) {

    if (filedata->signature != BYTE_CODE_SIGNATURE) {
        printf(RED "Unable to read file\n" END_OF_COLOR);
        return ERROR;
    }

    if (filedata->version != VERSION) {
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
        return ERROR;

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

void fileclose(FILE *fn) {

    assert(fn);

    if (fn != stdout)
        if (fclose(fn) != 0)
            fprintf(stderr, "File not closed\n");
}