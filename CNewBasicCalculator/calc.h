#ifndef __CALC_H__
#define __CALC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>

#ifndef EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */

#ifndef EXIT_FAILURE
	#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

/* Returns the sum of two numbers added */
double calc_add(const double, const double);

/* Returns the sum of two numbers subtracted */
double calc_sub(const double, const double);

/* Returns the sum of two numbers multiplyed */
double calc_mul(const double, const double);

/* Returns the sum of two numbers divided */
double calc_div(const double, const double);

/* Returns the remainder of two numbers divided */
double calc_mod(const double, const double);

/* Returns the first argument to the power of the second */
double calc_pow(const double, const double);

/* Clears the number and operator stacks */
void calc_clear_stack(void);

int calc_parse_args(const char **, double *);

#include "calc.c"

#endif /* __CALC_H__ */
