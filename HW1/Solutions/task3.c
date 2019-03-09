#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFSIZE 4096

void fwdstdout(char *path){

	char *fname;

	int fd;
	int cnt;
	char *buf[BUFFSIZE];
	fd = open(STDIN_FILENO, O_CREAT | O_RDWR | O_TRUNC, 0777);

	while((cnt = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if(write(STDOUT_FILENO, buf, cnt) != cnt) {
			perror("writing failed");
		}
	}
	if(cnt < 0) perror("reading error");
}

int main(int argc, char *argv[]) {

	fwdstdout(argv[1]);
}
