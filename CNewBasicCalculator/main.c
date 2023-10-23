#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

static double num_stack[255];
static char op_stack[127];

/* Sets all stack values to 0 */
void
clear_stack(void)
{
	size_t i;

	for (i = 0; num_stack[i]; i++)
		num_stack[i] = 0;

	for (i = 0; op_stack[i]; i++)
		op_stack[i] = (char)0;
}

/* Pushes a double to the number stack */
void
push_num(const double num)
{
	size_t i;

	for (i = 0; num_stack[i]; i++)
		;

	num_stack[i] = num;
}

/* Pushes an char to the operator stack */
void
push_op(const char op)
{
	size_t i;

	for (i = 0; op_stack[i] != (char)0; i++)
		;

	op_stack[i] = op;
}

double
calculate(const double n1, const char op, const double n2)
{
	switch (op) {
		case '+':
			return (n1 + n2);
		case '-':
			return (n1 - n2);
		case 'x':
		/* Fall through */
		case 'X':
			return (n1 * n2);
		case '/':
			return (n1 / n2);
		case '^':
			return pow(n1, n2);
	}
	return 0.0;
}

void
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

	fprintf(stdout, "%0.3f\n", result);
}

/* Returns 1 if string is not convertable to a float */
int
is_numeric(const char * str)
{
	unsigned int dot_count = 0;
	size_t i;

	if (strspn(str, "+-.0123456789") != strlen(str))
		return 0;

	for (i = 0; str[i]; i++) {
		if (i != 0 && str[i] == '-')
			return 0;

		if (i != 0 && str[i] == '+')
			return 0;

		if (str[i] == '.')
			++dot_count;
	}

	if (dot_count >= 2)
		return 0;

	return 1;
}

int
is_operator(const char op)
{
	switch (op) {
		case '+':
		case '-':
		case 'x':
		case 'X':
		case '/':
		case '^':
		case '%':
			return 1;
	}
	return 0;
}

/*
 * Goes through argv array from cmd line,
 * pushes numbers and operators their,
 * corresponding stacks then starts the,
 * calculation, uses a 1 + 1 assumption.
*/
int
parse_args(int argc, const char ** argv)
{
	int n = 1;

	if (argc < 4)
		return 1;

	while (*++argv) {
		if (n) {
			if (!is_numeric(*argv)) {
				fprintf(stderr, "ERROR:%s is not numeric\n", *argv);
				exit(1);
			}
			push_num((double)strtod(*argv, NULL));
			n = 0;
		}

		else {
			if (!is_operator(**argv) || strlen(*argv) >= 2) {
				fprintf(stderr, "ERROR:%s is not a valid operator\n", *argv);
				exit(1);
			}
			push_op(**argv);
			n = 1;
		}
	}

	calculate_entry(op_stack, num_stack);
	return 0;
}

int
event_loop(void) {
	for (;;) {
	}
	return 0;
}

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? event_loop() : parse_args(argc, argv);
}