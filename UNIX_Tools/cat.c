#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/*
 * Reads a file byte by byte
 * Prints the char to the screen
 * Calls write a lot could be very slow
 */
int
read_file(const int fd)
{
	char c;
	ssize_t ret;

	while ((ret = read(fd, &c, 1))) {
		if (ret < 0) {
			err(errno, "read");
			return EXIT_FAILURE;
		}

		write(STDOUT_FILENO, &c, 1);
	}

	return EXIT_SUCCESS;
}

/*
 * Starts the program
 */
int
entry(const char ** argv)
{
	int fd;

	while (*++argv) {
		if ((fd = open(*argv, O_RDONLY)) < 0) {
			err(errno, "open");
			return EXIT_FAILURE;
		}

		read_file(fd);

		if (close(fd) < 0) {
			err(errno, "close");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int
main(int argc, const char ** argv)
{
	return (argc < 2) ? 0 : entry(argv);
}
