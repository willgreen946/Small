#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifndef EXIT_FAILURE
	#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

#ifndef EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */

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
double CalculateMod(const double, const double);

/*
 * Returns the first argument to the power of the second
 */
double CalculatePow(const double, const double);

/*
 * Pushes double to the number stack
 */
static int PushDigit(const double);

/*
 * Pushes char to the operator stack
 */
static int PushOperator(const char);

/*
 * Flags that can be passed at the command line
 */
bool CALC_VERBOSE_F = false;

/*
 * Resets the values in both number & operator stacks
 * Sets the number_stack_count & operator_stack_count to 0
 */
void ClearCalculationStack(void);

/*
 * Sets sum of n1 (op) n2 in the double *, returns EXIT_FAILURE if op is not an operator
 */
static int Calculate(const double, const char, const double, double *);

/*
 * Returns 1 if the string is numeric
 */
static int IsNumeric(const char *);

/*
 * Goes through the operator stack and the number stack
 */
static double CalculateLoop(const char *, const double[]);

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

enum {
	NUMBER_STACK_MAX = 128,
	OPERATOR_STACK_MAX = 64,
	TOTAL_STACK_SIZE = NUMBER_STACK_MAX + OPERATOR_STACK_MAX
};

/*
 * Stores all the digits to be used in the calculation
 */
static double numberStack[NUMBER_STACK_MAX];

/*
 * Stores number of all numbers in the stack
 */
static int numberStackCount = 0;

/*
 * Stores all the operators to be used in the calculation
 */
static char operatorStack[OPERATOR_STACK_MAX];

/*
 * Stores number of all operators in the stack
 */
static int operatorStackCount = 0;

double
CalculateAdd(const double n1, const double n2)
{
	return (n1 + n2);
}

double
CalculateSub(const double n1, const double n2)
{
	return (n1 - n2);
}

double
CalculateMul(const double n1, const double n2)
{
	return (n1 * n2);
}

double
CalculateDiv(const double n1, const double n2)
{
	return (n1 / n2);
}

double
CalculateMod(const double n1, const double n2)
{
	return ((long)n1 % (long)n2);
}

double
CalculatePow(const double n1, const double n2)
{
	return pow(n1, n2);
}

int 
PushDigit(const double num)
{
	if (numberStackCount < NUMBER_STACK_MAX)
		numberStack[numberStackCount++] = num;

	else {
		fprintf(stderr, "ERROR:Too many numbers in stack\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int 
PushOperator(const char op)
{
	if (operatorStackCount < OPERATOR_STACK_MAX)
		operatorStack[operatorStackCount++] = op;

	else {
		fprintf(stderr, "ERROR:Too many operators in stack\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void
ClearCalculationStack(void)
{
	int i;

	for (i = 0; numberStack[i]; i++)
		numberStack[i] = 0;

	for (i = 0; operatorStack[i]; i++)
		operatorStack[i] = (char)0;

	operatorStackCount = numberStackCount = 0;
}

static int 
Calculate(const double n1, const char op, const double n2, double * result)
{
	switch (op) {
		case '+':
			*result = CalculateAdd(n1,n2);
			return EXIT_SUCCESS;
		case '-':
			*result = CalculateSub(n1,n2);
			return EXIT_SUCCESS;
		case '*':
			*result = CalculateMul(n1,n2);
			return EXIT_SUCCESS;
		case 'x':
			*result = CalculateMul(n1,n2);
			return EXIT_SUCCESS;
		case 'X':
			*result = CalculateMul(n1,n2);
			return EXIT_SUCCESS;
		case '/':
			*result = CalculateDiv(n1,n2);
			return EXIT_SUCCESS;
		case '^':
			*result = CalculatePow(n1,n2);
			return EXIT_SUCCESS;
		case '~':
			*result = CalculatePow(n1,n2);
			return EXIT_SUCCESS;
		case '%':
			*result = CalculateMod(n1,n2);
			return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

static int
IsNumeric(const char * str)
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
CalculateLoop(const char * op, const double num[])
{
	size_t i;
	size_t k;
	double active, result;

	active = result = 0;

	for (i = 0, k = 0; num[i]; i++) {
		active = num[i];

		if (!i)
			Calculate(active, op[k++], num[++i], &result);
		else
			Calculate(result, op[k++], active, &result);
	}

	return result;
}

int
CalculateEntry(const char ** argv, double * result, int offset)
{
	short n = 1;
	size_t i;
	double test;

	for (i = offset; argv[i]; i++) {
		if (n) {
			if (IsNumeric(argv[i])) {
				fprintf(stderr, "%s is not numeric\n", argv[i]);
				return EXIT_FAILURE;
			}

			if (PushDigit((double)strtod(argv[i], NULL)))
				return EXIT_FAILURE;

			n = 0;
		}

		else {
			if (Calculate(1, *argv[i], 1, &test) || strlen(argv[i]) >= 2) {
				fprintf(stderr, "%s is not a valid operator\n", argv[i]);
				return EXIT_FAILURE;
			}
	
			if (PushOperator(*argv[i]))
				return EXIT_FAILURE;

			n = 1;
		}
	}

	*result = CalculateLoop(operatorStack, numberStack);
	return EXIT_SUCCESS;
}
