#ifndef __TTYCTRL_H__
#define __TTYCTRL_H__

#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#ifndef EXIT_FAILURE
  #define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

#ifndef EXIT_SUCCESS
  #define EXIT_SUCCESS 0 
#endif /* EXIT_SUCCESS */

/*
 * Ansi color escape sequences (foreground)
 */
#define FG_ANSI_BLACK    "\x1b[30m"
#define FG_ANSI_RED      "\x1b[31m"
#define FG_ANSI_GREEN    "\x1b[32m"
#define FG_ANSI_YELLOW   "\x1b[33m"
#define FG_ANSI_BLUE     "\x1b[34m"
#define FG_ANSI_MAGENTA  "\x1b[35m"
#define FG_ANSI_CYAN     "\x1b[36m"
#define FG_ANSI_WHITE    "\x1b[37m"

/*
 * Ansi color escape sequences (bright foreground)
 */
#define FI_ANSI_BLACK    "\x1b[90m"
#define FI_ANSI_RED      "\x1b[91m"
#define FI_ANSI_GREEN    "\x1b[92m"
#define FI_ANSI_YELLOW   "\x1b[93m"
#define FI_ANSI_BLUE     "\x1b[94m"
#define FI_ANSI_MAGENTA  "\x1b[95m"
#define FI_ANSI_CYAN     "\x1b[96m"
#define FI_ANSI_WHITE    "\x1b[97m"

/*
 * Ansi color escape sequences (background)
 */
#define BG_ANSI_BLACK    "\x1b[40m"
#define BG_ANSI_RED      "\x1b[41m"
#define BG_ANSI_GREEN    "\x1b[42m"
#define BG_ANSI_YELLOW   "\x1b[43m"
#define BG_ANSI_BLUE     "\x1b[44m"
#define BG_ANSI_MAGENTA  "\x1b[45m"
#define BG_ANSI_CYAN     "\x1b[46m"
#define BG_ANSI_WHITE    "\x1b[47m"

/*
 * Ansi color escape sequences (bright background)
 */
#define BI_ANSI_BLACK    "\x1b[100m"
#define BI_ANSI_RED      "\x1b[101m"
#define BI_ANSI_GREEN    "\x1b[102m"
#define BI_ANSI_YELLOW   "\x1b[103m"
#define BI_ANSI_BLUE     "\x1b[104m"
#define BI_ANSI_MAGENTA  "\x1b[105m"
#define BI_ANSI_CYAN     "\x1b[106m"
#define BI_ANSI_WHITE    "\x1b[107m"

/*
 * Resets the color foreground or background
 */
#define ANSI_RESET "\x1b[0m"

struct TTY {
  struct winsize scr;
  struct termios tty;
};

/*
 * Uses ioctl to get terminal size
 * Sets the values in TTY struct under scr
 * Can be accessed via tty.scr.ws_* (row/col)
 * Returns EXIT_FAILURE on error
 * This error can be acessed via errno
 */
int ttysize(struct TTY);

/*
 * Clears the screen (stdout)
 */
void clrscr(void);

/*
 * Reads one char from stdin
 */
char getch(void);

/*
 * Reads one char from a file descriptor
 */
char fdgetch(int);

#include "ttyctrl.c"

#endif /* __TTYCTRL_H__ */
