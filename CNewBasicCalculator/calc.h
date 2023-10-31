#ifndef __CALC_H__
#define __CALC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*
 * Returns the sum of two numbers added
 */
double CalculateAdd(const double, const double);

/*
 * Returns the sum of two numbers subtracted
 */
double CalculateSub(const double, const double);

/*
 * Returns the sum of two numbers multiplyed
 */
double CalculateMul(const double, const double);

/*
 * Returns the sum of two numbers divided
 */
double CalculateDiv(const double, const double);

/*
 * Returns the remainder of two numbers divided
 */
double CalculateDiv(const double, const double);

/*
 * Returns the first argument to the power of the second 
 */
double CalculatePow(const double, const double);

/* 
 * Clears the number and operator stacks 
 * Sets the counts for both to 0
 * Should be called if we do more than 1 calculation
 */
void ClearCalculationStack(void);

/*
 * Entry point for the calculator,
 * Checks that all the strings argv are valid,
 * To ensure that we don't get errors later on,
 * then the function will call upon all other functions,
 * in this file to set the double * to the result of, the calculation
 * all you need to do is split up a string into a char * array
 * and pass it off to this function and put your double in there,
 */
int CalculateEntry(const char **, double *, int);

#include "calc.c"

#endif /* __CALC_H__ */
