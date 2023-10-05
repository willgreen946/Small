#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

typedef signed long long I64; /* Wanting a 64 bit interger, so stole from HolyC */

bool hex_f, bin_f, silent_f;

/*
	 This program prints out a decimal,
	 that is enetered by the user as a
	 hexadeciamal number.
*/

int help (void) {
	fprintf(stderr,"usage:\n-h [int] -> hexadecimal\n-b [int] -> binary\n-s for silent output\n");
	return 0;
}

/* Converts 64 bit int to binary */
void print_bin (I64 num) {
	I64 num_bits = sizeof(num) * 8;
	I64 bit;

	if (!silent_f)
		printf("%lld -> ", num);

	/* Go through most significant bit to least significant bit */
	for (I64 i = (num_bits - 1); i >= 0; i--) {
		/* Shift bits to the right and bitwise && to get current bit */
		bit = (num >> i) & 1;
		printf("%lld", bit);
	}

	printf("\n");
}

void conv_bin (const char **argv) {
	I64 val;

	for (I64 i = 2; argv[i]; i++) {
		val = (I64)strtol(argv[i], NULL, 10);
		print_bin(val);
	}
}

/* Converts string to a signed long long (I64) and prints it as a hexadecimal */
void conv_hex (const char **argv) {
	I64 val;

	for (I64 i = 2; argv[i]; i++) {
		val = (I64)strtol(argv[i], NULL, 10); /* Convert string to base 10 */

		if (!silent_f)
			printf("%lld -> ", val);

		printf("0x%llx\n", val);
	}
}

int scan_args (const char **argv) {
	hex_f = bin_f = silent_f = false;

	/* Goes through the args passed at the cmd line */
	for (size_t i = 1; argv[i]; i++) {
		if (argv[i][0] == '-' && argv[i][1] != (char) 0)
			for (size_t k = 1; argv[i][k]; k++) {
				switch (argv[i][k]) {
					case 'h': hex_f = true; break;
					case 'b': bin_f = true; break;
					case 's': silent_f = true; break;
					default: printf("No such flag %c\n", argv[i][k]); goto fail;
				}
		}
	}

	if (hex_f) {
		conv_hex(argv);
		return 0;
	}

	if (bin_f) {
		conv_bin(argv);
		return 0;
	}

	fail:
		return help();
}

int main (int argc, const char **argv) {
	return (argc < 2) ? help() : scan_args(argv);
}
