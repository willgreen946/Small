#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <math.h>

#ifndef EXIT_FAILURE
	#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

#ifndef EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */

#define NUMBER_STACK_MAX 128

#define OPERATOR_STACK_MAX 64 

/*
 * Returns the sum of two numbers added
 */
double calc_add(const double, const double);

/*
 * Returns the sum of two numbers subtracted
 */
double calc_sub(const double, const double);

/*
 * Returns the sum of two numbers multiplyed
 */
double calc_mul(const double, const double);

/*
 * Returns the sum of two numbers divided
 */
double calc_div(const double, const double);

/*
 * Returns the remainder of two numbers divided
 */
double calc_mod(const double, const double);

/*
 * Returns the first argument to the power of the second
 */
double calc_pow(const double, const double);

/*
 * Pushes double to the number stack
 */
static int push_digit(const double);

/*
 * Pushes char to the operator stack
 */
static int push_operator(const char);

/*
 * Sets sum of n1 (op) n2 in the double *, returns EXIT_FAILURE if op is not an operator
 */
static int calculate(const double, const char, const double, double *);

/*
 * Returns 1 if the string is numeric
 */
static int calc_is_numeric(const char *);

/*
 * Goes through the operator stack and the number stack
 */
static double calculate_entry(const char *, const double[]);

/*
 * Entry point for the calculator,
 * Checks that all the strings argv are valid,
 * To ensure that we don't get errors later on,
 * then the function will call upon all other functions,
 * in this file to set the double * to the result of, the calculation
 * all you need to do is split up a string into a char * array
 * and pass it off to this function and put your double in there,
 */
int calc_parse_args(const char **, double *);

/*
 * Stores all the digits to be used in the calculation
 */
static double number_stack[NUMBER_STACK_MAX];

/*
 * Stores number of all numbers in the stack
 */
static int number_stack_count = 0;

/*
 * Stores all the operators to be used in the calculation
 */
static char operator_stack[OPERATOR_STACK_MAX];

/*
 * Stores number of all operators in the stack
 */
static int operator_stack_count = 0;

double
calc_add(const double n1, const double n2)
{
	return (n1 + n2);
}

double
calc_sub(const double n1, const double n2)
{
	return (n1 - n2);
}

double
calc_mul(const double n1, const double n2)
{
	return (n1 * n2);
}

double
calc_div(const double n1, const double n2)
{
	return (n1 / n2);
}

double
calc_mod(const double n1, const double n2)
{
	return ((long)n1 % (long)n2);
}

double
calc_pow(const double n1, const double n2)
{
	return pow(n1, n2);
}

static int 
push_digit(const double num)
{
	if (number_stack_count < NUMBER_STACK_MAX)
		number_stack[number_stack_count++] = num;
	else {
		err(EXIT_FAILURE, "too many numbers in number stack");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int 
push_operator(const char op)
{
	if (operator_stack_count < OPERATOR_STACK_MAX)
		operator_stack[operator_stack_count++] = op;
	else {
		err(EXIT_FAILURE, "too many operators in operators stack");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int 
calculate(const double n1, const char op, const double n2, double * result)
{
	switch (op) {
		case '+':
			*result = calc_add(n1,n2);
			return EXIT_SUCCESS;
		case '-':
			*result = calc_sub(n1,n2);
			return EXIT_SUCCESS;
		case '*':
			*result = calc_mul(n1,n2);
			return EXIT_SUCCESS;
		case 'x':
			*result = calc_mul(n1,n2);
			return EXIT_SUCCESS;
		case 'X':
			*result = calc_mul(n1,n2);
			return EXIT_SUCCESS;
		case '/':
			*result = calc_div(n1,n2);
			return EXIT_SUCCESS;
		case '^':
			*result = calc_pow(n1,n2);
			return EXIT_SUCCESS;
		case '~':
			*result = calc_pow(n1,n2);
			return EXIT_SUCCESS;
		case '%':
			*result = calc_mod(n1,n2);
			return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

static int
calc_is_numeric(const char * str)
{
	size_t i;
	unsigned int decimal_count = 0;

	if (strspn(str, "+-.0123456789") != strlen(str))
		return EXIT_FAILURE;

	for (i = 0; str[i]; i++) {
		if (str[i] == '.' && !isdigit(str[i-1]))
			return EXIT_FAILURE;

		if (i != 0 && str[i] == '-')
			return EXIT_FAILURE;

		if (i != 0 && str[i] == '+')
			return EXIT_FAILURE;

		if (str[i] == '.')
			++decimal_count;
	}

	if (decimal_count >= 2)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

static double
calculate_entry(const char * op, const double num[])
{
	size_t i;
	size_t k;
	double active, result;

	active = result = 0;

	for (i = 0, k = 0; num[i]; i++) {
		active = num[i];

		if (!i)
			calculate(active, op[k++], num[++i], &result);
		else
			calculate(result, op[k++], active, &result);
	}

	return result;
}

int
calc_parse_args(const char ** argv, double * result)
{
	short n = 1;
	double test;

	while (*++argv) {
		if (n) {
			if (calc_is_numeric(*argv)) {
				err(EXIT_FAILURE, "%s is not numeric", *argv);
				return EXIT_FAILURE;
			}

			if (push_digit((double)strtod(*argv, NULL)))
				return EXIT_FAILURE;

			n = 0;
		}

		else {
			if (calculate(1, **argv, 1, &test) || strlen(*argv) >= 2) {
				err(EXIT_FAILURE, "%s is not a valid operator", *argv);
				return EXIT_FAILURE;
			}

			if (push_operator(**argv))
				return EXIT_FAILURE;

			n = 1;
		}
	}

	*result = calculate_entry(operator_stack, number_stack);
	return EXIT_SUCCESS;
}
