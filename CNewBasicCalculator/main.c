#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

static double num_stack[255];
static char op_stack[127];

void
clear_stack(void)
{
	size_t i;

	for (i = 0; num_stack[i]; i++)
		num_stack[i] = 0;

	for (i = 0; op_stack[i]; i++)
		op_stack[i] = (char)0;
}

void
push_num(const double num)
{
	size_t i;

	for (i = 0; num_stack[i] != 0; i++)
		;

	num_stack[i] = num;
}

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
		default:
			fprintf(stderr, "ERROR:No such operator %c\n", op);
			exit(1);
	}
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

	fprintf(stdout, "%f\n", result);
}

/* Goes through argv array from cmd line */
int
parse_args(const char ** argv)
{
	int n = 1;

	while (*++argv) {
		if (n) {
			push_num((double)strtod(*argv, NULL));
			n = 0;
		}

		else {
			push_op(**argv);
			n = 1;
		}
	}

	calculate_entry(op_stack, num_stack);
	return 0;
}

void
event_loop(void) {
	for (;;) {
	}
}

int
program_setup(void)
{
	event_loop();
	return 0;
}

int
main(int argc, const char ** argv)
{
	return (argc < 4) ? program_setup() : parse_args(argv);
}