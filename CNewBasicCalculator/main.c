#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

void
parse_string(char * str)
{
	char * argv[640];
	char ** p_argv;
	int argc;
	double result = 0.0;

	argv[0] = "NULL";

	/* TODO */
	/* Split up string by whitespace */
	for (p_argv = argv; (*++p_argv = strsep(&str, " \t"));)
		if (**p_argv)
			if (++p_argv >= &argv[640])
				break;

	for (argc = 0; argv[argc]; ++argc)
		;

	calc_parse_args((const char**)argv, &result);

	calc_clear_stack();

	for (argc = 0; argv[argc]; ++argc)
		memset(argv[argc], 0, strlen(argv[argc]));
}

int
event_loop(void) {
	char buf[512];

	for (;;) {
		fprintf(stdout, ">");
		if (fgets(buf, 511, stdin)) {
			parse_string(buf);
		}
	}
	return 0;
}

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
	return (argc < 2) ? event_loop() : setup(argv);
}