/* This is a namespace that contains useful functions for char * operations */

#include <cstdlib>

namespace str
{
	typedef unsigned long size_u;
	typedef signed long size_s;

	/* returns the length of a string */
	str::size_u len (const char *string)
	{
		str::size_u k;

		for (k = 0; *string++; k++)
			;

		return k;
	}

	/* returns length of string as long as it's under the max */
	str::size_u nlen (const char *string, str::size_u max)
	{
		str::size_u k;

		for (k = 0; *string++; k++)
			;

		return (k <= max) ? k : 0;
	}

	/* concatonates a string */
	void cat (char *destination, const char *source)
	{
		do {
			*destination = *source;
		} while (*source++ && *destination++);
	}

	/* concatonates a string until it reaches the max */
	void ncat (char *destination, const char *source, str::size_u max)
	{
		for (str::size_u k = 0; k < max && *source; k++, *source++, *destination++)
			*destination = *source;
	}

	/* compares two strings, returns 1 if they are not the same */
	int cmp (const char *one, const char *two)
	{
		while (*one++ && *two++)
			if (*one != *two) return 1;

		return 0;
	}

	int ncmp (const char *one, const char *two, str::size_u max)
	{
		if (str::nlen(one, max) || str::nlen(two, max)) return 1;

		while (*one++ && *two++)
			if (*one != *two) return 1;

		return 0;
	}

	/* returns a char pointer to duplicated string */
	char *dup (const char *source)
	{
		char *string = (char*)malloc(str::len(source));

		if (string == NULL) return NULL;

		for (str::size_u k = 0; source[k]; k++)
			string[k] = source[k];

		return string;
	}

	/* returns a char* to a duplicated string of a certain size */ 
	char *ndup (const char *source, str::size_u max)
	{
		str::size_u len;
		char *string = ((len = str::len(source)) > max) ? NULL : (char*)malloc(len);

		if (string == NULL) return NULL;

		for (str::size_u k = 0; source[k]; k++)
			string[k] = source[k];

		return string;
	}

	/* copys a char * to another char * but returns -1 if it goes over max */ 
	int ncp (char *destination, const char *source, str::size_u max)
	{
		if (str::len(source) > max) return -1;

		while ((*destination++ = *source++))
			;

		return 0;
	}

	/* copys a char * to another char *  */
	void cp (char *destination, const char *source)
	{
		while ((*destination++ = *source++))
			;
	}
}
