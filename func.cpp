#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include <sys/stat.h>

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
