#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {

	int n;
	char string[100];
        int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	
	int number = atoi(argv[2]); //converts char to int
	number = number - 1;	
	lseek(fd, number, SEEK_SET);
	write(fd, "", 1);
	close(fd);

}
