#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "ttyctrl.h"

struct TTY tty;

void
printe(const char * str)
{
  write(STDERR_FILENO, str, strlen(str));
}

void
eventloop(const int fd)
{
  char c = '\0';

  for (;;) {
    ttysize(tty);

    c = getch();
  }
}

void
readfd(const int fd)
{
  char c = '\0';

  write(STDOUT_FILENO, BG_ANSI_RED, strlen(BG_ANSI_RED));

  while (read(fd, &c, 1)) {
    write(STDOUT_FILENO, &c, 1);
  }

  write(STDOUT_FILENO, ANSI_RESET, strlen(ANSI_RESET));
}

/*
 * Takes tty out of cooked mode,
 * Applys our new settings
 */
int
rawmode(void)
{
  struct termios raw;

  if (tcgetattr(STDIN_FILENO, &raw) < 0) {
    printe(strerror(errno));
    return EXIT_FAILURE;
  }

  /* Setting local flags (off) */
  raw.c_lflag &= ~(
    ICANON | ECHO | ECHOCTL | ECHONL | ECHOE);

  /* 1 byte is enough to return from read() */
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  /* Apply new terminal settings */
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
    printe(strerror(errno));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int
cleanup(void)
{
  return EXIT_SUCCESS;
}

/*
 * Starts the setup process
 * Trys to open every file
 * Treating argv[1+] as a file
 * Using file descriptors
 */
int
setup(const char ** argv)
{
  int fd;
  
  clrscr();
  rawmode();

  while (*++argv) {
    if ((fd = open(*argv, O_RDONLY)) < 0) {
      printe(strerror(errno));
      return EXIT_FAILURE;
    }

    readfd(fd);

    eventloop(fd);

    if (close(fd) < 0) {
      printe(strerror(errno));
      return EXIT_FAILURE;
    }
  }
  
  cleanup();
  return EXIT_SUCCESS;
}

int
main(int argc, const char ** argv)
{
  return (argc < 2) ? EXIT_FAILURE : setup(argv);
}
