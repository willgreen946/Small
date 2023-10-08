#include <iostream>
#include <cstring>
#include "string.hpp"

int main (int argc, char **argv)
{
	char *p = (char*)malloc(8);
	str::ncat(p, "hello world this should fail", 8);
	std::cout << p << '\n';
	free(p);
	return 0;
}
