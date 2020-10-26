#include "misc_utils.h"

char** cloneStringsArray(char** array, int n) {
    char** clone = (char**) malloc(sizeof(char*) * n);

    for (int i = 0; i < n; ++i) {
        clone[i] = strdup(array[i]);
    }

    return clone;
}

void freeStringsArray(char** array, int* n) {
    for (int i = 0; i < *n; ++i) {
        free(array[i]);
    }
    free(array);
    *n = 0;
}

void printStringsArray(char** array, int n) {
    printf("[\n");
    for (int i = 0; i < n; ++i) {
        printf("\t%s\n", array[i]);
    }
    printf("]\n");
}

void printIntArray(int* array) {
    const int n = sizeof(array) / sizeof(int);

    printf("[\n");
    for (int i = 0; i < n; ++i) {
        printf("\t%d\n", array[i]);
    }
    printf("]\n");
}

void printDoubleArray(double* array) {
    const int n = sizeof(array) / sizeof(double);

    printf("[\n");
    for (int i = 0; i < n; ++i) {
        printf("\t%f\n", array[i]);
    }
    printf("]\n");
}

int digitsCount(int n) {
    return (n == 0 ? 1 : floor(log10(abs(n))) + 1);
}