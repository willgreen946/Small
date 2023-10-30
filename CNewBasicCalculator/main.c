#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "calc.h"

/*
 * Simple help function
 */
int
help(void)
{
	fputs("USAGE:\n", stderr);
	fputs("nbc [string] e.g 'nbc 5 + 5 x 20'\n", stderr);
	fputs("nbc -f [file] e.g 'nbc -f myFile.txt'\n", stderr);
	return EXIT_FAILURE;
}

/*
 * Removes newline char at end of string
 */
void
rm_newline(char * str)
{
	size_t len = strlen(str);

	if (str[len - 1] == '\n')
		str[len - 1] = (char)0;
}

/*
 * Splits up a string by whitespace
 * Places results into char ** 
 */
void
split_str(char ** argv, char * str, const char * delim, size_t cap)
{
	char ** p;

	for (p = argv; p < &argv[cap] && (*p = strsep(&str, delim));)
		if (**p)
			p++;
}

/*
 * Follows the num op num op assumption
 * Reads the file line by line,
 * Calculating each line and printing it
 * Reads in a maximum of 384 bytes per line
 */
int
read_file(FILE * fp)
{
	size_t i;
	double result = 0;
	char buf[TOTAL_STACK_SIZE*2];
	char * argv[TOTAL_STACK_SIZE];

	while (fgets(buf, (TOTAL_STACK_SIZE*2) - 1, fp)) {
		rm_newline(buf);

		if (CALC_VERBOSE_F)
			fprintf(stdout, "%s = ", buf);

		split_str((char**)argv, buf, " \t", TOTAL_STACK_SIZE);

		if (calc_parse_args((const char**)argv, &result, 0))
			return EXIT_FAILURE;

		calc_clear_stack();
		fprintf(stdout, "%0.4f\n", result);

		/* Clear argv array */
		for (i = 0; argv[i]; i++)
			memset(argv[i], 0, strlen(argv[i]));
	}

	return EXIT_SUCCESS;
}

/*
 * Attempts to open all arguments of argv,
 * The function starts from the offset.
 */
int
open_file(const char ** argv, int offset)
{
	FILE * fp;
	size_t i;

	for (i = offset; argv[i]; i++) {
		if (!(fp = fopen(argv[i], "r"))) {
			fprintf(stderr, "ERROR:fopen \"%s\":%s\n", argv[i], strerror(errno));
			return EXIT_FAILURE;
		}

		if (read_file(fp)) {
			if (fclose(fp)) {
				fprintf(stderr, "ERROR:fclose \"%s\":%s\n", argv[i], strerror(errno));
				return EXIT_FAILURE;
			}

			return EXIT_FAILURE;
		}

		if (fclose(fp)) {
			fprintf(stderr, "fclose \"%s\":%s\n", argv[i], strerror(errno));
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
setup(const char ** argv)
{
	int i;
	int offset = 1;
	short file = FALSE;
	double result = 0;

	/* Parse command line args */	
	for (i = 0; argv[i]; i++) {
		if (!strncmp(argv[i], "-f", 2)) {
			file = TRUE;
			offset++;
		}

		else if (!strncmp(argv[i], "-v", 2)) {
			CALC_VERBOSE_F = TRUE;
			offset++;
		}

		else if (!strncmp(argv[i], "-h", 2))
			return help();
	}

	if (file)	
		return open_file(argv, offset);

	else if (!calc_parse_args(argv, &result, offset)) {
		fprintf(stdout, "%0.4f\n", result);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

int
event_loop(void)
{
	for (;;)
		read_file(stdin);
	
	return EXIT_SUCCESS;
}

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? event_loop() : setup(argv);
}
