#ifndef TELEMETRY_MISC_UTILS
#define TELEMETRY_MISC_UTILS

/** IMPORTS **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* FUNCTIONS */

/**
 * Clones an array of strings
 * 
 * @param array The array of strings
 * @param n The number of strings in the array
*/
char** cloneStringsArray(char** array, int n);

/**
 * Deallocates an array of strings and sets its size to 0
 * 
 * @param array The array of strings
 * @param n The pointer to the number of strings in the array
*/
void freeStringsArray(char** array, int* n);

/**
 * Prints an array of strings
 * 
 * @param array The array of strings
 * @param n The number of strings in the array
*/
void printStringsArray(char** array, int n);

/**
 * Returns the number of digits of a number
 * 
 * @param n The number whose digits are counted
*/
int digitsCount(int n);

#endif