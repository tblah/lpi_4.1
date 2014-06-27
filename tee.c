#include <sys/stat.h>	// for open
#include <fcntl.h>		// for open
#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit

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
	

	exit(EXIT_SUCCESS);
}