#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "calc.h"

/*
 * Simple help function
 */
int
Help(void)
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
RemoveNewline(char * str)
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
SplitString(char ** argv, char * str, const char * delim, size_t cap)
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
ReadFile(FILE * fp)
{
	size_t i;
	double result = 0;
	char buf[TOTAL_STACK_SIZE*2];
	char * argv[TOTAL_STACK_SIZE];

	while (fgets(buf, (TOTAL_STACK_SIZE*2) - 1, fp)) {
		RemoveNewline(buf);

		if (CALC_VERBOSE_F)
			fprintf(stdout, "%s = ", buf);

		SplitString((char**)argv, buf, " \t", TOTAL_STACK_SIZE);

		if (CalculateEntry((const char**)argv, &result, 0))
			return EXIT_FAILURE;

		ClearCalculationStack();
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
OpenFile(const char ** argv, int offset)
{
	FILE * fp;
	size_t i;

	for (i = offset; argv[i]; i++) {
		if (!(fp = fopen(argv[i], "r"))) {
			fprintf(stderr, "ERROR:fopen \"%s\":%s\n", argv[i], strerror(errno));
			return EXIT_FAILURE;
		}

		if (ReadFile(fp)) {
			if (fclose(fp)) {
				fprintf(stderr, "ERROR:fclose \"%s\":%s\n", argv[i], strerror(errno));
				return EXIT_FAILURE;
			}

			return EXIT_FAILURE;
		}

		if (fclose(fp)) {
			fprintf(stderr, "ERROR:fclose \"%s\":%s\n", argv[i], strerror(errno));
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
Setup(const char ** argv)
{
	int i;
	int offset = 1;
	bool file = false; 
	double result = 0;

	/* Parse command line args */	
	for (i = 0; argv[i]; i++) {
		if (!strncmp(argv[i], "-f", 2)) {
			file = true;
			offset++;
		}

		else if (!strncmp(argv[i], "-v", 2)) {
			CALC_VERBOSE_F = false;
			offset++;
		}

		else if (!strncmp(argv[i], "-h", 2))
			return Help();
	}

	if (file)	
		return OpenFile(argv, offset);

	else if (!CalculateEntry(argv, &result, offset)) {
		fprintf(stdout, "%0.4f\n", result);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

int
EventLoop(void)
{
	for (;;)
		ReadFile(stdin);
	
	return EXIT_SUCCESS;
}

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? EventLoop() : Setup(argv);
}
