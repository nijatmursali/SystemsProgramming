#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "filedes.h"

//int main(int argc, char *argv[]) {
int fdinfo(int fd) {

	int flags, val;

	if((val = fcntl(fd, F_GETFL, 0)) < 0)
	//if((val = fcntl(atoi(argv[1]), F_GETFL, 0)) <0)
		perror("Error with fcntl.\n");

	flags = val & O_ACCMODE;


	switch(val & O_ACCMODE) {

	case O_RDONLY:
		printf("Read only \n");
		break;
	case O_WRONLY:
		printf("Write only\n");
		break;
	case O_RDWR:
		printf("Read and Write\n");
		break;
	case O_TRUNC:
		printf("Truncating\n");
		break;
	case O_EXCL:
		printf("Execute only\n");
		break;
	default:
		printf("Flag is not found\n");
	}

	if(val & O_APPEND) printf("Appending\n");
	if(val & O_NONBLOCK) printf("Nonblocking\n");
	if(val & O_SYNC) printf("Synchronous\n");

	/* 
		VERSION TWO


	if(flags == O_RDONLY) 
	 
		printf("Read Only\n");  
	
	
	else if(flags == O_WRONLY)
	 
		printf("Write only\n"); 
	
	
	else if(flags == O_RDWR)
	 
		printf("Read and Write\n"); 
	
	
	else if(flags == O_TRUNC)
	 
		printf("Truncating\n");
	
	
	else if(flags == O_EXCL)
	 
		printf("Must create a file\n");
	
	else 
		printf("Flag is not found\n");
	
	if(val & O_APPEND) 
	 
		printf("Append\n");
	
	
	if(val & O_NONBLOCK)
	
		printf("Nonblocking.\n");
	
	
	if(val & O_SYNC)
	
		printf("Synchronus writing\n.");
	
	*/

}
