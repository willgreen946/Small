#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>

#define FILE_TYPE 1
#define DIR_TYPE 2

typedef uint8_t type_t;

/* Total size of all the files combined */
size_t total_size = 0;

/* Count of all directorys and files */
size_t filec = 0, dirc = 0;

/* Prints the type of the file and returns the type as a type_t */
type_t dir_print_type (uint8_t type) {
 switch (type) {
	case DT_DIR:
	 fprintf(stdout, "<DIR>\t");
	 return DIR_TYPE;
	default:
	 fprintf(stdout, "\t");
	 return FILE_TYPE;
 }

 return 0;
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

 /* Remove \n */
 tms[strlen(tms) - 1] = (char) 0;

 fprintf(stdout, "%s\t", tms);
}

/* Returns the size of a file */
size_t get_size (const char *fn) {
 size_t size = 0;
 FILE *fp;

 if (!(fp = fopen(fn, "r")))
	return 0;

 fseek(fp, 0, SEEK_END);

 size = ftell(fp);
 fclose(fp);
 return size;
}

/* Starts printing out directory information to the screen */
void dir_print (struct dirent *dir) {
 type_t type;

 dir_print_time(dir->d_name);
 type = dir_print_type(dir->d_type);

 switch (type) {
	case FILE_TYPE:
	 ++filec;
	 break;
	case DIR_TYPE:
	 ++dirc;
	 break;
 }

 /* Increment total file size */
 total_size += get_size(dir->d_name);

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

 /* Print the file count */
 fprintf(stdout, "\t%zu File(s) %zu bytes\n", filec, total_size);

 /* Print the directory count */
 fprintf(stdout, "\t%zu Dir(s)\t\n", dirc);
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