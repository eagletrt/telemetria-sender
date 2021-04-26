#ifndef TELEMETRY_MISC_UTILS
#define TELEMETRY_MISC_UTILS

/** IMPORTS **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/sysinfo.h>


/* FUNCTIONS */

/**
 * Clones an array of strings
 * 
 * @param array The array of strings
 * @param n The number of strings in the array
 * @return The cloned array of strings. NULL if n is zero
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
 * Prints an array of integers
 * 
 * @param array The array of integers
*/
void printIntArray(int* array, int n);

/**
 * Prints an array of double
 * 
 * @param array The array of double
*/
void printDoubleArray(double* array, int n);

/**
 * Returns the number of digits of a number
 * 
 * @param n The number whose digits are counted
 * @return the number of digits of the passed argument
*/
int digitsCount(int n);

/**
 * Swaps two pointers
 * 
 * @param x The first pointer that needs to be swapped
 * @param y The second pointer that needs to be swapped
*/
void swapPointers(void** x, void** y);

/**
 * Gets the current timestamp in milliseconds
 * @return The current timestamp in milliseconds
*/
long long int getCurrentTimestamp();

#endif