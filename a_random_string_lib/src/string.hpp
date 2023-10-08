#ifndef STRING_HPP
#define STRING_HPP

/* Included for malloc */
#include <cstdlib>

namespace str
{
	/* size types used locally */ 
	typedef unsigned long size_u;
	typedef signed long size_s;

	/* Returns the length of a string */
	str::size_u len (const char *string);

	/* Returns length of a string but -1 if it's over the max */
	str::size_u nlen (const char *string, str::size_u max);

	/* Concatonates a string onto another string */
	void cat (char *destination, const char *source);

	/* concatonates a string until it reaches the max */
	void ncat (char *destination, const char *source, str::size_u max);

	/* Compares two strings returns 0 if they match */
	int cmp (const char *one, const char *two);

	/* Compares two strings returns 0 if they match and are under the max*/
	int ncmp (const char *one, const char *two, str::size_u max);

	/* Returns a char * to a string matching the value of source */
	char *dup (const char *source);

	/* Returns a char * to a string matching the value of source as long as it's under the max */
	char *ndup (const char *source, str::size_u max);

	/* Copys char * to another char * */
	void cp (char *destination, const char *source);

	/* Copys char * to another char * as long it's under the max */
	int ncp (char *destination, const char *source, str::size_u max);
}

#include "string.cpp"

#endif // STRING_HPP
