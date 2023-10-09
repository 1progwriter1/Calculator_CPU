#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include <sys/stat.h>
#include <string.h>

int FileVerify(FILE *fn, const char *sign, const int version) {

    assert(sign);

    char signature[MAX_STRLEN] = "";
    if (fscanf(fn, "%s", signature) != 1) {
        printf("\033[31mUnable to read file\n\033[0m");
        return 0;
    }

    if (strcmp(sign, signature) != 0) {
        printf("%s %s\n", sign, signature);
        printf("\033[31mIncompatible file\n\033[0m");
        return 0;
    }

    int code_version = 0;
    if (fscanf(fn, "%d", &code_version) != 1) {
        printf("\033[31mUnable to verify version\n\033[0m");
        return 0;
    }
    if (version != code_version) {
        printf("\033[31mIncorrect version\n\033[0m");
        return 0;
    }
    return 1;
}

FILE *fileopen(const char *filename, const char mode[]) {

    assert(filename);

    FILE *fn = fopen(filename, mode);

    if (fn == NULL)
        perror("");

    return fn;
}

long long int filelen(const char *filename) {

    assert(filename);

    struct stat buf = {};
    if (stat(filename, &buf) == - 1)
        return -1;

    return buf.st_size;
}

char *readfile(FILE *fn, long long int length) {

    assert(fn);

    char *buf = (char *) calloc (length + 1, sizeof (char));
    fread(buf, sizeof (char), length, fn);

    return buf;
}
