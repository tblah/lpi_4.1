#include <sys/stat.h>	// for open
#include <fcntl.h>		// for open
#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <limits.h>		// for NAME_MAX
#include <string.h>		// for strncmp, strcmp
#include <unistd.h>		// for close, read, write

// implement tee: takes stdin and writes to stdout and a file given as an arguement
// if -a is specified before the file then data will be appended to the file, otherwiser it will replace the file

void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	char filename[NAME_MAX + 1]; 	// sets the filename buffer to the maximum allowed size of filename (the + 1 is to include the null byte)
	int openFlags = O_WRONLY | O_CREAT;
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH; // rw-rw-r--



	// handle command line arguements
	if (argc == 2) {							// set filename and truncate file on open
		strncpy(filename, argv[1], NAME_MAX);
		filename[NAME_MAX] = '\0';	// ensure this is NULL

		openFlags = openFlags | O_TRUNC;
	}
	else if (argc == 3) {
		if (!strcmp(argv[1], "-a")) {	// set filename and append on the file
			strncpy(filename, argv[2], NAME_MAX);
			filename[NAME_MAX] = '\0';

			openFlags = openFlags | O_APPEND;
		}
		else
			errExit("Usage: tee [-a] filename");
	}
	else 			// display usage instructions
		errExit("Usage: tee [-a] filename");



	// open the file
	int fd = open(filename, openFlags, openMode);
	if (fd == -1)
		errExit("Opening File");

	// read from file one byte at a time into output
	char byte; 
	size_t readReturn = read(STDIN_FILENO, &byte, 1);
	
	while (readReturn > 0)
	{
		if (write(fd, &byte, 1) != 1)
			errExit("Writing to file");

		if (write(STDOUT_FILENO, &byte, 1) != 1)
			errExit("Writing to stdout");

		readReturn = read(STDIN_FILENO, &byte, 1);
	}

	if (readReturn == -1)
		errExit("Reading File");



	// close the file
	if (close(fd) == -1)
		errExit("Closing File");

	exit(EXIT_SUCCESS);
}