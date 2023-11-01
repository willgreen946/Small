#include "ttyctrl.h"

int
ttysize(struct TTY tty)
{
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &tty) < 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

void
clrscr(void)
{
  write(STDOUT_FILENO, "\033[H\033[J", 6);
}

char
getch(void)
{
  char c = '\0';

  if (read(STDIN_FILENO, &c, 1) < 0)
    return '\0';

  return c;
}

char
fdgetch(int fd)
{
  char c = '\0';

  if (read(fd, &c, 1) < 0)
    return '\0';

  return c;
}
