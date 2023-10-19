#include <stdio.h>
#include "func.h"
#include <assert.h>
#include "calculator_values.h"
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int labels[10] = {};

void SetLabel(const int label_number, const int address) {
    labels[label_number] = address;
}

int GetLabel(const int label_number) {
    return labels[label_number];
}

int FileVerify(const int sign, const int version_icon, const int version_given) {

    if (sign != MY_SIGN) {
        printf("\033[31mUnable to read file\n\033[0m");
        return 0;
    }

    if (version_given != version_icon) {
        printf("\033[31mIncompatible version\n\033[0m");
        return 0;
    }
    return 1;
}

FILE *fileopen(const char *filename, const char mode[]) {

    assert(filename);

    FILE *fn = fopen(filename, mode);

    if (fn == nullptr)
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

int DayNumber() {
    time_t tm = time(NULL);
    struct tm *current_time = localtime(&tm);
    return current_time->tm_yday % 7;
}