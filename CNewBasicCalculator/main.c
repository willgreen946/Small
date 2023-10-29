#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

int
setup(const char ** argv)
{
	double result = 0.0;

	if (calc_parse_args(argv, &result))
		return 1;

	fprintf(stdout, "%0.4f\n", result);
	return 0;
}

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? EXIT_FAILURE : setup(argv);
}
