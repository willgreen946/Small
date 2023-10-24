#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* Pushes double to the number stack */
static void push_digit(const double);

/* Pushes char to the operator stack */
static void push_operator(const char);

/* Returns sum of two doubles using the char as the operator */
static double calculate(const double, const char, const double);

/* Returns 1 if the string is numeric */
static int calc_is_numeric(const char *);

/* Returns 1 if the char is a valid operator */
static int calc_is_operator(const char);

/* Goes through the operator stack and the number stack */
static double calculate_entry(const char *, const double[]);

/*
 * Entry point for the calculator,
 * Checks that all the strings argv are valid,
 * To ensure that we don't get errors later on,
 * then the function will call upon all other functions,
 * in this file to set the double * to the result of,
 * the calculation.
 * all you need to do is split up a string into a char * array
 * and pass it off to this function and put your double in there,
 */
int calc_parse_args(const char **, double *);

static struct OPERATOR_MAP {
	const char operator;
	double(*function)(double, double);
};

static struct OPERATOR_MAP operator_map[] = {
	{ '+', calc_add },
	{ '-', calc_sub },
	{ 'x', calc_mul },
	{ 'X', calc_mul },
	{ '*', calc_mul },
	{ '/', calc_div },
	{ '%', calc_mod },
	{ '^', calc_pow },
	{ '~', calc_pow },
};

/* Stores all the digits to be used in the calculation */
static double number_stack[128];

/* Stores all the operators to be used in the calculation */
static char operator_stack[64];

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
	if (!n1 || !n2) {
		fprintf(stderr, "ERROR:Cannot divide by 0\n");
		return 0.0;
	}

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

void
calc_clear_stack(void)
{
	size_t i;

	for (i = 0; number_stack[i]; i++)
		number_stack[i] = 0;

	for (i = 0; operator_stack[i]; i++)
		operator_stack[i] = (char)0;
}

static void
push_digit(const double num)
{
	size_t i;

	for (i = 0; number_stack[i]; i++)
		;

	number_stack[i] = num;
}

static void
push_operator(const char op)
{
	size_t i;

	for (i = 0; operator_stack[i]; i++)
		;

	operator_stack[i] = op;
}

static double
calculate(const double n1, const char op, const double n2)
{
	size_t i;

	for (i = 0; operator_map[i].operator; i++)
		if (op == operator_map[i].operator)
			return operator_map[i].function(n1, n2);
	return 0.0;
}

static int
calc_is_numeric(const char * str)
{
	unsigned int decimal_count = 0;
	size_t i;

	if (strspn(str, "+-.0123456789") != strlen(str))
		return 0;

	for (i = 0; str[i]; i++) {
		if (i != 0 && str[i] == '-')
			return 0;

		if (i != 0 && str[i] == '+')
			return 0;

		if (str[i] == '.')
			++decimal_count;
	}

	if (decimal_count >= 2)
		return 0;

	return 1;
}

/* Checks that op char is a valid operator in the program */
static int
calc_is_operator(const char op)
{
	size_t i;

	for (i = 0; operator_map[i].operator; i++)
		if (op == operator_map[i].operator)
			return 1;
	return 0;
}

static double
calculate_entry(const char * op, const double num[])
{
	double active, result;
	size_t i, k;

	active = result = 0.0;

	for (i = 0, k = 0; num[i]; i++) {
		active = num[i];

		if (!i)
			result = calculate(active, op[k++], num[++i]);
		else
			result = calculate(result, op[k++], active);
	}

	return result;
}

int
calc_parse_args(const char ** argv, double * result)
{
	int n = 1;

	while (*++argv) {
		if (n) {
			if (!calc_is_numeric(*argv)) {
				fprintf(stderr, "ERROR:%s is not numeric\n", *argv);
				return 1;
			}
			push_digit((double)strtod(*argv, NULL));
			n = 0;
		}

		else {
			if (!calc_is_operator(**argv) || strlen(*argv) >= 2) {
				fprintf(stderr, "ERROR:%s is not a valid operator\n", *argv);
				return 1;
			}
			push_operator(**argv);
			n = 1;
		}
	}

	*result = calculate_entry(operator_stack, number_stack);
	return 0;
}