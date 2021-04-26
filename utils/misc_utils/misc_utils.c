#include "misc_utils.h"

char** cloneStringsArray(char** array, int n) {
    char** clone = n > 0 
        ? (char**) malloc(sizeof(char*) * n) 
        : NULL;

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
    if (n <= 0) {
        printf("[]\n");
    }
    else {
        printf("[\n");
        for (int i = 0; i < n; ++i) {
            printf("\t%s\n", array[i]);
        }
        printf("]\n");
    }
}

void printIntArray(int* array, int n) {
    if (n <= 0) {
        printf("[]\n");
    }
    else {
        printf("[\n");
        for (int i = 0; i < n; ++i) {
            printf("\t%d\n", array[i]);
        }
        printf("]\n");
    }
}

void printDoubleArray(double* array, int n) {
    if (n <= 0) {
        printf("[]\n");
    }
    else {
        printf("[\n");
        for (int i = 0; i < n; ++i) {
            printf("\t%f\n", array[i]);
        }
        printf("]\n");
    }
}

int digitsCount(int n) {
    return (n == 0 ? 1 : floor(log10(abs(n))) + 1);
}

void swapPointers(void** x, void** y) {
    void* temp = *x;
    *x = *y;
    *y = temp;
}


long long int getCurrentTimestamp() {
    struct timeval tv;
    if (!gettimeofday(&tv, NULL)) {
        return (long long int)(tv.tv_sec) * 1000ll + (long long int)(tv.tv_usec) / 1000ll;
    } else {
        return -1;
    }
}
