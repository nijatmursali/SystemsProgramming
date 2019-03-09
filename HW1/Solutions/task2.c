#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define BUFFSIZE 4096

int main(int argc, char* argv[]) {
	FILE *fd;
	char buf[BUFFSIZE];
	fd = fopen(argv[1], "r");
	//fd = open(argv[1], O_RDONLY);
	if(fd == NULL) { 
	perror("");
	return -1; // gonna print error if no file in directory
	}
	
	int currentLineLength = 0;
	int longestLength = 0;
	int shortestLentgh = 0;
	
	int totlines = 0;
	char currentchar;
	while((currentchar = getc(fd)) !=EOF) { // EOF is end of file
	//while((crrchar = read(fd, buf, BUFFSIZE)) != EOF) {
		currentLineLength++;
		if(currentchar == '\n'){
			if(shortestLentgh == 0){
				shortestLentgh = currentLineLength;
			}else if(shortestLentgh > currentLineLength){
				shortestLentgh = currentLineLength;
			}
			if(longestLength < currentLineLength){
				longestLength = currentLineLength;
			}
			totlines++;
			currentLineLength = 0;
		}
	}
	fclose(fd);	
	
	printf("Number of lines are: %d\n", totlines);
	printf("Length of longest line is: %d\n", longestLength-1);
	printf("Length of shortest line is: %d\n", shortestLentgh-1);

}
