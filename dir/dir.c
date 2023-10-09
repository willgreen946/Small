#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

/* Prints the type of the file */
void dir_print_type (uint8_t type) {
 switch (type) {
	case DT_DIR:
	 fprintf(stdout, "<DIR>\t");
	 break;
	case DT_BLK:
	 fprintf(stdout, "<BLOCK>\t");
	 break;
	case DT_SOCK:
	 fprintf(stdout, "<SOCKET>\t");
	 break;
	default:
	 fprintf(stdout, "\t");
	 break;
 }
}

/* Prints the time info without \n */
void dir_print_time (const char *str) {
 struct stat stat_t;
 struct tm *time_info;
 char *tms;
 
 /* Get the file creation date */
 stat(str, &stat_t);
 time_info = localtime(&stat_t.st_ctime);
 
 tms = asctime(time_info);
 tms[strlen(tms) - 1] = (char) 0; /* Remove \n */
 
 fprintf(stdout, "%s\t", tms);
 memset(tms, 0, strlen(tms));
}

/* Starts printing out directory information to the screen */
void dir_print (struct dirent *dir) {
 dir_print_time(dir->d_name);
 dir_print_type(dir->d_type);
 
 /* Print out the file name */
 fprintf(stdout, "%s\n", dir->d_name);
}

/* Opens the directory and passes it to printer */
int dir_open (const char *dn) {
 struct dirent *entry;
 DIR *dp;
 
 if (!(dp = opendir(dn))) {
	fprintf(stderr, "ERROR: Couldnt open directory %s\n", dn);
	return 1;
 }
 
 /* Read the directory contents */
 while ((entry = readdir(dp)))
	dir_print(entry);

 closedir(dp);
 return 0;
}

/* The entry point to the command */
int dir_entry (const char **argv) {
 if (!argv)
	return dir_open(".");

 while (*++argv) {
	if (dir_open(*argv))
	 return 1;
 }

 return 0;
}

int main (int argc, const char **argv) {
 return (argc < 2) ? dir_entry(NULL) : dir_entry(argv);
}